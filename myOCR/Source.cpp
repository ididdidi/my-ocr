#include "stdafx.h"

// выбор режима
mode getMode()
{
	char ch=0;
	while (true)
	{
		cout << " Select mode: the detection(d) or training(t)...";
		ch = _getch();
		if (ch == 'd' || ch == 'D'){
			cout << " detection...";
			return detection;
		}
		else if (ch == 't' || ch == 'T'){
				cout << " training...";
				return training;
			 }
		cout << ch << " - not the right request" << endl;
	}
}

// выбор изображения
string getFileName()
{
	string temp;
	cout << "\n Please enter the name of the image... ";
	cin >> temp;
	return temp;
}

						/* методы класса Pixel */

// получить значение яркости пикселя
void Pixel::get(const double& br)			// получить значение яркости
{
	brightness = static_cast<char>(br);
}

// передать значение яркости пикселя
unsigned char& Pixel::put()					
{
	return brightness;
}
  
						/* методы класса Image */

// конструктор = загрузчик информации с изображения
Image::Image(string fileName)			// получает значения яркости из файла
{
	
	// открываем файл
	std::ifstream fileStream(fileName, std::ifstream::binary);
	if (!fileStream) {
		throw ImageEx("download image", fileName, " is not available!");
	}

	// заголовк изображения
	BITMAPFILEHEADER fileHeader;
	read(fileStream, fileHeader.bfType, sizeof(fileHeader.bfType));
	read(fileStream, fileHeader.bfSize, sizeof(fileHeader.bfSize));
	read(fileStream, fileHeader.bfReserved1, sizeof(fileHeader.bfReserved1));
	read(fileStream, fileHeader.bfReserved2, sizeof(fileHeader.bfReserved2));
	read(fileStream, fileHeader.bfOffBits, sizeof(fileHeader.bfOffBits));

	if (fileHeader.bfType != 0x4D42) {
		throw ImageEx("download image", fileName, " is not BMP file."); 
	}

	// информация изображения
	BITMAPINFOHEADER fileInfoHeader;
	read(fileStream, fileInfoHeader.biSize, sizeof(fileInfoHeader.biSize));

	// bmp core
	if (fileInfoHeader.biSize >= 12) {
		read(fileStream, fileInfoHeader.biWidth, sizeof(fileInfoHeader.biWidth));
		read(fileStream, fileInfoHeader.biHeight, sizeof(fileInfoHeader.biHeight));
		read(fileStream, fileInfoHeader.biPlanes, sizeof(fileInfoHeader.biPlanes));
		read(fileStream, fileInfoHeader.biBitCount, sizeof(fileInfoHeader.biBitCount));
	}

	// получаем информацию о битности
	int colorsCount = fileInfoHeader.biBitCount >> 3;
	if (colorsCount < 3) {
		colorsCount = 3;
	}

	int bitsOnColor = fileInfoHeader.biBitCount / colorsCount;
	int maskValue = (1 << bitsOnColor) - 1;

	// bmp v1
	if (fileInfoHeader.biSize >= 40) {
		read(fileStream, fileInfoHeader.biCompression, sizeof(fileInfoHeader.biCompression));
		read(fileStream, fileInfoHeader.biSizeImage, sizeof(fileInfoHeader.biSizeImage));
		read(fileStream, fileInfoHeader.biXPelsPerMeter, sizeof(fileInfoHeader.biXPelsPerMeter));
		read(fileStream, fileInfoHeader.biYPelsPerMeter, sizeof(fileInfoHeader.biYPelsPerMeter));
		read(fileStream, fileInfoHeader.biClrUsed, sizeof(fileInfoHeader.biClrUsed));
		read(fileStream, fileInfoHeader.biClrImportant, sizeof(fileInfoHeader.biClrImportant));
	}

	// bmp v2
	fileInfoHeader.biRedMask = 0;
	fileInfoHeader.biGreenMask = 0;
	fileInfoHeader.biBlueMask = 0;

	if (fileInfoHeader.biSize >= 52) {
		read(fileStream, fileInfoHeader.biRedMask, sizeof(fileInfoHeader.biRedMask));
		read(fileStream, fileInfoHeader.biGreenMask, sizeof(fileInfoHeader.biGreenMask));
		read(fileStream, fileInfoHeader.biBlueMask, sizeof(fileInfoHeader.biBlueMask));
	}

	// если маска не задана, то ставим маску по умолчанию
	if (fileInfoHeader.biRedMask == 0 || fileInfoHeader.biGreenMask == 0 || fileInfoHeader.biBlueMask == 0) {
		fileInfoHeader.biRedMask = maskValue << (bitsOnColor * 2);
		fileInfoHeader.biGreenMask = maskValue << bitsOnColor;
		fileInfoHeader.biBlueMask = maskValue;
	}

	// bmp v3
	if (fileInfoHeader.biSize >= 56) {
		read(fileStream, fileInfoHeader.biAlphaMask, sizeof(fileInfoHeader.biAlphaMask));
	}
	else {
		fileInfoHeader.biAlphaMask = maskValue << (bitsOnColor * 3);
	}

	// bmp v4
	if (fileInfoHeader.biSize >= 108) {
		read(fileStream, fileInfoHeader.biCSType, sizeof(fileInfoHeader.biCSType));
		read(fileStream, fileInfoHeader.biEndpoints, sizeof(fileInfoHeader.biEndpoints));
		read(fileStream, fileInfoHeader.biGammaRed, sizeof(fileInfoHeader.biGammaRed));
		read(fileStream, fileInfoHeader.biGammaGreen, sizeof(fileInfoHeader.biGammaGreen));
		read(fileStream, fileInfoHeader.biGammaBlue, sizeof(fileInfoHeader.biGammaBlue));
	}

	// bmp v5
	if (fileInfoHeader.biSize >= 124) {
		read(fileStream, fileInfoHeader.biIntent, sizeof(fileInfoHeader.biIntent));
		read(fileStream, fileInfoHeader.biProfileData, sizeof(fileInfoHeader.biProfileData));
		read(fileStream, fileInfoHeader.biProfileSize, sizeof(fileInfoHeader.biProfileSize));
		read(fileStream, fileInfoHeader.biReserved, sizeof(fileInfoHeader.biReserved));
	}

	// проверка на поддерку этой версии формата
	if (fileInfoHeader.biSize != 12 && fileInfoHeader.biSize != 40 && fileInfoHeader.biSize != 52 &&
		fileInfoHeader.biSize != 56 && fileInfoHeader.biSize != 108 && fileInfoHeader.biSize != 124) {
		throw ImageEx("download image", fileName, " unsupported BMP format.");
	}

	if (fileInfoHeader.biBitCount != 16 && fileInfoHeader.biBitCount != 24 && fileInfoHeader.biBitCount != 32) {
		throw ImageEx("download image", fileName, " unsupported BMP bit count.");
	}

	if (fileInfoHeader.biCompression != 0 && fileInfoHeader.biCompression != 3) {
		throw ImageEx("download image", fileName, " unsupported BMP compression.");
	}

	// сохранение данных в классе Image
	unsigned int bufer;

	height = fileInfoHeader.biHeight;
	width = fileInfoHeader.biWidth;

	// определение размера отступа в конце каждой строки
	int linePadding = ((fileInfoHeader.biWidth * (fileInfoHeader.biBitCount / 8)) % 4) & 3;


	if (!(height*width)) {
		throw ImageEx("download image", fileName, " less than one pixel.");
	}	
		// создаём динамический массив для хранения значений ярокости	
	if (pixel)
	{
		delete[]pixel;
	}
	pixel = new Pixel[height*width];
	int i = 0;
	unsigned int j = 0;

	// вычисляем значения яркости пикселей и сохраняем их в динамический массив
#pragma omp parallel num_threads(omp_get_max_threads()/2)
	{
#pragma omp for schedule (guided) firstprivate(j) lastprivate(i) ordered
		for (i = 0; i < fileInfoHeader.biHeight; i++) {
#pragma omp ordered
			for (j = 0; j < fileInfoHeader.biWidth; j++) {
				read(fileStream, bufer, fileInfoHeader.biBitCount / 8);
				pixel[j + (fileInfoHeader.biWidth - 10)*i + 10 * i].get(0.3* bitextract(bufer, fileInfoHeader.biRedMask) + 0.59*bitextract(bufer, fileInfoHeader.biGreenMask) + 0.11*bitextract(bufer, fileInfoHeader.biBlueMask));
			}
			fileStream.seekg(linePadding, std::ios_base::cur);
		}
	}
	return;
}

