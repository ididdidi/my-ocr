// myOCR.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"


int main()
{
	Image img(2, 2);
	for(int i=0; i<4; i++)
	 img[i] = i+1;
	float  tmp=0.0;
	Filters F;
	tmp = F.filter0(img, 0, 2);
	std::cout << tmp;
	system("pause");
    return 0;
}

