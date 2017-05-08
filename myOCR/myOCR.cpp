// myOCR.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <stdio.h>
#include <cstdlib>


int main()
{
#ifdef _OPENMP
	printf("OpenMP is supported!\n");
#endif
	system("pause");
    return 0;
}

