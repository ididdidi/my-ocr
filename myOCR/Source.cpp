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

//	������ ������ Settings
void Settings::getMode()
{
	char ch;
	do{ 
		cout << "�������� ����� ��������(t) ��� ���������(p)";
		cin >> ch;
	} while (ch != 't' || ch != 'p' || ch != 'T' || ch != 'P' );
	(ch == 't' || ch == 'T') ? mode = true : mode = false;
}

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
		cout << " ������ ������� � ����";
	cout << " ������� ������... ";  cin >> match;

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

//	������ ������ Strainer
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
void Strainer::selection(Image& image, Settings& user)
{
									// ���������� �����
	unsigned int width = image.putWidth();
	unsigned int widthMask = user.widthMask;
	unsigned int stepOffset = user.stepOffset;
	unsigned int minInterval = user.minInterval;
	unsigned int maxInterval = user.maxInterval;
	unsigned int j = 0;			// ������ ������ ���������
	int posX = 0;		// ������� ������� �� X
	float MinCD;				// ����������� ��������� ����������
	char  nearestMatch = 0;		// ��������� ������
	cout << "selection..." << endl;
						// ������� ���� ���������. ����� �� ������ ����������� � �������� ����� 
	omp_lock_t lock;
	omp_init_lock(&lock);
#pragma omp parallel num_threads(1) shared(stepOffset)
	{
#pragma omp for schedule (guided) private(MinCD) firstprivate(nearestMatch,j) lastprivate(posX)
	for (posX = stepOffset; posX < width - widthMask; posX += stepOffset)
	{
		if (((image.valueF(posX - stepOffset, widthMask) < image.valueF(posX, widthMask))
			&& (image.valueF(posX, widthMask) > image.valueF(posX + stepOffset, widthMask)))
			|| ((image.valueF(posX - stepOffset, widthMask) > image.valueF(posX, widthMask))
				&& (image.valueF(posX, widthMask) < image.valueF(posX + stepOffset, widthMask))))
		{
			j = posX + minInterval;	// ���� ������ ������� �������� �������(����� ����������);
			while ((j + 1 - posX < maxInterval) && (j + 1 < width))
			{
				if (((image.valueF(j - 1, widthMask) < image.valueF(j, widthMask))
					&& (image.valueF(j, widthMask) > image.valueF(j + 1, widthMask)))
					|| ((image.valueF(j - 1, widthMask) > image.valueF(j, widthMask))
					&& (image.valueF(j, widthMask) < image.valueF(j + 1, widthMask))))
				{
					for (int i = 0; i < QF; i++)Filtered[i] = 0.0;
					Sample temp;
					temp.filtering(image, posX, j);
				/*	if (user.mode)
					{
						cout << " x0 = " << posX << " xEnd = " << j << " �������� � ����(y/n)?.. ";
						char ch;
						cin >> ch;
						if (ch == 'y')
							diskOut();
						cout << " ��������� ������ �� X0(���� ��� - 0): ";
						int offset = 0;
						cin >> offset;
						if (offset)
						{
							posX = offset;
							break;
						}
					}
					else */{
						nearestMatch = temp.compareWithBase(MinCD);
						omp_set_lock(&lock);
						compliance.push_back(Compliance(posX, j, nearestMatch, MinCD));
						omp_unset_lock(&lock);
					}
				}
				j++;
			}
		}
	}
    }
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
}