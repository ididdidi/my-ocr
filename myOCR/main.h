#pragma once
#ifndef MAINHEAD
#define MAINHEAD
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
class Filters								// ������ ������� ��� � ���� �������;
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
class Srainer : public Filters				// ��������� ��������� �������� ���;
{
protected:
	int x0, xk;								// ��������� � �������� ��������� �������;
	float rest;								// ������������������ ��������;
};


#endif // !MAINHEAD
