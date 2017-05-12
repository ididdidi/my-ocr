#pragma once
#ifndef MAINHEAD
#define MAINHEAD
#define QF 16						// ���������� ��������
class Pixel						// ������ ������� �������
{
private:
	char brightness;			
public:
	Pixel() : brightness(0)
	{}
	void get(const char& br);			// �������� �������� �������
	char& put();					// �������� �������� �������
};
class Image						// ������ ����������� � ���� ������� Pixel;
{
private:
	Image(const Image&)	{}					// ����������� �����������;
protected:
	Pixel* pixel;								// ��������� �� ������;
	unsigned int height;						// ������ �����������;
	unsigned int width;							// ������ �����������;
public:
	Image(const int& h, const int& w) : height(h), width(w) // �����������
	{
		pixel = new Pixel[h*w];					// ������ ������ ������������ �������� �����������
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
class Settings						// ��������� �������� �������������
{
public:
	unsigned int widthMask = 4;		// ������ ������������� ����� ��� ������ ���������� f1
	unsigned int stepOffset = 1;	// ��� �������� ��� ������ ���������� f1
	unsigned int minInterval = 3;	// ����������� ������ ������� � ��������
	unsigned int maxInterval = 5;	// ������������ ������ ������� � ��������
	float percentOverlay = 100;		// ������� ��������� 
};
class Sample								// ������� ������, � ���������� �������;
{
protected:
	char match;								// ����� �������
	float Filtered[QF];						// ������������������ ��������;
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
	int x0;							// ���������� ���������� �������� �������;
	int xEnd;						// �������� ���������� �������� �������;
	int nearestMatch;				// ����� ������� ���������� �� �������� ���������;
	float CartesianDistance;		// ��������� ���������� �� ���������� �������;
public:
	Compliance() : x0(0), xEnd(0), nearestMatch(0), CartesianDistance(0.0)
	{ }
	Compliance(int X0, int End, int nM, float CD) :
		x0(X0), xEnd(End), nearestMatch(nM), CartesianDistance(CD)
	{ }
};
class Strainer : public Sample		// ������� ����������
{
protected:
	list<Compliance> compliance;
public:
	char compareWithBase(float& MinCD);
	void selection(Image& image, Settings& user);
};
#endif // !MAINHEAD
