#include "stdafx.h"

//	������ ������ Pixel
void Pixel::get(const double& br)			// �������� �������� �������
{
	brightness = static_cast<char>(br);
}

unsigned char& Pixel::put()					// �������� �������� �������
{
	return brightness;
}
  
//	������ ������ Image

Image::Image(Settings& user)			// �������� �������� ������� �� �����
{
	char *fileName = user.fileName;

	// ��������� ����
	std::ifstream fileStream(fileName, std::ifstream::binary);
	if (!fileStream) {
		throw ImageEx("Download image", user.fileName);
	}

	// �������� �����������
	BITMAPFILEHEADER fileHeader;
	read(fileStream, fileHeader.bfType, sizeof(fileHeader.bfType));
	read(fileStream, fileHeader.bfSize, sizeof(fileHeader.bfSize));
	read(fileStream, fileHeader.bfReserved1, sizeof(fileHeader.bfReserved1));
	read(fileStream, fileHeader.bfReserved2, sizeof(fileHeader.bfReserved2));
	read(fileStream, fileHeader.bfOffBits, sizeof(fileHeader.bfOffBits));

	if (fileHeader.bfType != 0x4D42) {
		std::cout << "Error: '" << fileName << "' is not BMP file." << std::endl;
		return;
	}

	// ���������� �����������
	BITMAPINFOHEADER fileInfoHeader;
	read(fileStream, fileInfoHeader.biSize, sizeof(fileInfoHeader.biSize));

	// bmp core
	if (fileInfoHeader.biSize >= 12) {
		read(fileStream, fileInfoHeader.biWidth, sizeof(fileInfoHeader.biWidth));
		read(fileStream, fileInfoHeader.biHeight, sizeof(fileInfoHeader.biHeight));
		read(fileStream, fileInfoHeader.biPlanes, sizeof(fileInfoHeader.biPlanes));
		read(fileStream, fileInfoHeader.biBitCount, sizeof(fileInfoHeader.biBitCount));
	}

	// �������� ���������� � ��������
	int colorsCount = fileInfoHeader.biBitCount >> 3;
	if (colorsCount < 3) {
		colorsCount = 3;
	}

	int bitsOnColor = fileInfoHeader.biBitCount / colorsCount;
	int maskValue = (1 << bitsOnColor) - 1;

	// bmp v1
	if (fileInfoHeader.biSize >= 40) {
		read(fileStream, fileInfoHeader.biCompression, sizeof(fileInfoHeader.biCompression));
		read(fileStream, fileInfoHeader.biSizeImage, sizeof(fileInfoHeader.biSizeImage));
		read(fileStream, fileInfoHeader.biXPelsPerMeter, sizeof(fileInfoHeader.biXPelsPerMeter));
		read(fileStream, fileInfoHeader.biYPelsPerMeter, sizeof(fileInfoHeader.biYPelsPerMeter));
		read(fileStream, fileInfoHeader.biClrUsed, sizeof(fileInfoHeader.biClrUsed));
		read(fileStream, fileInfoHeader.biClrImportant, sizeof(fileInfoHeader.biClrImportant));
	}

	// bmp v2
	fileInfoHeader.biRedMask = 0;
	fileInfoHeader.biGreenMask = 0;
	fileInfoHeader.biBlueMask = 0;

	if (fileInfoHeader.biSize >= 52) {
		read(fileStream, fileInfoHeader.biRedMask, sizeof(fileInfoHeader.biRedMask));
		read(fileStream, fileInfoHeader.biGreenMask, sizeof(fileInfoHeader.biGreenMask));
		read(fileStream, fileInfoHeader.biBlueMask, sizeof(fileInfoHeader.biBlueMask));
	}

	// ���� ����� �� ������, �� ������ ����� �� ���������
	if (fileInfoHeader.biRedMask == 0 || fileInfoHeader.biGreenMask == 0 || fileInfoHeader.biBlueMask == 0) {
		fileInfoHeader.biRedMask = maskValue << (bitsOnColor * 2);
		fileInfoHeader.biGreenMask = maskValue << bitsOnColor;
		fileInfoHeader.biBlueMask = maskValue;
	}

	// bmp v3
	if (fileInfoHeader.biSize >= 56) {
		read(fileStream, fileInfoHeader.biAlphaMask, sizeof(fileInfoHeader.biAlphaMask));
	}
	else {
		fileInfoHeader.biAlphaMask = maskValue << (bitsOnColor * 3);
	}

	// bmp v4
	if (fileInfoHeader.biSize >= 108) {
		read(fileStream, fileInfoHeader.biCSType, sizeof(fileInfoHeader.biCSType));
		read(fileStream, fileInfoHeader.biEndpoints, sizeof(fileInfoHeader.biEndpoints));
		read(fileStream, fileInfoHeader.biGammaRed, sizeof(fileInfoHeader.biGammaRed));
		read(fileStream, fileInfoHeader.biGammaGreen, sizeof(fileInfoHeader.biGammaGreen));
		read(fileStream, fileInfoHeader.biGammaBlue, sizeof(fileInfoHeader.biGammaBlue));
	}

	// bmp v5
	if (fileInfoHeader.biSize >= 124) {
		read(fileStream, fileInfoHeader.biIntent, sizeof(fileInfoHeader.biIntent));
		read(fileStream, fileInfoHeader.biProfileData, sizeof(fileInfoHeader.biProfileData));
		read(fileStream, fileInfoHeader.biProfileSize, sizeof(fileInfoHeader.biProfileSize));
		read(fileStream, fileInfoHeader.biReserved, sizeof(fileInfoHeader.biReserved));
	}

	// �������� �� �������� ���� ������ �������
	if (fileInfoHeader.biSize != 12 && fileInfoHeader.biSize != 40 && fileInfoHeader.biSize != 52 &&
		fileInfoHeader.biSize != 56 && fileInfoHeader.biSize != 108 && fileInfoHeader.biSize != 124) {
		std::cout << "Error: Unsupported BMP format." << std::endl;
		return;
	}

	if (fileInfoHeader.biBitCount != 16 && fileInfoHeader.biBitCount != 24 && fileInfoHeader.biBitCount != 32) {
		std::cout << "Error: Unsupported BMP bit count." << std::endl;
		return;
	}

	if (fileInfoHeader.biCompression != 0 && fileInfoHeader.biCompression != 3) {
		std::cout << "Error: Unsupported BMP compression." << std::endl;
		return;
	}

	// ���������� ������ � ������ Image
	unsigned int bufer;

	height = fileInfoHeader.biHeight;
	width = fileInfoHeader.biWidth;

	// ����������� ������� ������� � ����� ������ ������
	int linePadding = ((fileInfoHeader.biWidth * (fileInfoHeader.biBitCount / 8)) % 4) & 3;

	if (!(height*width)) exit(2);
	if (pixel)
	{
		delete[]pixel;
	}
	pixel = new Pixel[height*width];
	//unsigned int k = 0;
	int i = 0;
	unsigned int j = 0;

#pragma omp parallel num_threads(omp_get_max_threads()/2)
	{
#pragma omp for schedule (guided) firstprivate(j) lastprivate(i) ordered
		for (i = 0; i < fileInfoHeader.biHeight; i++) {
#pragma omp ordered
			for (j = 0; j < fileInfoHeader.biWidth; j++) {
				read(fileStream, bufer, fileInfoHeader.biBitCount / 8);
				pixel[j + (fileInfoHeader.biWidth - 10)*i + 10 * i].get(0.3* bitextract(bufer, fileInfoHeader.biRedMask) + 0.59*bitextract(bufer, fileInfoHeader.biGreenMask) + 0.11*bitextract(bufer, fileInfoHeader.biBlueMask));
			}
			fileStream.seekg(linePadding, std::ios_base::cur);
		}
	}
	return;
}

