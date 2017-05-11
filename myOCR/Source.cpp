#include "stdafx.h"

//	������ ������ Pixel
void Pixel::get(const char& br)			// �������� �������� �������
{
	brightness = br;
}
char& Pixel::put()					// �������� �������� �������
{
	return brightness;
}

//	������ ������ Image
void Image::getPixel(const unsigned int& numberPix, const char& br)
{
	if (numberPix > height*width) exit(1);
	
	pixel[numberPix].get(br);
}
char Image::putPixel(const unsigned int& numberPix)
{
	if (numberPix > height*width) exit(1);

	return pixel[numberPix].put();
}
unsigned int Image::putHeight()
{
	return height;
}
unsigned int Image::putWidth()
{
	return width;
}
float Image::extremum(unsigned int posX, const unsigned int& widthMask)
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
char& Image::operator[](const unsigned int& numberPix)
{
	if (numberPix > height*width) exit(1);

	return pixel[numberPix].put();
}

//	������ ������ Strainer
void Strainer::filtering(Image& image, const unsigned int& x0, const unsigned int& xEnd)
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
				Filtered[0] = image[numberPix]; 
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
void Strainer::display()
{
	for (int i = 0; i < QF; i++)
		cout << setw(7) << Filtered[i];
}

//	������ ������ Sample
void Sample::selection(Image& image, Settings& user)
{
	char way=0;					// ����������� ������ ��������� ���������;
								// ���������� �����
	unsigned int width = image.putWidth();
	unsigned int widthMask = user.widthMask;
	unsigned int stepOffset = user.stepOffset;
	unsigned int minInterval = user.minInterval;
	unsigned int maxInterval = user.maxInterval;
	int numberOfSteps = ((width - widthMask) / stepOffset);
	int j = 0;					// ������ ������ ���������
	unsigned int posX = 0;		// ������� ������� �� X
	float MinCD;				// ����������� ��������� ����������
	int  nearestMatch;			// ��������� ������
	float tempCD;				// ������� �������� ����������� ����������

						// ������� ���� ���������. ����� �� ������ ����������� � �������� ����� 
	for (int i = 0; i <= numberOfSteps; i++)
	{
		posX += stepOffset;	// ���� ����� ������� �������� �������
		if (((image.extremum(posX - stepOffset, widthMask) < image.extremum(posX, widthMask))
			&& (image.extremum(posX, widthMask) > image.extremum(posX + stepOffset, widthMask)))
			|| ((image.extremum(posX - stepOffset, widthMask) > image.extremum(posX, widthMask))
				&& (image.extremum(posX, widthMask) < image.extremum(posX + stepOffset, widthMask))))
		{
			j = posX + minInterval;	// ���� ������ ������� �������� �������;
					while ((j+1-posX<maxInterval)&&(j+1<width))
					{
						if (((image.extremum(j - 1, widthMask) < image.extremum(j, widthMask))
							&& (image.extremum(j, widthMask) > image.extremum(j + 1, widthMask)))
							|| ((image.extremum(j - 1, widthMask) > image.extremum(j, widthMask))
								&& (image.extremum(j, widthMask) < image.extremum(j + 1, widthMask))))
							filtering(image, posX, j);
					}
		}
	}
}