// myOCR.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"


int main()
{
	setlocale(LC_ALL, "Russian");
	enum mode { detection, training };			// detection - распознавание, training 
	Settings user(4,1,10,25,10,"01.bmp");
	Image img(user);
	if (detection){
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