unsigned char Image::bitextract(const unsigned int byte, const unsigned int mask) {
	if (mask == 0) {
		return 0;
	}

	// ����������� ���������� ������� ��� ������ �� �����
	int
		maskBufer = mask,
		maskPadding = 0;

	while (!(maskBufer & 1)) {
		maskBufer >>= 1;
		maskPadding++;
	}

	// ���������� ����� � ��������
	return (byte & mask) >> maskPadding;
}

unsigned int Image::putHeight()const
{
	return height;
}

unsigned int Image::putWidth()const
{
	return width;
}

float Image::valueF(unsigned int posX, const unsigned int& widthMask)
{
	float temp = 0.0;
	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < widthMask; j++, posX++)
		{
			if (j < (widthMask / 2)) temp -= pixel[posX].put();
			else temp += pixel[posX].put();
		}
		posX += width - widthMask;
	}
	return temp;
}

unsigned char& Image::operator[](const unsigned int& numberPix)const
{
	if (numberPix > height*width) exit(1);

	return pixel[numberPix].put();
}

bool Image::extremum(unsigned int posX, const int& stepOffset, const unsigned int& widthMask)
{
	return (((valueF(posX - stepOffset, widthMask) < valueF(posX, widthMask))
		&& (valueF(posX, widthMask) > valueF(posX + stepOffset, widthMask)))
		|| ((valueF(posX - stepOffset, widthMask) > valueF(posX, widthMask))
			&& (valueF(posX, widthMask) < valueF(posX + stepOffset, widthMask))));
}

