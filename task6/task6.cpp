#include <iostream>
#include <chrono>
#include <clocale>

double func5(double x) {
	return sin(1.9 * x) / (pow(x, 4) - 1.5);
}

double func5_asm(double x) {
	double res;
	double coef1 = 1.9, coef2 = 1.5;
	__asm
	{
		finit // Инициализация сопроцессора, элементы хранятся в стеке. Элементы стека обозначаются st(i), где i=0..n
		fld x // st0 = x => добавляем x в стек
		fld coef1 // st0 = 1.9, st1 = x
		fmul st(0), st(1) // st0 = 1.9x, st1 = x
		fsin //st0 = sin(1.9x), st1 = x
		fxch st(1) // st0 = x, st1 = sin(1.9x)
		fmul st(0), st(0) //st0 = x^2, st1 = sin(1.9x)
		fmul st(0), st(0) //st0 = x^4, st1 = sin(1.9x)
		fld coef2 //st0 = 1.5, st1 = x^4, st2 = sin(1.9x)
		fxch st(1) //st0 = x^4, st1 = 1.5, st2 = sin(1.9x)
		fsub st(0), st(1) //st0 = x^4 - 1.5, st1 = 1.5, st2 = sin(1.9x)
		fxch st(2) //st0 = sin(1.9x), st1 = 1.5, st2 = x^4 - 1.5
		fdiv st(0), st(2) //st0 = sin(1.9x) / (x^4 - 1.5), st1 = 1.5, st2 = x^4 - 1.5
		fst res
	}
	return res;
}

int main()
{
	setlocale(LC_ALL, "Russian");

	double x, result;
	long long duration;
	std::chrono::steady_clock::time_point start, end;

	std::cout << "Введите x: " << std::endl;
	std::cin >> x;

	start = std::chrono::high_resolution_clock::now();
	result = func5(x);
	end = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count(); //время в наносекундах
	std::cout << "Значение функции на C++: " << result << std::endl;
	std::cout << "Время: " << duration << " наносекунд" << std::endl;

	start = std::chrono::high_resolution_clock::now();
	result = func5_asm(x);
	end = std::chrono::high_resolution_clock::now();
	duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
	std::cout << "Значение функции на Ассемблере: " << result << std::endl;
	std::cout << "Время: " << duration << " наносекунд" << std::endl;
}

