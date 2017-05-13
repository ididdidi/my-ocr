// myOCR.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"


int main()
{
	setlocale(LC_ALL, "Russian");
	Settings user(false,4,1,8,35,10);
	Image img;
	img.download();
	Strainer S;
	S.selection(img, user);
	S.display();
	system("pause");
    return 0;
}