//	������ ������ Settings


//	������ ������ Compliance
void Compliance::dispay()
{
	cout << endl << x0 << ' ' << xEnd << ' ' << nearestMatch << ' ' << CartesianDistance;
}

//	������ ������ Sample
char Sample::returnMatch()
{
	return match;
}

void Sample::filtering(const Image& image, const unsigned int& x0, const unsigned int& xEnd)
{
	unsigned int numberPix = x0;				// ��������� ���������� ��������� �������
	unsigned int widthMask = (xEnd - x0) / 4;	// ������������ ������ �� ������� �������
	widthMask *= 4;								
	unsigned int height = image.putHeight();	// ������ �����������;
	unsigned int width = image.putWidth();		// ������ �����������;
	
	// ��������������� ���������� ������� ����� ������ ������� 
	for (unsigned int i = 0; i < height; i++)	// ������� ���� ��������� ��������
	{
		for (unsigned int j = 0; j < widthMask; j++, numberPix++) // ���������� ���� ��������� ��������
		{
			// ����������� ������ 0
			{	
				Filtered[0] += image[numberPix]; 
			}
			// ����������� ������ 1										
			{
				if (j < (width / 2)) Filtered[1] -= image[numberPix];
				else Filtered[1] += image[numberPix];
			}	
		    // ����������� ������ 2
			{
				if (i < (height / 2)) Filtered[2] -= image[numberPix];
				else Filtered[2] += image[numberPix];
			}
			// ����������� ������ 3
			{
				if ((2 * i / height + 2 * j / widthMask) % 2) Filtered[3] += image[numberPix];
				else Filtered[3] -= image[numberPix];
			}
			// ����������� ������ 4
			{
				if (j <= (widthMask / 4) || j > (widthMask * 3 / 4)) Filtered[4] -= image[numberPix];
				else Filtered[4] += image[numberPix];
			}
			// ����������� ������ 5
			{
				if (j <= (height / 4) || j > (height * 3 / 4)) Filtered[5] -= image[numberPix];
				else Filtered[5] += image[numberPix];
			}
			// ����������� ������ 6
			{
				if (i <= (height / 2) && j <= (widthMask / 4) || i <= (height / 2) && j > (widthMask * 3 / 4) || i > (height / 2) && j > (widthMask / 4) && j <= (widthMask * 3 / 4))
					Filtered[6] -= image[numberPix];
				else Filtered[6] += image[numberPix];
			}
			// ����������� ������ 7
			{
				if (j <= (widthMask / 2) && i > (height / 4) && i <= (height * 3 / 4) || j > (widthMask / 2) && i <= (height / 4) || i > (height * 3 / 4) && j > (widthMask / 2))
					Filtered[7] -= image[numberPix];
				else Filtered[7] += image[numberPix];
			}
			// ����������� ������ 8
			{
				if (i > (height / 2) && j < (widthMask / 2) || i<(height / 2) && j>(widthMask / 2))
					if ((i / (height / 4) + j / (widthMask / 4)) % 2) Filtered[8] += image[numberPix];
					else Filtered[8] -= image[numberPix];
				else if ((i / (height / 4) + j / (widthMask / 4)) % 2) Filtered[8] -= image[numberPix];
				else Filtered[8] += image[numberPix];
			}
			// ����������� ������ 9
			{
				if ((j > (widthMask / 4) && j <= (widthMask / 2)) || j >= (widthMask * 3 / 4))
					Filtered[9] -= image[numberPix];
				else Filtered[9] += image[numberPix];
			}
			// ����������� ������ 10
			{
				if (i <= (height / 4) || (i > (height / 2) && i < (height * 3 / 4)))
					Filtered[10] -= image[numberPix];
				else Filtered[10] += image[numberPix];
			}
			// ����������� ������ 11
			{
				if (i <= (height / 2))
					if (j>(widthMask / 4) && j <= (widthMask / 2) || j >= (widthMask * 3 / 4))Filtered[11] -= image[numberPix];
					else Filtered[11] += image[numberPix];
				else if (j <= (widthMask / 4) || j>(widthMask / 2) && j<(widthMask * 3 / 4))Filtered[11] -= image[numberPix];
				else Filtered[11] += image[numberPix];
			}
			// ����������� ������ 12
			{
				if (j<(widthMask / 2))
					if ((i>(height / 4) && i <= (height / 2)) || i>(height * 3 / 4))Filtered[12] -= image[numberPix];
					else Filtered[12] += image[numberPix];
				else if (i <= (height / 4) || (i >= (height / 2) && i<(height * 3 / 4)))Filtered[12] -= image[numberPix];
				else Filtered[12] += image[numberPix];
			}
			// ����������� ������ 13
			{
				if (i<(height / 2))
					if ((i / (height / 4) + j / (widthMask / 4)) % 2)Filtered[13] += image[numberPix];
					else Filtered[13] -= image[numberPix];
				else if ((i / (height / 4) + j / (widthMask / 4)) % 2)Filtered[13] -= image[numberPix];
				else Filtered[13] += image[numberPix];
			}
			// ����������� ������ 14
			{
				if (j<(widthMask / 2))
					if ((i / (height / 4) + j / (widthMask / 4)) % 2)Filtered[14] -= image[numberPix];
					else Filtered[14] += image[numberPix];
				else if ((i / (height / 4) + j / (widthMask / 4)) % 2)Filtered[14] += image[numberPix];
				else Filtered[14] -= image[numberPix];
			}
			// ����������� ������ 15
			{
				if ((i / (height / 4) + j / (widthMask / 4)) % 2)Filtered[15] += image[numberPix];
				else Filtered[15] -= image[numberPix];
			}
		}
		numberPix += width - widthMask;
	}
}

