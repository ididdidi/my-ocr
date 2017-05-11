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
	float extremum(unsigned int posX, const unsigned int& widthMask);
};
class Settings					// параметры вводимые пользователем
{
public:
	unsigned int widthMask;		// ширина накладываемой маски при поиске экстреммум f1
	unsigned int stepOffset;	// шаг смещения при поиске экстреммум f1
	unsigned int minInterval;	// минимальная ширина символа
	unsigned int maxInterval;	// максимальная ширина символа
	float percentOverlay;		// процент наложения 
};
class Strainer								// реализует наложение фильтров ТАВ;
{
protected:
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
	void filtering(Image& image, const unsigned int& x0, const unsigned int& xEnd);
	void display();
	
};
class Compliance
{
protected:
	int x0;							// Начаольная координата искомого объекта;
	int xEnd;						// конечная координата искомого объекта;
	int nearestMatch;				// номер эталона ближайшего по значению признаков;
	float CartesianDistance;		// декартово расстояние до ближайшего эталона;
public:
	Compliance() : x0(0), xEnd(0), nearestMatch(0), CartesianDistance(0.0)
	{ }
	Compliance(int X0, int End, int nM, float CD) :
		x0(X0), xEnd(End), nearestMatch(nM), CartesianDistance(CD)
	{ }
};
class Sample : public Strainer		// выборка совпадений
{
protected:
	list<Compliance> compliance;
public:
	void selection(Image& image, Settings& user);
};
#endif // !MAINHEAD
