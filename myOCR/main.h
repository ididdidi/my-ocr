#pragma once
#ifndef MAINHEAD
#define MAINHEAD
#define QF 16						// ���������� ��������
class Pixel						// ������ ������� �������
{
private:
	unsigned char brightness;			
public:
	Pixel() : brightness(0)
	{}
	void get(const double& br);			// �������� �������� �������
	unsigned char& put();					// �������� �������� �������
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
	Image() : height(0), width(0), pixel(0) // �����������
	{ }	 
	~Image()
	{
		delete[]pixel;
	}
	unsigned int putHeight()const;
	unsigned int putWidth()const;
	unsigned char& operator [](const unsigned int& numberPix)const;
	float valueF(unsigned int posX, const unsigned int& widthMask);
	int download();
};
class Settings						// ��������� �������� �������������
{
private:
	bool mode;
	unsigned int widthMask;			// ������ ������������� ����� ��� ������ ���������� f1
	unsigned int stepOffset;		// ��� �������� ��� ������ ���������� f1
	unsigned int minInterval;		// ����������� ������ ������� � ��������
	unsigned int maxInterval;		// ������������ ������ ������� � ��������
	float percentOverlay;			// ������� ���������
public:
	Settings(bool m, int wM, int sO, int minIn, int maxIn, float pO)
	{
		mode = m;
		widthMask = wM;			// ������ ������������� ����� ��� ������ ���������� f1
		stepOffset = sO;		// ��� �������� ��� ������ ���������� f1
		minInterval = minIn;		// ����������� ������ ������� � ��������
		maxInterval = maxIn;		// ������������ ������ ������� � ��������
		percentOverlay = pO;
	}
	void getSettings();
	friend class Sample;
	friend class Strainer;
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
	void filtering(const Image& image, const unsigned int& x0, const unsigned int& xEnd);
	void display();
	void diskOut();
	float operator - (const Sample& matchFiltred);
};
class Compliance
{
protected:
	int x0;							// ���������� ���������� �������� �������;
	int xEnd;						// �������� ���������� �������� �������;
	char nearestMatch;				// ����� ������� ���������� �� �������� ���������;
	float CartesianDistance;		// ��������� ���������� �� ���������� �������;
public:
	Compliance() : x0(0), xEnd(0), nearestMatch(0), CartesianDistance(0.0)
	{ }
	Compliance(int X0, int End, int nM, float CD) :
		x0(X0), xEnd(End), nearestMatch(nM), CartesianDistance(CD)
	{ }
	void dispay();
	friend class Strainer;
};
class Strainer : public Sample		// ������� ����������
{
protected:
	list<Compliance> compliance;
public:
	char compareWithBase(float& MinCD);
	void selection(Image& image, Settings& user);
	void display();
};
#endif // !MAINHEAD
