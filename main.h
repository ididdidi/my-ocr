#pragma once
#include "stdafx.h"


#ifndef MAINHEAD
#define MAINHEAD
#define QF 16							// ���������� ��������

enum mode { detection, training };		// ������ ������ ��������� 
										// detection - �������������, training - ����������

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

// ����� ������
mode getMode();

// ����� �����������
string getFileName();

// ������� �������
class Pixel
{
private:
	unsigned char brightness;
public:
	Pixel() : brightness(0)
	{}
	void get(const double& br);				// �������� �������� �������
	unsigned char& put();					// �������� �������� �������
};

// ����������� � ���� ������� Pixel;
class Image
{
private:
	Image(const Image&) {}						// ����������� �����������;
protected:
	Pixel* pixel;								// ��������� �� ������;
	unsigned int height;						// ������ �����������;
	unsigned int width;							// ������ �����������;
	unsigned char bitextract(const unsigned int byte, const unsigned int mask);
public:
	Image(string fileName); 						// �����������
	~Image()
	{
		delete[]pixel;
	}
	class ImageEx								// ����� ����������
	{
	public:
		string origin;							// ��� ����� �������
		string value;							// ��� �������� ���������� ��������;
		string description;						// ��������
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

// ���������, �������� �������������
class Settings
{
private:
	unsigned int widthMask;			// ������ ������������� ����� ��� ������ ���������� f1
	unsigned int stepOffset;		// ��� �������� ��� ������ ��������� f1
	unsigned int minInterval;		// ����������� ������ ������� � ��������
	unsigned int maxInterval;		// ������������ ������ ������� � ��������
	float percentOverlay;			// ���������� ������� ��������� �������
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
// �������, ������, � ���������� �������;
class Sample
{
protected:
	char match;								// ����� �������
	float Filtered[QF];						// ��������� ��������� ��������;
public:
	Sample()
	{
		for (int i = 0; i < QF; i++)Filtered[i] = 0.0; // �������� ������ ��� ��������
	}
	class SampleEx								// ����� ����������
	{
	public:
		string origin;							// ��� ����� �������
		string value;							// ��� �������� ���������� ��������;
		string description;						// ��������
		SampleEx(string or , string vl, string desc)
			: origin(or ), value(vl), description(desc)
		{ }
	};
	char returnMatch();						// ������� ����� �������(�������)
	void filtering(const Image& image, const unsigned int& x0,
		const unsigned int& xEnd);		// ��������� ��������;
	void diskOut();									// ������ �������� � ����
	char compareWithBase(float& MinCD);				// ���������� � ���������
	float operator - (const Sample& matchFiltred);
	void training(Image& image, Settings& user);	// ���������� ��������
};

// ������ � ������������ ������� ��������
class Compliance
{
protected:
	int x0;							// ��������� ���������� �������� �������;
	int xEnd;						// �������� ���������� �������� �������;
	char nearestMatch;				// ����� ������� ���������� �� �������� ���������;
	float EuclideanMetric;		// ���������� �� ���������� �������;
public:
	Compliance() : x0(0), xEnd(0), nearestMatch(0), EuclideanMetric(0.0)
	{ }
	Compliance(int X0, int End, int nM, float CD) :
		x0(X0), xEnd(End), nearestMatch(nM), EuclideanMetric(CD)
	{ }
	bool operator < (const Compliance& rhs)	// ��� ����������
	{
		return x0 < rhs.x0;
	}
	void dispay();					// ����� �� �����
	friend class Strainer;
};

// ������� ����������
class Strainer
{
protected:
	list<Compliance> compliance;
public:
	Strainer(Image& image, Settings& user)
	{
		selection(image, user);			// ���� ���������� � ���������
	}
	void selection(Image& image, Settings& user);	// ����� ����������
	void minimize(Settings& user);		// �������� ������ ����������
	void display();						// ����� ����������  ������ ����������
};
#endif // !MAINHEAD