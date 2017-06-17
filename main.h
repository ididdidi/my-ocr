#pragma once
#include "stdafx.h"


#ifndef MAINHEAD
#define MAINHEAD
#define QF 16							// Количество фильтров

enum mode { detection, training };		// режимы работы программы 
										// detection - распознавание, training - тренировка

										// CIEXYZTRIPLE stuff
typedef int FXPT2DOT30;

typedef struct {
	FXPT2DOT30 ciexyzX;
	FXPT2DOT30 ciexyzY;
	FXPT2DOT30 ciexyzZ;
} CIEXYZ;

typedef struct {
	CIEXYZ  ciexyzRed;
	CIEXYZ  ciexyzGreen;
	CIEXYZ  ciexyzBlue;
} CIEXYZTRIPLE;

// bitmap file header
typedef struct {
	unsigned short bfType;
	unsigned int   bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned int   bfOffBits;
} BITMAPFILEHEADER;

// bitmap info header
typedef struct {
	unsigned int   biSize;
	unsigned int   biWidth;
	unsigned int   biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned int   biCompression;
	unsigned int   biSizeImage;
	unsigned int   biXPelsPerMeter;
	unsigned int   biYPelsPerMeter;
	unsigned int   biClrUsed;
	unsigned int   biClrImportant;
	unsigned int   biRedMask;
	unsigned int   biGreenMask;
	unsigned int   biBlueMask;
	unsigned int   biAlphaMask;
	unsigned int   biCSType;
	CIEXYZTRIPLE   biEndpoints;
	unsigned int   biGammaRed;
	unsigned int   biGammaGreen;
	unsigned int   biGammaBlue;
	unsigned int   biIntent;
	unsigned int   biProfileData;
	unsigned int   biProfileSize;
	unsigned int   biReserved;
} BITMAPINFOHEADER;

// read bytes
template <typename Type>
void read(std::ifstream &fp, Type &result, std::size_t size) {
	fp.read(reinterpret_cast<char*>(&result), size);
}

// выбор режима
mode getMode();

// выбор изображения
string getFileName();

// яркость пикселя
class Pixel
{
private:
	unsigned char brightness;
public:
	Pixel() : brightness(0)
	{}
	void get(const double& br);				// получить значение яркости
	unsigned char& put();					// передать значение яркости
};

// изображение в виде массива Pixel;
class Image
{
private:
	Image(const Image&) {}						// Конструктор копирования;
protected:
	Pixel* pixel;								// указатель на массив;
	unsigned int height;						// высота изображение;
	unsigned int width;							// ширина изображения;
	unsigned char bitextract(const unsigned int byte, const unsigned int mask);
public:
	Image(string fileName); 						// конструктор
	~Image()
	{
		delete[]pixel;
	}
	class ImageEx								// класс исключений
	{
	public:
		string origin;							// для имени функции
		string value;							// для хранения ошибочного значения;
		string description;						// описание
		ImageEx(string or , string vl, string desc)
			: origin(or ), value(vl), description(desc)
		{ }
	};
	unsigned int putHeight()const;
	unsigned int putWidth()const;
	unsigned char& operator [](const unsigned int& numberPix)const;
	float valueF(unsigned int posX, const unsigned int& widthMask);
	bool extremum(unsigned int posX, const int& stepOffset, const unsigned int& widthMask);
};

// параметры, вводимые пользователем
class Settings
{
private:
	unsigned int widthMask;			// ширина накладываемой маски при поиске экстреммум f1
	unsigned int stepOffset;		// шаг смещения при поиске экстремум f1
	unsigned int minInterval;		// минимальная ширина символа в пикселях
	unsigned int maxInterval;		// максимальная ширина символа в пикселях
	float percentOverlay;			// допустимый процент наложения гипотез
public:
	Settings(const mode& md, unsigned int width);
	Settings(int wM, int sO, int minIn, int maxIn, float pO)
		: widthMask(wM), stepOffset(sO), minInterval(minIn), maxInterval(maxIn),
		percentOverlay(pO)
	{ }
	friend class Image;
	friend class Sample;
	friend class Strainer;
};
// находит, хранит, и записывает эталоны;
class Sample
{
protected:
	char match;								// номер эталона
	float Filtered[QF];						// результат наложения фильтров;
public:
	Sample()
	{
		for (int i = 0; i < QF; i++)Filtered[i] = 0.0; // обнуляем массив при создании
	}
	class SampleEx								// класс исключений
	{
	public:
		string origin;							// для имени функции
		string value;							// для хранения ошибочного значения;
		string description;						// описание
		SampleEx(string or , string vl, string desc)
			: origin(or ), value(vl), description(desc)
		{ }
	};
	char returnMatch();						// вернуть номер эталона(символа)
	void filtering(const Image& image, const unsigned int& x0,
		const unsigned int& xEnd);		// наложение фильтров;
	void diskOut();									// запись значений в файл
	char compareWithBase(float& MinCD);				// сравниваем с эталонами
	float operator - (const Sample& matchFiltred);
	void training(Image& image, Settings& user);	// добавление эталонов
};

// данные о соответствии гипотез эталонам
class Compliance
{
protected:
	int x0;							// Начальная координата искомого объекта;
	int xEnd;						// конечная координата искомого объекта;
	char nearestMatch;				// номер эталона ближайшего по значению признаков;
	float EuclideanMetric;		// расстояние до ближайшего эталона;
public:
	Compliance() : x0(0), xEnd(0), nearestMatch(0), EuclideanMetric(0.0)
	{ }
	Compliance(int X0, int End, int nM, float CD) :
		x0(X0), xEnd(End), nearestMatch(nM), EuclideanMetric(CD)
	{ }
	bool operator < (const Compliance& rhs)	// для сортировки
	{
		return x0 < rhs.x0;
	}
	void dispay();					// вывод на экран
	friend class Strainer;
};

// выборка совпадений
class Strainer
{
protected:
	list<Compliance> compliance;
public:
	Strainer(Image& image, Settings& user)
	{
		selection(image, user);			// ищем совпадения с эталонами
	}
	void selection(Image& image, Settings& user);	// поиск совпадений
	void minimize(Settings& user);		// удаление лишних совпадений
	void display();						// вывод результата  поиска совпадений
};
#endif // !MAINHEAD