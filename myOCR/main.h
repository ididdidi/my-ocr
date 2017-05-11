#pragma once
#ifndef MAINHEAD
#define MAINHEAD
#define QF 16						// Количество филььров
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
class Strainer								// реализует наложение фильтров ТАВ;
{
protected:
//	int x0, xk;								// начальное и конечное положение фильтра;
	float Filtered[QF];						// результатналожения фильтров;
public:
	Strainer() //: x0(0), xk(0)
	{
		for (int i = 0; i < QF; i++)Filtered[i] = 0.0;
	}
	Strainer(float M[QF])
	{
		for (int i = 0; i < QF; i++)Filtered[i] = M[i];
	}
	void filtering(Image& image, const unsigned int& x0, const unsigned int& xk);
	void display();
	
};


#endif // !MAINHEAD
