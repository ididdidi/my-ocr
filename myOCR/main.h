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
	float extremum(unsigned int posX, const unsigned int& widthMask);
};
class Settings					// ��������� �������� �������������
{
public:
	unsigned int widthMask;		// ������ ������������� ����� ��� ������ ���������� f1
	unsigned int stepOffset;	// ��� �������� ��� ������ ���������� f1
	unsigned int minInterval;	// ����������� ������ �������
	unsigned int maxInterval;	// ������������ ������ �������
	float percentOverlay;		// ������� ��������� 
};
class Strainer								// ��������� ��������� �������� ���;
{
protected:
	float Filtered[QF];						// ������������������ ��������;
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
class Sample : public Strainer		// ������� ����������
{
protected:
	list<Compliance> compliance;
public:
	void selection(Image& image, Settings& user);
};
#endif // !MAINHEAD
