// myOCR.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"


int main()
{
	Settings user(4,1,10,25,10,"07.bmp");
	try{
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
	}
	catch (Image::ImageEx ix) {
		cout << "Error in the process " << ix.origin <<": the ";
		cout << ix.value << " is not available!" << endl;
	}

	system("pause");
    return 0;
}

