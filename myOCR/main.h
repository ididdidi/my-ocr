#pragma once
#ifndef MAINHEAD
#define MAINHEAD
class Pixel						// хранит яркость пикселя
{
private:
	char brightness;			
public:
	Pixel() : brightness(0)
	{}
	void get(const char& br);			// получить значение яркости
	char& put();					// передать значение яркости
};
class Image						// хранит изображение в виде массива Pixel;
{
private:
	Image(const Image&)	{}					// Конструктор копирования;
protected:
	Pixel* pixel;								// указатель на массив;
	unsigned int height;						// высота изображение;
	unsigned int width;							// ширина изображения;
public:
	Image(const int& h, const int& w) : height(h), width(w) // конструктор
	{
		pixel = new Pixel[h*w];					// размер ассива определяется размером изображения
	}	 
	~Image()
	{
		delete[]pixel;
	}
	unsigned int putHeight();
	unsigned int putWidth();
	void getPixel(const unsigned int& numberPix, const char& br);
	char putPixel(const unsigned int& numberPix);
	char& operator [](const unsigned int& numberPix);
};
class Filters								// хранит фильтры ТАБ в виде методов;
{
public:
	float filter0(Image& image, unsigned int numberPixel, const int& widthMas);
	float filter1(Image& image, unsigned int numberPixel, const int& widthMas);
	float filter2(Image& image, unsigned int numberPixel, const int& widthMas);
	float filter3(Image& image, unsigned int numberPixel, const int& widthMas);
	float filter4(Image& image, unsigned int numberPixel, const int& widthMas);
	float filter5(Image& image, unsigned int numberPixel, const int& widthMas);
	float filter6(Image& image, unsigned int numberPixel, const int& widthMas);
	float filter7(Image& image, unsigned int numberPixel, const int& widthMas);
	float filter8(Image& image, unsigned int numberPixel, const int& widthMas);
	float filter9(Image& image, unsigned int numberPixel, const int& widthMas);
	float filter10(Image& image, unsigned int numberPixel, const int& widthMas);
	float filter11(Image& image, unsigned int numberPixel, const int& widthMas);
	float filter12(Image& image, unsigned int numberPixel, const int& widthMas);
	float filter13(Image& image, unsigned int numberPixel, const int& widthMas);
	float filter14(Image& image, unsigned int numberPixel, const int& widthMas);
	float filter15(Image& image, unsigned int numberPixel, const int& widthMas);
};
class Srainer : public Filters				// реализует наложение фильтров ТАВ;
{
protected:
	int x0, xk;								// начальное и конечное положение фильтра;
	float rest;								// результатналожения фильтров;
};


#endif // !MAINHEAD
