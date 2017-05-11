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
};
class Strainer								// ��������� ��������� �������� ���;
{
protected:
//	int x0, xk;								// ��������� � �������� ��������� �������;
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
	void filtering(Image& image, const unsigned int& x0, const unsigned int& xk);
	void display();
	
};


#endif // !MAINHEAD