void Sample::diskOut()						// ������ � ����� �����.
{
		cout << "\tRecord a sample in the database";
	cout << "\t\tEnter the symbol... ";  cin >> match;

	ofstream outfile;						// ������� ����� ������
	outfile.open("MatchBase.dat", ios::app | ios::out | ios::binary); // ������� ��� ������
	outfile.write((char*)this, sizeof(*this));	// ������ �������;
	outfile.close();	
}

float Sample::operator - (const Sample& match)
{
	long double tempDC = 0.0;
	for (int i = 0; i < QF; i++)
	{
		double minus = Filtered[i] - match.Filtered[i];
		tempDC += minus*minus;
	}
	return (float)(sqrt(tempDC));
}

char Sample::compareWithBase(float& MinCD) //���������� � ���������, ���������� ���������
{											 // � ���������� �� ����(&MinCD);
	char  nearestMatch = 0;
	float tempCD;				// ������� �������� ����������� ����������
	Sample tempSample;			// ����� ��� ���������� �������� ��������
	fstream infile;				// ������� ����� ����� �� �����;
	infile.open("MatchBase.dat", ios::in | ios::binary);
	if (!infile) exit(3);
	// ��������� �������� ������� �� ��������� �������:
	infile.read(reinterpret_cast<char*>(&tempSample), sizeof(tempSample));
	while (!infile.eof())
	{
		tempCD = *this - tempSample;	// ������� ��������� ����������

		if (!nearestMatch || MinCD > tempCD)// ���� ��� ������ ��������� ���  
		{									// ���������� ��������� ��������� ������
			MinCD = tempCD;					// ��������� ��� ��� ����������� 
											// ���������� ������ ��� �������� �������
			nearestMatch = tempSample.returnMatch();
		}						// ��������� �������� ������� �� ��������� �������:
		infile.read(reinterpret_cast<char*>(&tempSample), sizeof(tempSample));
	}
	infile.close();
	return nearestMatch;		// ������ ��������� ������
}

