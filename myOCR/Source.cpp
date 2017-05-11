#include "stdafx.h"

//	методы класса Pixel
void Pixel::get(const char& br)			// получить значение яркости
{
	brightness = br;
}
char& Pixel::put()					// передать значение яркости
{
	return brightness;
}

//	методы класса Image
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
char& Image::operator[](const unsigned int& numberPix)
{
	if (numberPix > height*width) exit(1);

	return pixel[numberPix].put();
}

//	методы класса Filters
float Filters::filter0(Image& image,  unsigned int numberPix, const int& widthMask)
{
	unsigned int heigth = image.putHeight();
	unsigned int width = image.putWidth();
	float temp = 0.0;
	for (unsigned int i=0; i < heigth; i++)
	{
		for (int j = 0; j < widthMask; j++)
		{
			temp += image[numberPix++];
		}
		numberPix += width - widthMask;
	}
	return temp;
}
float Filters::filter1(Image& image, unsigned int numberPix, const int& widthMask)
{
	unsigned int heigth = image.putHeight();
	unsigned int width = image.putWidth();
	float temp = 0.0;
	for (unsigned int i = 0; i < heigth; i++)
	{
		for (int j = 0; j < widthMask; j++)
		{
			if (j < (width / 2)) temp -= image[numberPix++];
			else temp += image[numberPix++];
		}
		numberPix += width - widthMask;
	}
	return temp;
}
float Filters::filter2(Image& image, unsigned int numberPix, const int& widthMask)
{
	unsigned int heigth = image.putHeight();
	unsigned int width = image.putWidth();
	float temp = 0.0;
	for (unsigned int i = 0; i < heigth; i++)
	{
		for (int j = 0; j < widthMask; j++)
		{
			if (i < (heigth / 2)) temp -= image[numberPix++];
			else temp += image[numberPix++];
		}
		numberPix += width - widthMask;
	}
	return temp;
}
float Filters::filter3(Image& image, unsigned int numberPix, const int& widthMask)
{
	unsigned int heigth = image.putHeight();
	unsigned int width = image.putWidth();
	float temp = 0.0;
	for (unsigned int i = 0; i < heigth; i++)
	{
		for (int j = 0; j < widthMask; j++)
		{
			if ((2 * i / heigth + 2 * j / widthMask) % 2) temp += image[numberPix++];
			else temp -= image[numberPix++];
		}
		numberPix += width - widthMask;
	}
	return temp;
}
float Filters::filter4(Image& image, unsigned int numberPix, const int& widthMask)
{
	unsigned int heigth = image.putHeight();
	unsigned int width = image.putWidth();
	float temp = 0.0;
	for (unsigned int i = 0; i < heigth; i++)
	{
		for (int j = 0; j < widthMask; j++)
		{
			if (j<=(widthMask/4) || j>(widthMask*3/4) ) temp -= image[numberPix++];
			else temp += image[numberPix++];
		}
		numberPix += width - widthMask;
	}
	return temp;
}
float Filters::filter5(Image& image, unsigned int numberPix, const int& widthMask)
{
	unsigned int heigth = image.putHeight();
	unsigned int width = image.putWidth();
	float temp = 0.0;
	for (unsigned int i = 0; i < heigth; i++)
	{
		for (int j = 0; j < widthMask; j++)
		{
			if (j<=(heigth / 4) || j>(heigth * 3 / 4)) temp -= image[numberPix++];
			else temp += image[numberPix++];
		}
		numberPix += width - widthMask;
	}
	return temp;
}
float Filters::filter6(Image& image, unsigned int numberPix, const int& widthMask)
{
	unsigned int heigth = image.putHeight();
	unsigned int width = image.putWidth();
	float temp = 0.0;
	for (unsigned int i = 0; i < heigth; i++)
	{
		for (int j = 0; j < widthMask; j++)
		{
			if (i<=(heigth / 2)&&j<=(widthMask / 4) || i<=(heigth / 2) && j>(widthMask * 3 / 4) || i>(heigth / 2) && j>(widthMask / 4) && j<=(widthMask * 3 / 4))
				temp -= image[numberPix++];
			else temp += image[numberPix++];
		}
		numberPix += width - widthMask;
	}
	return temp;
}
float Filters::filter7(Image& image, unsigned int numberPix, const int& widthMask)
{
	unsigned int heigth = image.putHeight();
	unsigned int width = image.putWidth();
	float temp = 0.0;
	for (unsigned int i = 0; i < heigth; i++)
	{
		for (int j = 0; j < widthMask; j++)
		{
			if (j<=(widthMask / 2) && i>(heigth / 4) && i<=(heigth * 3 / 4) || j>(widthMask / 2) && i<=(heigth / 4) || i>(heigth * 3 / 4) && j>(widthMask / 2))
				temp -= image[numberPix++];
			else temp += image[numberPix++];
		}
		numberPix += width - widthMask;
	}
	return temp;
}
float Filters::filter8(Image& image, unsigned int numberPix, const int& widthMask)
{
	unsigned int heigth = image.putHeight();
	unsigned int width = image.putWidth();
	float temp = 0.0;
	for (unsigned int i = 0; i < heigth; i++)
	{
		for (int j = 0; j < widthMask; j++)
		{
			if (i>(heigth / 2) && j<(widthMask / 2) || i<(heigth / 2) && j>(widthMask / 2))
				if ((i / (heigth / 4) + j / (widthMask / 4)) % 2) temp += image[numberPix++];
				else temp -= image[numberPix++];
			else if ((i / (heigth / 4) + j / (widthMask / 4)) % 2) temp -= image[numberPix++];
				 else temp += image[numberPix++];
		}
		numberPix += width - widthMask;
	}
	return temp;
}
float Filters::filter9(Image& image, unsigned int numberPix, const int& widthMask)
{
	unsigned int heigth = image.putHeight();
	unsigned int width = image.putWidth();
	float temp = 0.0;
	for (unsigned int i = 0; i < heigth; i++)
	{
		for (int j = 0; j < widthMask; j++)
		{
			if ((j>(widthMask / 4) && j<=(widthMask / 2)) || j>=(widthMask * 3 / 4))
				temp -= image[numberPix++];
			else temp += image[numberPix++];
		}
		numberPix += width - widthMask;
	}
	return temp;
}
float Filters::filter10(Image& image, unsigned int numberPix, const int& widthMask)
{
	unsigned int heigth = image.putHeight();
	unsigned int width = image.putWidth();
	float temp = 0.0;
	for (unsigned int i = 0; i < heigth; i++)
	{
		for (int j = 0; j < widthMask; j++)
		{
			if (i<=(heigth / 4) || (i>(heigth / 2) && i<(heigth * 3 / 4)))
				temp -= image[numberPix++];
			else temp += image[numberPix++];
		}
		numberPix += width - widthMask;
	}
	return temp;
}
float Filters::filter11(Image& image, unsigned int numberPix, const int& widthMask)
{
	unsigned int heigth = image.putHeight();
	unsigned int width = image.putWidth();
	float temp = 0.0;
	for (unsigned int i = 0; i < heigth; i++)
	{
		for (int j = 0; j < widthMask; j++)
		{
			if (i<=(heigth / 2))
				if (j>(widthMask / 4) && j<=(widthMask / 2) || j>=(widthMask * 3 / 4))temp -= image[numberPix++];
				else temp += image[numberPix++];
			else if (j<=(widthMask / 4) || j>(widthMask / 2) && j<(widthMask * 3 / 4))temp -= image[numberPix++];
				else temp += image[numberPix++];
		}
		numberPix += width - widthMask;
	}
	return temp;
}
float Filters::filter12(Image& image, unsigned int numberPix, const int& widthMask)
{
	unsigned int heigth = image.putHeight();
	unsigned int width = image.putWidth();
	float temp = 0.0;
	for (unsigned int i = 0; i < heigth; i++)
	{
		for (int j = 0; j < widthMask; j++)
		{
			if (j<(widthMask / 2))
				if ((i>(heigth / 4) && i<=(heigth / 2)) || i>(heigth * 3 / 4))temp -= image[numberPix++];
				else temp += image[numberPix++];
			else if (i<=(heigth / 4) || (i>=(heigth / 2) && i<(heigth * 3 / 4)))temp -= image[numberPix++];
			else temp += image[numberPix++];
		}
		numberPix += width - widthMask;
	}
	return temp;
}
float Filters::filter13(Image& image, unsigned int numberPix, const int& widthMask)
{
	unsigned int heigth = image.putHeight();
	unsigned int width = image.putWidth();
	float temp = 0.0;
	for (unsigned int i = 0; i < heigth; i++)
	{
		for (int j = 0; j < widthMask; j++)
		{
			if (i<(heigth / 2))
				if ((i / (heigth / 4) + j / (widthMask / 4)) % 2)temp += image[numberPix++];
				else temp -= image[numberPix++];
			else if ((i / (heigth / 4) + j / (widthMask / 4)) % 2)temp -= image[numberPix++];
			else temp += image[numberPix++];
		}
		numberPix += width - widthMask;
	}
	return temp;
}
float Filters::filter14(Image& image, unsigned int numberPix, const int& widthMask)
{
	unsigned int heigth = image.putHeight();
	unsigned int width = image.putWidth();
	float temp = 0.0;
	for (unsigned int i = 0; i < heigth; i++)
	{
		for (int j = 0; j < widthMask; j++)
		{
			if (j<(widthMask / 2))
				if ((i / (heigth / 4) + j / (widthMask / 4)) % 2)temp -= image[numberPix++];
				else temp += image[numberPix++];
			else if ((i / (heigth / 4) + j / (widthMask / 4)) % 2)temp += image[numberPix++];
			else temp -= image[numberPix++];
		}
		numberPix += width - widthMask;
	}
	return temp;
}
float Filters::filter15(Image& image, unsigned int numberPix, const int& widthMask)
{
	unsigned int heigth = image.putHeight();
	unsigned int width = image.putWidth();
	float temp = 0.0;
	for (unsigned int i = 0; i < heigth; i++)
	{
		for (int j = 0; j < widthMask; j++)
		{
			if ((i / (heigth / 4) + j / (widthMask / 4)) % 2)temp += image[numberPix++];
			else temp -= image[numberPix++];
		}
		numberPix += width - widthMask;
	}
	return temp;
}