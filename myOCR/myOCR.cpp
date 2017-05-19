// myOCR.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"


int main()
{
	setlocale(LC_ALL, "Russian");
#ifdef _OPENMP
	printf("OpenMP is supported!\n");
#endif
	// ����������� ���������� ����� ������� � ����� ��������� �����������
	{
		printf("����������� ���������� ����� �������: %d\n", omp_get_max_threads());
		printf("����� ��������� �����������: %d\n", omp_get_num_procs());
	}
	double start_time, end_time, tick;
	start_time = omp_get_wtime();
	tick = omp_get_wtick();
	Settings user(false,4,1,8,35,10,"01.bmp");
	Image img(user);
	Strainer S(img, user);
	S.display();
	end_time = omp_get_wtime();
	cout << endl << "����� ���������� ���� ���������: " << end_time - start_time << endl;
	system("pause");
    return 0;
}

