#include <iostream>
#include <vector>
#include <clocale>
#include <ctime>
#include <chrono>

void mul_asm(double* m1, double* m2, double* res, int K, int L, int M);
void mul(double* m1, double* m2, double* res, int K, int L, int M);
void transpose(double* m, double* res, int K, int L);
void print_matrix(double* m, int K, int L);
bool equals(double* m1, double* m2, int N);
void fill_arr_random(double* arr, int N);

int main()
{
	setlocale(LC_ALL, "Russian");
	int K, L, M;
	std::chrono::time_point<std::chrono::system_clock> start, end;
	std::chrono::duration<double> diff;

	std::cout << "Количество строк 1ой матрицы: ";    std::cin >> K;
	std::cout << "Количество столбцов 1ой матрицы: "; std::cin >> L;
	std::cout << "Количество столбцов 2ой матрицы: "; std::cin >> M;

	double* m1 = new double[K * L];
	fill_arr_random(m1, K * L);

	double* m2 = new double[L * M];
	fill_arr_random(m2, L * M);

	double* res1 = new double[K * M];
	for (int i = 0; i < K * M; i++)
		res1[i] = 0;

	double* res2 = new double[K * M];
	for (int i = 0; i < K * M; i++)
		res2[i] = 0;

	start = std::chrono::system_clock::now();
	mul(m1, m2, res2, K, L, M);
	end = std::chrono::system_clock::now();
	diff = end - start;
	std::cout << "Без ассемблера: " << diff.count() << " мс." << std::endl;

	std::chrono::time_point<std::chrono::system_clock> start1 = std::chrono::system_clock::now();
	mul_asm(m1, m2, res1, K, L, M);
	std::chrono::time_point<std::chrono::system_clock> end1 = std::chrono::system_clock::now();
	diff = end1 - start1;
	std::cout << "С ассемблером: " << diff.count() << " мс." << std::endl;

	if (equals(res1, res2, K * M)) {
		std::cout << "Матрицы совпадают" << std::endl;
	}
	else {
		std::cout << "Матрицы НЕ совпадают" << std::endl;
	}

	//std::cout << "m1 " << std::endl;
	//print_matrix(m1, K, L);

	//std::cout << "m2 " << std::endl;
	//print_matrix(m2, L, M);

	//std::cout << "res1 " << std::endl;
	//print_matrix(res1, K, M);

	//std::cout << "res2 " << std::endl;
	//print_matrix(res2, K, M);
}

void mul(double* m1, double* m2, double* res, int K, int L, int M) {
	double* m2_t = new double[M * L];
	transpose(m2, m2_t, L, M);

	for (int i = 0; i < K; ++i) {
		double* m1_row = m1 + i * L;

		for (int j = 0; j < M; ++j) {
			double* m2_t_row = m2_t + j * L;
			for (int k = 0; k < L; k++)
				res[i * M + j] += m1_row[k] * m2_t_row[k];
		}
	}
	delete[] m2_t;
}

void mul_asm(double* m1, double* m2, double* res, int K, int L, int M) {
	double* m2_t = new double[M * L];
	transpose(m2, m2_t, L, M);
	double* tmp = new double[4];

	for (int i = 0; i < K; ++i) {
		double* m1_row = m1 + i * L;

		for (int j = 0; j < M; ++j) {
			double* m2_t_row = m2_t + j * L;

			for (int k = 0; k < L / 4 * 4; k += 4) {
				double* m1_row_seg = m1_row + k;
				double* m2_t_row_seg = m2_t_row + k;

				__asm {
					mov eax, [m1_row_seg]    // Кладем в eax ссылку на line_m1_segment
					vmovupd ymm0, [eax]      // Загружаем в вектор элементы по ссылке eax
					mov edx, [m2_t_row_seg]  // Кладем в edx ссылку на line_transpose_m2_segment
					vmovupd ymm1, [edx]      // Загружаем в вектор элементы по ссылке edx
					vmulpd ymm0, ymm0, ymm1  // Умножаем поэлементно из векторов ymm0 и ymm1 и записываем в ymm0
					mov edx, [tmp]           // Кладем в edx ссылку на tmp_res
					vmovupd[edx], ymm0       // Загружаем в edx элементы вектора
				}

				for (int z = 0; z < 4; ++z) {
					res[i * M + j] += tmp[z];
				}
			}

			for (int k = L / 4 * 4; k < L; k++) {
				res[i * M + j] += m1_row[k] * m2_t_row[k];
			}
		}
	}
	delete[] tmp;
	delete[] m2_t;
}

void transpose(double* m, double* res, int K, int L) {
	for (int i = 0; i < L; ++i) {
		for (int j = 0; j < K; ++j) {
			res[i * K + j] = m[j * L + i];
		}
	}
}

void print_matrix(double* m, int K, int L) {
	for (int i = 0; i < K; i++) {
		for (int j = 0; j < L; j++) {
			std::cout << m[i * L + j] << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

bool equals(double* m1, double* m2, int N) {
	for (int i = 0; i < N; i++) {
		if (m1[i] != m2[i]) {
			return false;
		}
	}
	return true;
}

void fill_arr_random(double* arr, int N) {
	double low = 1.0;
	double high = 100.0;
	for (int i = 0; i < N; i++) {
		arr[i] = low + (double)rand() / (double)(RAND_MAX / (high - low));
	}
}