void Sample::training(Image& image, Settings& user)
{
	// ���������� �����
	unsigned int width = image.putWidth();
	unsigned int rightEdge = 0;			// ������ ������� ������������ ��������
	unsigned int leftEdge = 0;				// ������� ������� �� X

	char resume;
	do {
		do {
			cout << "\nEnter the left character border...";
			cin >> leftEdge;
		} while (leftEdge > (width - user.widthMask) || leftEdge < 0);
		do {
			cout << "Enter the right character border...";
			cin >> rightEdge;
		} while ((rightEdge - leftEdge) < user.minInterval || rightEdge > width || (rightEdge - leftEdge) > user.maxInterval);

		char ch = 0;
		// ������� ���� ���������. ����� �� ������ ����������� � �������� ����� 
		for ( ; leftEdge < width - user.widthMask; leftEdge += user.stepOffset)
		{
			if (image.extremum(leftEdge, user.stepOffset, user.widthMask))
			{
				// ���� ������ ������� �������� �������(����� ����������);
				while ((rightEdge - leftEdge > user.minInterval) && (rightEdge < width) && rightEdge > leftEdge)
				{
					if (image.extremum(rightEdge, 1, user.widthMask))
					{

						cout << "The actual border: l = " << leftEdge << " r = " << rightEdge;
						cout << "To preserve the value found(y/n)? ";
						cin >> ch;
						if (ch == 'y' || ch == 'Y')
						{
							for (int i = 0; i < QF; i++)Filtered[i] = 0.0;
							filtering(image, leftEdge, rightEdge);
							diskOut();
							break;
						}
					}
					rightEdge--;
				}
			}
			if (ch == 'y' || ch == 'Y')
				break;
		}
		cout << "Continue entering(y/n)? ";
		resume = _getch();
	} while (resume == 'y' || resume == 'Y');
}

//	������ ������ Strainer
void Strainer::selection(Image& image, Settings& user)
{
									// ���������� �����
	unsigned int width = image.putWidth();
	unsigned int j = 0;			// ������ ������� ������������ ��������
	int posX = 0;				// ������� ������� �� X
	float MinCD;				// ����������� ��������� ����������
	char  nearestMatch = 0;		// ��������� ������
	cout << "selection..." << endl;
						// ������� ���� ���������. ����� �� ������ ����������� � �������� ����� 
	omp_lock_t lock;
	omp_init_lock(&lock);
	int dinamic_threads = omp_get_dynamic();
	omp_set_dynamic(1);
#pragma omp parallel // shared(stepOffset)
	{
#pragma omp for schedule (guided) private(MinCD) firstprivate(nearestMatch,j) lastprivate(posX)
	for (posX = user.stepOffset; posX < width - user.widthMask; posX += user.stepOffset)
	{
		if (image.extremum(posX, user.stepOffset, user.widthMask))
		{
			j = posX + user.maxInterval;	// ���� ������ ������� �������� �������(����� ����������);
			while ((j - posX > user.minInterval) && (j < width))
			{
				if (image.extremum(j, 1, user.widthMask))
				{
					Sample temp;
					temp.filtering(image, posX, j);
					nearestMatch = temp.compareWithBase(MinCD);
					omp_set_lock(&lock);
					compliance.push_back(Compliance(posX, j, nearestMatch, MinCD));
					omp_unset_lock(&lock);
				}
				j--;
			}
		}
	}
    }
	omp_set_dynamic(dinamic_threads);
	omp_destroy_lock(&lock);
}

void Strainer::minimize(Settings& user)
{
	compliance.sort();
	list<Compliance>::iterator  iterLeft, iterRight;
	iterLeft = compliance.begin();
	iterRight = compliance.begin();
	iterRight++;

	for (; iterRight != compliance.end();)
	{
		float layering = (iterLeft->xEnd - iterRight->x0);
		float minW = (((iterLeft->xEnd - iterLeft->x0) < (iterRight->xEnd - iterRight->x0)) ? (iterLeft->xEnd - iterLeft->x0) : (iterRight->xEnd - iterRight->x0));
		if(layering / minW > (user.percentOverlay / 100))
			if (iterLeft->CartesianDistance > iterRight->CartesianDistance)
			{
				iterLeft = compliance.erase(iterLeft);
				iterRight++;
			}
			else
			{
				iterRight = compliance.erase(iterRight);
			}
		else
		{
			iterLeft = iterRight;
			iterRight++;
		}
	}
}

void Strainer::display()
{
	list<Compliance>::iterator iter;
	for (iter = compliance.begin(); iter != compliance.end(); ++iter)
		iter->dispay();
	cout << endl;
}