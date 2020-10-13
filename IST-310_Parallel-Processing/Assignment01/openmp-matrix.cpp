#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <omp.h>
#include <random>

/**
 * OpenMP-enabled matrix multiplication program. Creates two decently-sized 
 * matrices, fills them with pseduo-random values, and multiplies them in an
 * OpenMP parallelized block. The code outputs the number of seconds required 
 * to do the multiplication only.
 *
 * Control the number of threads used by the parallelism from the bash shell.
 * There's no need to edit this code, or recompile, to change the number of 
 * threads used. 
 * 
 * Enter this command before running the program:
 *		export OMP_NUM_THREADS=<number of threads to use>
 *
 * To make sure the thread count was registered, enter this command and check
 * the value:
 *		echo $OMP_NUM_THREADS
 */
int main() {
	using namespace std;

	// random number generator
	mt19937 prng(time(0));

	// matrix size variables
	int n = 1000, m = 2000, p = 1500;

	// create matrices A and B
	int **ppA = new int*[n];
	for (int i = 0; i < n; i++) {
		ppA[i] = new int[m];
	}
	int **ppB = new int*[m];
	for (int i = 0; i < m; i++) {
		ppB[i] = new int[p];
	}

	// fill matrices A and B with random numbers
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			ppA[i][j] = prng();
		}
	}

	for (int i = 0; i < m; i++) {
		for (int j = 0; j < p; j++) {
			ppB[i][j] = prng();
		}
	}

	// create result matrix AB
	int **ppAB = new int*[n];
	for (int i = 0; i < n; i++) {
		ppAB[i] = new int[p];
	}

	// start timing
	chrono::high_resolution_clock::time_point t1 =
		chrono::high_resolution_clock::now();

	// perform multiplication
#pragma omp parallel shared(ppA, ppB, ppAB)
	{
		// output number of threads used for data collection
		if (omp_get_thread_num() == 0) {
			cout << omp_get_num_threads() << ","; 
		}

		// perform multiplication
#pragma omp for
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < p; j++) {
				ppAB[i][j] = 0.0;
				for (int k = 0; k < m; k++) {
					ppAB[i][j] += ppA[i][k] * ppB[k][j];
				}
			}
		}
	}

	// stop timing
	chrono::high_resolution_clock::time_point t2 =
		chrono::high_resolution_clock::now();

	// compute and print the elapsed time
	chrono::duration<double> time_span =
		chrono::duration_cast<chrono::duration<double>>(t2 - t1);

	cout << time_span.count() << endl;

	// delete memory for A, B and AB
	for (int i = 0; i < n; i++) {
		delete[] ppA[i];
	}
	delete[] ppA;
	for (int i = 0; i < m; i++) {
		delete[] ppB[i];
	}
	delete[] ppB;
	for (int i = 0; i < n; i++) {
		delete[] ppAB[i];
	}
	delete[] ppAB;

	return	EXIT_SUCCESS;
}