// myOCR.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"


int main()
{
	setlocale(LC_ALL, "Russian");
	bool mode = true;
	Settings user(4,1,8,35,10,"01.bmp");
	Image img(user);
	if (mode){
		Sample smpl;
		smpl.training(img, user);
	}
	else{
		Strainer S(img, user);
		S.minimize(user);
		S.display();
	}
	
	system("pause");
    return 0;
}

