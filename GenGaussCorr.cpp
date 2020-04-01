// GenGauss.cpp генерация гауссовских случайных чисел
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
	const int s = 10000; // количество генерируемых чисел
	double mo = 0, sig = 1; // задание параметров генератора случайных чисел
	double r = 0.95; // коэффициент корреляции соседних отсчетов
	double rsqr = sqrt(1 - r * r);
	int bins = 50; // количество карманов гистограммы
	int i, n;
	double g[s]; // массив для записи случайных чисел
	normal_distribution<> distr(mo, sig); // инициализация генератора сл. чисел
	map<int, int> hist; // создание объекта гистограммы из шаблона map
	//======================================================== 
	high_resolution_clock::time_point t0 = high_resolution_clock::now();
	unsigned int start_time = clock(); // запрос системного время вначале
	g[0] = distr(gen);
	for (i = 1; i < s; i++) {
		g[i] = rsqr * distr(gen) + r * g[i - 1];
	}
	unsigned int end_time = clock();   // запрос системного время в конце
	high_resolution_clock::time_point  t1 = high_resolution_clock::now();
	//========================================================
	// Построение гистограммы
	double M = 0, D = 0;
	double ming = -2 * sig + mo, maxg = 2 * sig + mo; // пределы изменения g
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
	cout<<endl<<setw(50)<<"Гистограмма распределения"<< endl << endl;
	for (i = 1; i < bins; i++) {
		if (hist[i] + hist[i - 1] + hist[i + 1] != 0) {
			cout << setw(3) << i << ' ' << string(hist[i], ':') << endl;
		}
	}
	double gcor[75] = { 0 };
	for (i = 0; i < s; i++) gcor[0] += g[i] * g[i];
	for (int k = 1; k < 75; k++) {
		for (i = 0; i < s - k; i++) {
			gcor[k] += g[i] * g[i + k];
		} gcor[k] /= gcor[0];
	}
	gcor[0] = 1;
	cout<<endl<<setw(50)<<"Автокорреляционная функция"<<endl<<" \x5E"<< endl;
	char stroka[75];
	bool sth[75] = { 0 };
	for (i = 20; i >=0; i--) {
		for (int k = 0; k < 74; k++) {
			if ((gcor[k] * 20 - 0.5) > i && ! sth[k]) {
				stroka[k] = '*';
				sth[k] = true;
			}
			else stroka[k] = ' ';
		}
		stroka[74] = '\0';
		cout << " |"<< stroka << endl;
	}
	cout << " ";
	for (int k = 1; k < 76; k++) cout << '\x97';
	cout << '>' << endl << setw(75) << "Дискретное время" << endl << endl;

	cout << "min(g) = " << ming << "; max(g) = " << maxg
		<< ";  M = " << M << "; ско = " << D << endl;
	cout << "Время генерации " << s << " чисел = " << end_time - start_time << " мс" << endl;
	cout << "Время генерации " << s << " чисел = " << duration_cast<microseconds>(t1 - t0).count() << " мкс" << endl;
}