unsigned char Image::bitextract(const unsigned int byte, const unsigned int mask) {
	if (mask == 0) {
		return 0;
	}

	// определение количества нулевых бит справа от маски
	int
		maskBufer = mask,
		maskPadding = 0;

	while (!(maskBufer & 1)) {
		maskBufer >>= 1;
		maskPadding++;
	}

	// применение маски и смещение
	return (byte & mask) >> maskPadding;
}

// возвращает значение высоты в пикселях
unsigned int Image::putHeight()const
{
	return height;
}

// возвращает значение ширины в пикселях
unsigned int Image::putWidth()const
{
	return width;
}

// возвращает значение полученное от наложения фильтра F1
float Image::valueF(unsigned int posX, const unsigned int& widthMask)
{
	float temp = 0.0;
	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < widthMask; j++, posX++)
		{
			if (j < (widthMask / 2)) temp -= pixel[posX].put();
			else temp += pixel[posX].put();
		}
		posX += width - widthMask;
	}
	return temp;
}

// возвращает значение яркости пикселя хранимое в массиве
unsigned char& Image::operator[](const unsigned int& numberPix)const
{
	if (numberPix > height*width){
		throw ImageEx("operator[]", " method", " left was out of bounds of the array");
	}

	return pixel[numberPix].put();
}

