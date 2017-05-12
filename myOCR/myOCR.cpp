// myOCR.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"


int main()
{
	setlocale(LC_ALL, "Russian");
	Settings user;
	unsigned int x0 = 0, xk = 4;
	Image img(4, 4);
	for(int i=0; i<16; i++)
	 img[i] = 1;
	Strainer S;
	S.filtering(img, x0, xk);
	S.diskOut();

	system("pause");
    return 0;
}

