// GenRay.cpp генерация случайных чисел рэлеевским распределением
//
#include "pch.h"
#define _USE_MATH_DEFINES
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
	double a = 0, b = 1; // задание параметров генератора равномерного распределения
	double sig = 1. * sqrt(1/(2 - M_PI_2)); // параметр генератора чисел с релевским распределения 
	int bins = 40; // количество карманов гистограммы
	int i, n;
	double g[s]; // массив для записи случайных чисел
	uniform_real_distribution<> distr(a, b); // инициализация генератора сл. чисел
	map<int, int> hist; // создание объекта гистограммы из шаблона map
	//======================================================== 
	high_resolution_clock::time_point t0 = high_resolution_clock::now();
	unsigned int start_time = clock(); // запрос системного время вначале
	for (i = 0; i < s; i++) {
		g[i] = sig * sqrt(-2*log(distr(gen)));
	}
	unsigned int end_time = clock();   // запрос системного время в конце
	high_resolution_clock::time_point  t1 = high_resolution_clock::now();
	//========================================================
	// Построение гистограммы
	double M = 0, D = 0;
	double ming = 0, maxg = 2 * sig; // пределы изменения g
	for (i = 0; i < s; i++) {
		if (g[i] < ming) ming = g[i];
		if (g[i] > maxg) maxg = g[i];
		M += g[i];
		D += g[i] * g[i];
	}
	M = M / s; D = sqrt(D / s - M * M);
	double d = (maxg - ming) / bins; // размер кармана гистограммы
	for (i = 0; i < s; i++) {
		n = (int)ceil((g[i] - ming) / d);
		++hist[n];
	}
	// Нормировка значений гистограммы под длину строки консоли
	double m = hist[0];
	for (i = 1; i <= bins; i++) {
		if (hist[i] > m) m = hist[i];
	}
	for (i = 0; i <= bins; i++) {
		hist[i] = int(hist[i] / m * 75);
	}
	// Вывод гистограммы и времени генерации случайных чисел
	for (i = 1; i < bins; i++) {
		if (hist[i] + hist[i - 1] + hist[i + 1] != 0) {
			std::cout << setw(3) << i << ' ' << string(hist[i], ':') << endl;
		}
	}
	std::cout << std::endl;
	std::cout << "min(g) = " << ming << "; max(g) = " << maxg
		<< ";  M = " << M << "; ско = " << D << endl;
	std::cout << "Время генерации " << s << " чисел = " << end_time - start_time << " мс" << endl;
	std::cout << "Время генерации " << s << " чисел = " << duration_cast<microseconds>(t1 - t0).count() << " мкс" << endl;
}