// находит экстремумы графика полученного наложением фильтра F1 на изображение
bool Image::extremum(unsigned int posX, const int& stepOffset, const unsigned int& widthMask)
{
	return (((valueF(posX - stepOffset, widthMask) < valueF(posX, widthMask))
		&& (valueF(posX, widthMask) > valueF(posX + stepOffset, widthMask)))
		|| ((valueF(posX - stepOffset, widthMask) > valueF(posX, widthMask))
			&& (valueF(posX, widthMask) < valueF(posX + stepOffset, widthMask))));
}

					/* методы класса Settings */

// запрашивает у пользователя информацию о настройках
Settings::Settings(unsigned int width)
{
	// ширина накладываемой маски при поиске экстреммум f1
	do {
		cout << " Enter the width of the mask is a multiple of four... ";
		cin >> widthMask;
	} while (widthMask<1 || widthMask > width || widthMask%4);

	// шаг смещения при поиске экстреммум f1
	do {
		cout << " Enter the step offset of the mask... ";
		cin >> stepOffset;
	} while (stepOffset<1 || stepOffset > width);

	// минимальная ширина символа в пикселях
	do {
		cout << " Enter the minimum width of a character... ";
		cin >> minInterval;
	} while (minInterval<1 || minInterval > width);

	// максимальная ширина символа в пикселях
	do {
		cout << " Enter the maximum width of a character... ";
		cin >> maxInterval;
	} while (maxInterval<minInterval+1 || maxInterval > width);

	// допустимый процент наложения гипотез
	do {
		cout << " Enter the percentage of overlapping characters... ";
		cin >> percentOverlay;
	} while (percentOverlay<0 || percentOverlay > 100);
}

					/*	методы класса Compliance */

// выводит на экран данные о соответствии гипотез эталонам
void Compliance::dispay()
{
	cout << endl << x0 << ' ' << xEnd << ' ' << nearestMatch << ' ' << CartesianDistance;
}

						/*методы класса Sample */

// возвращает эталон
char Sample::returnMatch()
{
	return match;
}

