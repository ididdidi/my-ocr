#pragma once
#ifndef MAINHEAD
#define MAINHEAD
class Pixel						// ������ ������� �������
{
protected:
	char brightness;			
public:
	Pixel() : brightness(0)
	{}
	void get(char br)			// �������� �������� �������
	{
		brightness = br;
	}
	char put()					// �������� �������� �������
	{
		return brightness;
	}
};
class Image										// ������ ����������� � ���� ������� Pixel;
{
protected:
	Pixel* pixel;								// ��������� �� ������;
	unsigned int height;						// ������ �����������;
	unsigned int width;							// ������ �����������;
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
class Filters								// ������ ������� ��� � ���� �������;
{
protected:
	Image* image;
	float filter0();
};
class Srainer : public Filters				// ��������� ��������� �������� ���;
{
protected:
	int x0, xk;								// ��������� � �������� ��������� �������;
	float rest;								// ������������������ ��������;
};
float Filters::filter0()
{

}

#endif // !MAINHEAD
