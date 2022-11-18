#include <algorithm>
#include <intrin.h>
#include <iostream>
#include <vector>

#include <clocale>

using namespace std;

void generate_random_arr(double arr[]);
double* get_time(int times_count);

const int SIZE = 10000000;

int main()
{
	setlocale(LC_ALL, "Russian");
	int count;
	std::cout << "Размер массива: " << SIZE << std::endl;
	std::cout << "Количество запусков: "; std::cin >> count;

	double* res = get_time(count);

	std::cout << "Результаты: " << std::endl;
	for (int i = 0; i < count; i++) {
		std::cout << res[i] << " ";
	}

	delete[] res;
	return EXIT_SUCCESS;
}

double* get_time(int times_count)
{
	unsigned clock = 0;

	// время для каждой размерности массива
	double* res = new double[times_count];
	for (int i = 0; i < times_count; ++i) {
		res[i] = 0;
	}

	// массив, в по которму будем ходить
	double* arr;

	for (int i = 0; i < times_count; i++) {
		arr = new double[SIZE];

		generate_random_arr(arr);

		//одиночный обход массива для "разогрева" кэш-памяти
		int j = 0;
		for (double i = 0; i < SIZE; i++) {
			j = (int)arr[j];
		}

		//получение времени в тактах
		__asm
		{
			rdtscp;
			mov[clock], eax;
		}

		//обход
		j = 0;
		for (double i = 0; i < SIZE; i++) {
			j = (int)arr[j];
		}

		__asm
		{
			rdtscp;
			sub eax, [clock]
				mov[clock], eax;
		}

		std::cout << "Количество тактов: " << clock << std::endl;
		res[i] = clock;
		delete[] arr;
	}

	return res;
}

void generate_random_arr(double arr[]) {
	srand(time(NULL));
	std::vector<double> index;
	for (double i = 0; i < SIZE; ++i)
		index.push_back(i);
	std::random_shuffle(index.begin(), index.end());
	for (int i = 0; i < SIZE; i++)
		arr[i] = index[i];
}