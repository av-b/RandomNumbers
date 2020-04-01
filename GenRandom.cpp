// GenRandom.cpp генерация равномерно распределенных случайных чисел
//
#include "pch.h"
#include <iostream>
#include <cmath>
#include <string>
#include <random>
#include <map>
#include <iomanip>
#include <ctime>
#include <chrono>

using namespace std;
using namespace std::chrono;
int main()
{
	setlocale(LC_ALL, "rus");
	random_device rd; // источник случайного числа для инициализации генератора
	mt19937 gen(rd()); // генератор равномерно распределенных целых случ. чисел
	//   std::mt19937 gen(1234); // инициализация произвольным случайным числом
	const int s = 100000; // количество генерируемых чисел
	int minrand = 0, maxrand = 10000;  // диапазон значений случаных
	int maxhist = 20; // количество карманов гистограммы
	int i, n;
	int g[s]; // массив для записи случайных чисел
	uniform_int_distribution<> distr(minrand, maxrand); // 
	map<int, int> hist; // создание объекта гистограммы из шаблона map
	//======================================================== 
	high_resolution_clock::time_point t0 = high_resolution_clock::now();
	unsigned int start_time = clock(); // запрос системного время вначале
	for (i = 0; i < s; i++) {
		g[i] = distr(gen);
	}
	unsigned int end_time = clock();   // запрос системного время в конце
	high_resolution_clock::time_point  t1 = high_resolution_clock::now();
	//========================================================
	// Построение гистограммы
	float d = (float)maxrand / maxhist; // размер кармана гистограммы
	for (i = 0; i < s; i++) {
		n = (int)ceil(g[i] / d);
		++hist[n];
	}
	// Нормировка значений гистограммы под длину строки консоли
	int m = hist[0];
	for (i = 1; i <= maxhist; i++) {
		if (hist[i] > m) m = hist[i];
	}
	for (i = 0; i <= maxhist; i++) {
		hist[i] = int((float)hist[i] / m * 75);
	}
	// Вывод гистограммы и времени генерации случайных чисел
	for (const auto& elem : hist) {
		cout << setw(3) << elem.first << ' ' << string(elem.second, ':') << endl;
	}
	std::cout << std::endl;
	cout << "Время генерации " << s << " чисел = " << end_time - start_time << " мс" << endl;
	cout << "Время генерации " << s << " чисел = " << duration_cast<microseconds>(t1 - t0).count() << " мкс" << endl;
}