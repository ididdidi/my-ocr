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
	float valueF(unsigned int posX, const unsigned int& widthMask);
};
class Settings						// параметры вводимые пользователем
{
public:
	unsigned int widthMask = 4;		// ширина накладываемой маски при поиске экстреммум f1
	unsigned int stepOffset = 1;	// шаг смещения при поиске экстреммум f1
	unsigned int minInterval = 3;	// минимальная ширина символа в пикселях
	unsigned int maxInterval = 5;	// максимальная ширина символа в пикселях
	float percentOverlay = 100;		// процент наложения 
};
class Sample								// находит хранит, и записывает эталоны;
{
protected:
	char match;								// номер эталона
	float Filtered[QF];						// результатналожения фильтров;
public:
	Sample()
	{
		for (int i = 0; i < QF; i++)Filtered[i] = 0.0;
	}
	Sample(float M[QF])
	{
		for (int i = 0; i < QF; i++)Filtered[i] = M[i];
	}
	char returnMatch();
	void filtering(Image& image, const unsigned int& x0, const unsigned int& xEnd);
	void display();
	void diskOut();
	float operator - (Sample& matchFiltred);
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
class Strainer : public Sample		// выборка совпадений
{
protected:
	list<Compliance> compliance;
public:
	char compareWithBase(float& MinCD);
	void selection(Image& image, Settings& user);
};
#endif // !MAINHEAD
