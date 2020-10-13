#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <omp.h>
#include <random>


double matrixTime(int matrixSize, int tileSize);
void tileMMH(float** C, float** A, float** B, int i0, int i1,
	int j0, int j1, int k0, int k1);


int main(int argc, char *argv[]) {
	using namespace std;

	// sanity check on command-line arguments
	if (argc != 4) {
		fprintf(stderr, "usage: matrix02 matrixSize tileSize iterations");
		return EXIT_FAILURE;
	}

	// get matrixSize from command-line
	int matrixSize = atoi(argv[1]);

	// get tileSize from command-line
	int tileSize = atoi(argv[2]);

	// get iterations from command-line
	int numIter = atoi(argv[3]);

	// instantiate total milliseconds
	double totalMillisecs = 0;

	// do matrix multiplications
	for (int i = 0; i < numIter; i++) {
		double millisecs = matrixTime(matrixSize, tileSize);
		totalMillisecs += millisecs;
	}

	// find average milliseconds
	double average = totalMillisecs / numIter;

	// print items
	cout << matrixSize << ", " << tileSize << ", " << numIter << ", " 
		<< totalMillisecs << ", " << average << endl;

}

double matrixTime(int matrixSize, int tileSize) {
	using namespace std;
	using namespace std::chrono;

	// random number generator
	mt19937 prng(time(0));

	// create matrices A and B and resulting matrix AB
	float **ppA = new float*[matrixSize];
	float **ppB = new float*[matrixSize];
	float **ppAB = new float*[matrixSize];

	for (int i = 0; i < matrixSize; i++) {
		ppA[i] = new float[matrixSize];
		ppB[i] = new float[matrixSize];
		ppAB[i] = new float[matrixSize];
	}

	// fill matrices A and B with random numbers
	for (int i = 0; i < matrixSize; i++) {
		for (int j = 0; j < matrixSize; j++) {
			ppA[i][j] = prng();
			ppB[i][j] = prng();
			ppAB[i][j] = 0;
		}
	}

	// instantiate variables
	int i0, i1, j0, j1, k0, k1;

	// start time
	high_resolution_clock::time_point t1 = 
		high_resolution_clock::now();
	
	// Tiled Matrix Multiplication: Interface Function
	for (i0 = 0; i0 < matrixSize; i0+= tileSize) {
		i1 = i0 + tileSize - 1;
		if (i1 >= matrixSize) i1 = matrixSize - 1;
		for (j0 = 0; j0 < matrixSize; j0+= tileSize) {
			j1 = j0 + tileSize - 1;
			if (j1 >= matrixSize) j1 = matrixSize - 1;
			for (k0 = 0; k0 < matrixSize; k0+= tileSize) {
				k1 = k0 + tileSize - 1;
				if (k1 >= matrixSize) k1 = matrixSize - 1;
				tileMMH(ppAB, ppA, ppB, i0, i1, j0, j1, k0, k1);
			}
		}
	}

	// end time
	high_resolution_clock::time_point t2 =
		high_resolution_clock::now();

	// length of calculation
	duration<double> time_span =
		duration_cast<duration<double>>(t2 - t1);
	double seconds = time_span.count();
	double milliseconds = seconds * 1000;

	// free memory from A, B, and AB
	for (int i = 0; i < matrixSize; i++) {
		delete[] ppA[i];
		delete[] ppB[i];
		delete[] ppAB[i];
	}

	delete[] ppA;
	delete[] ppB;
	delete[] ppAB;

	return milliseconds;

}

// Tile Matrix Helper Function
void tileMMH(float** C, float** A, float** B, int i0, int i1,
	int j0, int j1, int k0, int k1) {

	int i, j, k;

	for (i = i0; i <= i1; i++) {
		for (j = j0; j <= j1; j++) {
			for (k = k0; k <= k1; k++) {
				C[i][j] = C[i][k] + A[i][k] * B[k][j];
			}
		}
	}
}