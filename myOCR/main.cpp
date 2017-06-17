// myOCR.cpp: определяет точку входа для консольного приложения.

#include "stdafx.h"

int main()
{
	mode operationMode = getMode();
	char renew;
	do {
		try {
			Image img(getFileName());
			Settings user(operationMode, img.putWidth());
			if (operationMode) {
				Sample smpl;
				smpl.training(img, user);
			}
			else {
				Strainer S(img, user);
				S.minimize(user);
				S.display();
			}
		}
		catch (Image::ImageEx ix) {
			cout << "Error in the process \"" << ix.origin << "\": the \"";
			cout << ix.value << '\"' << ix.description << endl;
		}
		catch (Sample::SampleEx sx) {
			cout << "Error in the process \"" << sx.origin << "\": the \"";
			cout << sx.value << '\"' << sx.description << endl;
		}

		cout << " Want to resume(y/n)? ";
		cin >> renew;
	} while (renew == 'y'|| renew == 'Y');
	
    return 0;
}