// накладывает все 16 фильтров ТАВ
void Sample::filtering(const Image& image, const unsigned int& x0, const unsigned int& xEnd)
{
	unsigned int numberPix = x0;				// начальная координата наложения фильтра
	unsigned int widthMask = (xEnd - x0) / 4;	// масштабируем фильтр по размеру отрезка
	widthMask *= 4;								
	unsigned int height = image.putHeight();	// высота изображения;
	unsigned int width = image.putWidth();		// ширина изображения;
	
	// Последовательно пропускаем пиксели через каждый фильтир 
	for (unsigned int i = 0; i < height; i++)	// внешний цикл наложения фильтров
	{
		for (unsigned int j = 0; j < widthMask; j++, numberPix++) // внутренний цикл наложения фильтров
		{
			// Накладываем фильтр 0
			{	
				Filtered[0] += image[numberPix]; 
			}
			// Накладываем фильтр 1										
			{
				if (j < (width / 2)) Filtered[1] -= image[numberPix];
				else Filtered[1] += image[numberPix];
			}	
		    // Накладываем фильтр 2
			{
				if (i < (height / 2)) Filtered[2] -= image[numberPix];
				else Filtered[2] += image[numberPix];
			}
			// Накладываем фильтр 3
			{
				if ((2 * i / height + 2 * j / widthMask) % 2) Filtered[3] += image[numberPix];
				else Filtered[3] -= image[numberPix];
			}
			// Накладываем фильтр 4
			{
				if (j <= (widthMask / 4) || j > (widthMask * 3 / 4)) Filtered[4] -= image[numberPix];
				else Filtered[4] += image[numberPix];
			}
			// Накладываем фильтр 5
			{
				if (j <= (height / 4) || j > (height * 3 / 4)) Filtered[5] -= image[numberPix];
				else Filtered[5] += image[numberPix];
			}
			// Накладываем фильтр 6
			{
				if (i <= (height / 2) && j <= (widthMask / 4) || i <= (height / 2) && j > (widthMask * 3 / 4) || i > (height / 2) && j > (widthMask / 4) && j <= (widthMask * 3 / 4))
					Filtered[6] -= image[numberPix];
				else Filtered[6] += image[numberPix];
			}
			// Накладываем фильтр 7
			{
				if (j <= (widthMask / 2) && i > (height / 4) && i <= (height * 3 / 4) || j > (widthMask / 2) && i <= (height / 4) || i > (height * 3 / 4) && j > (widthMask / 2))
					Filtered[7] -= image[numberPix];
				else Filtered[7] += image[numberPix];
			}
			// Накладываем фильтр 8
			{
				if (i > (height / 2) && j < (widthMask / 2) || i<(height / 2) && j>(widthMask / 2))
					if ((i / (height / 4) + j / (widthMask / 4)) % 2) Filtered[8] += image[numberPix];
					else Filtered[8] -= image[numberPix];
				else if ((i / (height / 4) + j / (widthMask / 4)) % 2) Filtered[8] -= image[numberPix];
				else Filtered[8] += image[numberPix];
			}
			// Накладываем фильтр 9
			{
				if ((j > (widthMask / 4) && j <= (widthMask / 2)) || j >= (widthMask * 3 / 4))
					Filtered[9] -= image[numberPix];
				else Filtered[9] += image[numberPix];
			}
			// Накладываем фильтр 10
			{
				if (i <= (height / 4) || (i > (height / 2) && i < (height * 3 / 4)))
					Filtered[10] -= image[numberPix];
				else Filtered[10] += image[numberPix];
			}
			// Накладываем фильтр 11
			{
				if (i <= (height / 2))
					if (j>(widthMask / 4) && j <= (widthMask / 2) || j >= (widthMask * 3 / 4))Filtered[11] -= image[numberPix];
					else Filtered[11] += image[numberPix];
				else if (j <= (widthMask / 4) || j>(widthMask / 2) && j<(widthMask * 3 / 4))Filtered[11] -= image[numberPix];
				else Filtered[11] += image[numberPix];
			}
			// Накладываем фильтр 12
			{
				if (j<(widthMask / 2))
					if ((i>(height / 4) && i <= (height / 2)) || i>(height * 3 / 4))Filtered[12] -= image[numberPix];
					else Filtered[12] += image[numberPix];
				else if (i <= (height / 4) || (i >= (height / 2) && i<(height * 3 / 4)))Filtered[12] -= image[numberPix];
				else Filtered[12] += image[numberPix];
			}
			// Накладываем фильтр 13
			{
				if (i<(height / 2))
					if ((i / (height / 4) + j / (widthMask / 4)) % 2)Filtered[13] += image[numberPix];
					else Filtered[13] -= image[numberPix];
				else if ((i / (height / 4) + j / (widthMask / 4)) % 2)Filtered[13] -= image[numberPix];
				else Filtered[13] += image[numberPix];
			}
			// Накладываем фильтр 14
			{
				if (j<(widthMask / 2))
					if ((i / (height / 4) + j / (widthMask / 4)) % 2)Filtered[14] -= image[numberPix];
					else Filtered[14] += image[numberPix];
				else if ((i / (height / 4) + j / (widthMask / 4)) % 2)Filtered[14] += image[numberPix];
				else Filtered[14] -= image[numberPix];
			}
			// Накладываем фильтр 15
			{
				if ((i / (height / 4) + j / (widthMask / 4)) % 2)Filtered[15] += image[numberPix];
				else Filtered[15] -= image[numberPix];
			}
		}
		numberPix += width - widthMask;
	}
}

