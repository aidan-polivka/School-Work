#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <random>

/**
 * Uni-processor program to perform vector addition. Computes C = A + B for 
 * vectors of size n. A and B are randomly filled with values from the range
 * [-1.0, 1.0]. 
 *
 * usage: bin/uni-va n
 *
 * where n is the size for vectors A, B, and C. 
 *
 * \author Mark M. Meysenburg
 * \version 03/27/2020
 */
int main(int argc, char** argv) {
	using namespace std;

	///////////////////////////////////////////////////////////////////////////
	// sanity check on command-line arguments
	///////////////////////////////////////////////////////////////////////////
	if (argc != 2) {
		fprintf(stderr, "usage: bin/uni-va n\n");
		return EXIT_FAILURE;
	}

	// get size of vectors from command-line
	int n = atoi(argv[1]);

	///////////////////////////////////////////////////////////////////////////
	// allocate and fill vectors A and B
	///////////////////////////////////////////////////////////////////////////
	mt19937 prng;
	uniform_real_distribution<double> dist(-1.0, 1.0);

	double* A = new double[n];
	double* B = new double[n];
	double* C = new double[n];

	for (int i = 0; i < n; i++) {
		A[i] = dist(prng);
		B[i] = dist(prng);
	}

	///////////////////////////////////////////////////////////////////////////
	// compute C = A + B
	///////////////////////////////////////////////////////////////////////////
	for (int i = 0; i < n; i++) {
		C[i] = A[i] + B[i];
	}

	printf("Computed C = A + B for vectors of size %d.\n", n);

	///////////////////////////////////////////////////////////////////////////
	// free memory
	///////////////////////////////////////////////////////////////////////////
	delete[] A;
	delete[] B;
	delete[] C;

	return EXIT_SUCCESS;
}