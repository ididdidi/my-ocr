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

//	методы класса Strainer
void Strainer::filtering(Image& image, const unsigned int& x0, const unsigned int& xk)
{
	unsigned int numberPix = x0;				// начальная координата наложения фильтра
	unsigned int widthMask = (xk - x0) / 4;		// масштабируем фильтр по размеру отрезка
	widthMask *= 4;								
	unsigned int heigth = image.putHeight();	// высота изображения;
	unsigned int width = image.putWidth();		// ширина изображения;
	
	// Последовательно пропускаем пиксели через каждый фильтир 
	for (unsigned int i = 0; i < heigth; i++)	// внешний цикл наложения фильтров
	{
		for (int j = 0; j < widthMask; j++, numberPix++) // внутренний цикл наложения фильтров
		{
			// Накладываем фильтр 0
			{	
				Filtered[0] = image[numberPix]; 
			}
			// Накладываем фильтр 1										
			{
				if (j < (width / 2)) Filtered[1] -= image[numberPix];
				else Filtered[1] += image[numberPix];
			}	
		    // Накладываем фильтр 2
			{
				if (i < (heigth / 2)) Filtered[2] -= image[numberPix];
				else Filtered[2] += image[numberPix];
			}
			// Накладываем фильтр 3
			{
				if ((2 * i / heigth + 2 * j / widthMask) % 2) Filtered[3] += image[numberPix];
				else Filtered[3] -= image[numberPix];
			}
			// Накладываем фильтр 4
			{
				if (j <= (widthMask / 4) || j > (widthMask * 3 / 4)) Filtered[4] -= image[numberPix];
				else Filtered[4] += image[numberPix];
			}
			// Накладываем фильтр 5
			{
				if (j <= (heigth / 4) || j > (heigth * 3 / 4)) Filtered[5] -= image[numberPix];
				else Filtered[5] += image[numberPix];
			}
			// Накладываем фильтр 6
			{
				if (i <= (heigth / 2) && j <= (widthMask / 4) || i <= (heigth / 2) && j > (widthMask * 3 / 4) || i > (heigth / 2) && j > (widthMask / 4) && j <= (widthMask * 3 / 4))
					Filtered[6] -= image[numberPix];
				else Filtered[6] += image[numberPix];
			}
			// Накладываем фильтр 7
			{
				if (j <= (widthMask / 2) && i > (heigth / 4) && i <= (heigth * 3 / 4) || j > (widthMask / 2) && i <= (heigth / 4) || i > (heigth * 3 / 4) && j > (widthMask / 2))
					Filtered[7] -= image[numberPix];
				else Filtered[7] += image[numberPix];
			}
			// Накладываем фильтр 8
			{
				if (i > (heigth / 2) && j < (widthMask / 2) || i<(heigth / 2) && j>(widthMask / 2))
					if ((i / (heigth / 4) + j / (widthMask / 4)) % 2) Filtered[8] += image[numberPix];
					else Filtered[8] -= image[numberPix];
				else if ((i / (heigth / 4) + j / (widthMask / 4)) % 2) Filtered[8] -= image[numberPix];
				else Filtered[8] += image[numberPix];
			}
			// Накладываем фильтр 9
			{
				if ((j > (widthMask / 4) && j <= (widthMask / 2)) || j >= (widthMask * 3 / 4))
					Filtered[9] -= image[numberPix];
				else Filtered[9] += image[numberPix];
			}
			// Накладываем фильтр 10
			{
				if (i <= (heigth / 4) || (i > (heigth / 2) && i < (heigth * 3 / 4)))
					Filtered[10] -= image[numberPix];
				else Filtered[10] += image[numberPix];
			}
			// Накладываем фильтр 11
			{
				if (i <= (heigth / 2))
					if (j>(widthMask / 4) && j <= (widthMask / 2) || j >= (widthMask * 3 / 4))Filtered[11] -= image[numberPix];
					else Filtered[11] += image[numberPix];
				else if (j <= (widthMask / 4) || j>(widthMask / 2) && j<(widthMask * 3 / 4))Filtered[11] -= image[numberPix];
				else Filtered[11] += image[numberPix];
			}
			// Накладываем фильтр 12
			{
				if (j<(widthMask / 2))
					if ((i>(heigth / 4) && i <= (heigth / 2)) || i>(heigth * 3 / 4))Filtered[12] -= image[numberPix];
					else Filtered[12] += image[numberPix];
				else if (i <= (heigth / 4) || (i >= (heigth / 2) && i<(heigth * 3 / 4)))Filtered[12] -= image[numberPix];
				else Filtered[12] += image[numberPix];
			}
			// Накладываем фильтр 13
			{
				if (i<(heigth / 2))
					if ((i / (heigth / 4) + j / (widthMask / 4)) % 2)Filtered[13] += image[numberPix];
					else Filtered[13] -= image[numberPix];
				else if ((i / (heigth / 4) + j / (widthMask / 4)) % 2)Filtered[13] -= image[numberPix];
				else Filtered[13] += image[numberPix];
			}
			// Накладываем фильтр 14
			{
				if (j<(widthMask / 2))
					if ((i / (heigth / 4) + j / (widthMask / 4)) % 2)Filtered[14] -= image[numberPix];
					else Filtered[14] += image[numberPix];
				else if ((i / (heigth / 4) + j / (widthMask / 4)) % 2)Filtered[14] += image[numberPix];
				else Filtered[14] -= image[numberPix];
			}
			// Накладываем фильтр 15
			{
				if ((i / (heigth / 4) + j / (widthMask / 4)) % 2)Filtered[15] += image[numberPix];
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