// Запись новых эталонов в конец файла - баыз данных
void Sample::diskOut()						
{
		cout << "\tRecord a sample in the database";
	cout << "\t\tEnter the symbol... ";  cin >> match;

	ofstream outfile;						// созадан поток вывода
	outfile.open("MatchBase.dat", ios::app | ios::out | ios::binary); // открыть для записи
	outfile.write((char*)this, sizeof(*this));	// Объект записан;
	if (!outfile) {
		throw SampleEx("the entry of new matchs", "MatchBase.dat", "  is not writable!");
	}
	outfile.close();	
}

// из результата наложения фильтра детектируемого изображения
	//вычитает результат наложения фильтра на эталон
float Sample::operator - (const Sample& match)
{		// Евклидова метрика в виде цикла
	long double tempDC = 0.0;
	for (int i = 0; i < QF; i++)
	{
		double minus = Filtered[i] - match.Filtered[i];
		tempDC += minus*minus;
	}
	return (float)(sqrt(tempDC));
}

//сравнивает с эталонами, возвращает ближайший
char Sample::compareWithBase(float& MinCD) 
{											 // и расстояние до него(&MinCD);
	char  nearestMatch = 0;
	float tempCD;				// текущее значение Евклидова расстояния
	Sample tempSample;			// буфер для временного хранения эталонов
	fstream infile;				// открыть поток ввода из файла;
	
	infile.open("MatchBase.dat", ios::in | ios::binary);
	if (!infile) {
		exit(1);
	}
	
	// считываем значения эталонв из обучающей выборки:
	infile.read(reinterpret_cast<char*>(&tempSample), sizeof(tempSample));
	while (!infile.eof())
	{
		tempCD = *this - tempSample;	// находим Евклидово расстояние

		if (!nearestMatch || MinCD > tempCD)// если это первое сравнение или  
		{									// предыдущее Евклидово рсстояние больше
			MinCD = tempCD;					// сохраняем это как минимальное 
											// запоминаем эталон как наиболее близкий
			nearestMatch = tempSample.returnMatch();
		}						// считываем значения эталонв из обучающей выборки:
		infile.read(reinterpret_cast<char*>(&tempSample), sizeof(tempSample));
	}
	infile.close();
	return nearestMatch;		// вернём ближайший эталон
}

// метод обучения с учителем
void Sample::training(Image& image, Settings& user)
{
	unsigned int width = image.putWidth(); // ширина изображения
	unsigned int rightEdge = 0;			// правая граница исследуемого сегмента
	unsigned int leftEdge = 0;				// текущая позиция по X

	char resume;
	do {					// цикл для необходимого числа повторений
		do {				// просим ввести левую границу эталона
			cout << "\nEnter the left character border...";
			cin >> leftEdge;
		} while (leftEdge > (width - user.widthMask) || leftEdge < 0);
		do {				// просим ввести правую границу эталона
			cout << "Enter the right character border...";
			cin >> rightEdge;
		} while ((rightEdge - leftEdge) < user.minInterval || rightEdge > width || (rightEdge - leftEdge) > user.maxInterval);

		char ch = 0;
		// внешний цикл обработки. обход по ширине изображения с заданным шагом 
		for ( ; leftEdge < width - user.widthMask; leftEdge += user.stepOffset)
		{
			if (image.extremum(leftEdge, user.stepOffset, user.widthMask))
			{
				// ищем правую границу искомого объекта(через экстремуму);
				while ((rightEdge - leftEdge > user.minInterval) && (rightEdge < width) && rightEdge > leftEdge)
				{
					if (image.extremum(rightEdge, 1, user.widthMask))
					{
						cout << "The actual border: l = " << leftEdge << " r = " << rightEdge;
						cout << "To preserve the value found(y/n)? ";
						cin >> ch;
						// сохраняем если устраивает
						if (ch == 'y' || ch == 'Y')
						{
							for (int i = 0; i < QF; i++)Filtered[i] = 0.0;
							filtering(image, leftEdge, rightEdge);
							diskOut();
							break;
						}
					}
					rightEdge--;
				}
			}
			if (ch == 'y' || ch == 'Y')
				break;		// выходим из цикла если сохранили эталон
		}
		cout << "Continue entering(y/n)? ";
		resume = _getch();
	} while (resume == 'y' || resume == 'Y');
}

						/* методы класса Strainer */

