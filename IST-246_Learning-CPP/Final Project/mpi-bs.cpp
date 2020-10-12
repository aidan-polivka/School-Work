#include <chrono>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <random>
#include "mpi.h"

/**
 * Parallelized bucket sort program using MPI.
 *
 * Fall 2019 IST 246 Final Assignment
 */

/**
 * Determine if an array of unsigned integers is sorted.
 *
 * \param pArr Pointer to the first element of the array.
 *
 * \param n Size of the array.
 *
 * \return True if the array is sorted in non-descending order, false
 * otherwise.
 */
bool isSorted(unsigned *pArr, int n) {
	for (int i = 0; i < n - 1; i++) {
		if (pArr[i + 1] < pArr[i]) {
			return false;
		} // if
	} // for
	return true;
}

/**
 * Application entry point.
 *
 * \param argc Number of command line arguments; ignored by this app.
 * 
 * \param pArgv Array of command line arguments; ignored by this app. 
 */
int main(int argc, char* pArgv[]) {
	using namespace std;

	int rank;		// processor's rank ID
	int nProcs;		// number of processors in use

	// initialize MPI constructs
	MPI_Init(&argc, &pArgv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);

	unsigned *pMaster = 0;
	int masterN = 100000000;

	// root process fills master array
	if (rank == 0) {
		pMaster = new unsigned[masterN];

		mt19937 prng(chrono::system_clock::now().time_since_epoch().count());
		for (int i = 0; i < masterN; i++) {
			pMaster[i] = (unsigned)prng();
		}
	}


	/****************************************************************************
	 * Put your bucketsort code here!                                           *
	 ****************************************************************************/


	if(rank == 0) {
		// master validates sorting and frees memory
		printf("**********************************************************\n");
		if (isSorted(pMaster, masterN)) {
			printf("Array is sorted.\n");
		} else {
			printf("Array is not sorted.\n");
		}
		printf("**********************************************************\n");

		delete[] pMaster;

	} 

	MPI_Finalize();

	return EXIT_SUCCESS;
}
