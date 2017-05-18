// myOCR.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"


int main()
{

#ifdef _OPENMP
	printf("OpenMP is supported!\n");
#endif
	double start_time, end_time, tick;
	start_time = omp_get_wtime();
	tick = omp_get_wtick();
	setlocale(LC_ALL, "Russian");
	Settings user(false,4,1,8,35,10,"01.bmp");
	Image img(user);
	Strainer S(img, user);
	S.display();
	end_time = omp_get_wtime();
	cout << endl << "Время выполнения всей программы: " << end_time - start_time << endl;
	system("pause");
    return 0;
}