// поиск соответствий эталонам
void Strainer::selection(Image& image, Settings& user)
{

	unsigned int width = image.putWidth();	// ширина изображения
	unsigned int j = 0;			// правая граница исследуемого сегмента
	int leftEdge = 0;				// 
	float MinCD;				// минимальное Евклидово расстояние
	char  nearestMatch = 0;		// ближайший эталон
	cout << "\nselection..." << endl;
						// внешний цикл обработки. обход по ширине изображения с заданным шагом 
	omp_lock_t lock;
	omp_init_lock(&lock);
	int dinamic_threads = omp_get_dynamic();
	omp_set_dynamic(1);
#pragma omp parallel 
	{
#pragma omp for schedule (guided) private(MinCD) firstprivate(nearestMatch,j) lastprivate(leftEdge)
		// цикл в котором смещается правая граница наложения фильтров
		for (leftEdge = user.stepOffset; leftEdge < width - user.widthMask; leftEdge += user.stepOffset)
		{	
			if (image.extremum(leftEdge, user.stepOffset, user.widthMask))
			{
				j = leftEdge + user.maxInterval;	// ищем правую границу искомого объекта(через экстремуму);
				// цикл в котором смещаестя левая граница наложения фильтров
				while ((j - leftEdge > user.minInterval) && (j < width))
				{	
					if (image.extremum(j, 1, user.widthMask))
					{
						Sample temp;	// хранит результат наложения фильтров на фрагмет изображения
						temp.filtering(image, leftEdge, j);	// наложение фильтров
						nearestMatch = temp.compareWithBase(MinCD);	// находим расстояние до ближайшего эталона 
						omp_set_lock(&lock);						// замок для корректного добавления значений в список
								// добавим найденое значение в список
						compliance.push_back(Compliance(leftEdge, j, nearestMatch, MinCD));
						omp_unset_lock(&lock);						// снимем замок
					}
					j--;		// правая граница смещается справа на лево
				}
			}
		}
    }
	omp_set_dynamic(dinamic_threads);
	omp_destroy_lock(&lock);
}

// убирает лишние значения
void Strainer::minimize(Settings& user)
{
	compliance.sort();			// сортируем по занчению x0(координата начала);
	list<Compliance>::iterator  thisIter, newIter;	//сохздаём итераторы
	thisIter = compliance.begin();				// инициализируем итераторы
	newIter = compliance.begin();
	newIter++;								// смещеаме правый итератор 

	for (; newIter != compliance.end();)		// пока правый итератор указывает на элемент списка
	{
		float layering = (thisIter->xEnd - newIter->x0); // найдём ширину наложения в пикселях
					// найдём ширину того из символов, который меньше:
		float minW = (((thisIter->xEnd - thisIter->x0) < (newIter->xEnd - newIter->x0)) ?
						(thisIter->xEnd - thisIter->x0) : (newIter->xEnd - newIter->x0));
				// сравниваем соотношение ширины наложения и ширины символа с допустимым процентом наложения
		if(layering / minW > (user.percentOverlay / 100)) 
				// сохраняем сивол с наименьшим Евклидовым расстоянием до одного из эталонов
			if (thisIter->CartesianDistance > newIter->CartesianDistance)
			{
				thisIter = compliance.erase(thisIter);
				newIter++;
			}
			else
			{
				newIter = compliance.erase(newIter);
			}
		else
		{
			thisIter = newIter;
			newIter++;
		}
	}
}

// вывод списка с найдеными символами на экран
void Strainer::display()
{
	cout << "\nresult:" << endl;
	list<Compliance>::iterator iter;
	for (iter = compliance.begin(); iter != compliance.end(); ++iter)
		iter->dispay();
	cout << endl;
}