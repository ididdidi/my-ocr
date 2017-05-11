// myOCR.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"


int main()
{
	Image img(4, 4);
	for(int i=0; i<16; i++)
	 img[i] = 1;
	float  tmp=0.0;
	Filters F;
	
	tmp = F.filter12(img, 0, 4);
	std::cout << tmp;
	system("pause");
    return 0;
}

