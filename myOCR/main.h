#pragma once
#ifndef MAINHEAD
#define MAINHEAD
class Pixel						// хранит €ркость пиксел€
{
protected:
	char brightness;			
public:
	Pixel() : brightness(0)
	{}
	void get(char br)			// получить значение €ркости
	{
		brightness = br;
	}
	char put()					// передать значение €ркости
	{
		return brightness;
	}
};
class Image										// хранит изображение в виде массива Pixel;
{
protected:
	Pixel* pixel;								// указатель на массив;
	unsigned int height;						// высота изображение;
	unsigned int width;							// ширина изображени€;
public:
	Image(int h, int w) : height(h), width(w)
	{
		pixel = new Pixel[h*w];
	}
	~Image()
	{
		delete[]pixel;
	}
	 

};
class Filters								// хранит фильтры “јЅ в виде методов;
{
protected:
	Image* image;
	float filter0();
};
class Srainer : public Filters				// реализует наложение фильтров “ј¬;
{
protected:
	int x0, xk;								// начальное и конечное положение фильтра;
	float rest;								// результатналожени€ фильтров;
};
float Filters::filter0()
{

}

#endif // !MAINHEAD
