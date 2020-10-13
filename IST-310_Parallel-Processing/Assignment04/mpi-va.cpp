#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <random>
#include <mpi.h>


/**
 * Convert this program to calculate C = A + B in parallel, using OpenMPI.
 */
int main(int argc, char** argv) {
	using namespace std;

	int SIZE_N = 0;		// constant for sending size of vector
	int ARRAY_A = 1;	// constant for sending fragment from array A
	int ARRAY_B = 2;	// constant for sending fragment from array B
	int ARRAY_C = 3;	// constant for sending fragment from array C

	int rank;		// processor's rank ID
	int nProcs;		// number of processors in the communicator

	// initialize MPI constructs
	MPI_Init(&argc, &argv);					// set up the communicator
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);	// which processor am I
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);	// how many processors are there

	// Master Processor
	if (rank == 0) {

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
		// Split Arrays into Segments and Send to Slaves for Parallelism
		///////////////////////////////////////////////////////////////////////////

		int start = 0;
		int frag_n = n / (nProcs - 1);
		int check = n % frag_n;
		if (check != 0) {
			frag_n++;
		}

		// Split and Send to each Slave Processor
		for (int i = 1; i < (nProcs - 1); i++) {
			MPI_Send(&frag_n, 1, MPI_INT, i, SIZE_N, MPI_COMM_WORLD);
			MPI_Send(A + start, frag_n, MPI_DOUBLE, i, ARRAY_A, MPI_COMM_WORLD);
			MPI_Send(B + start, frag_n, MPI_DOUBLE, i, ARRAY_B, MPI_COMM_WORLD);
			start += frag_n;
		}

		int frag_last = n - start;
		// Final Fragment may be Oddly Sized
		MPI_Send(&frag_last, 1, MPI_INT, nProcs - 1, SIZE_N, MPI_COMM_WORLD);
		MPI_Send(A + start, frag_last, MPI_DOUBLE, nProcs - 1, ARRAY_A, MPI_COMM_WORLD);
		MPI_Send(B + start, frag_last, MPI_DOUBLE, nProcs - 1, ARRAY_B, MPI_COMM_WORLD);

		///////////////////////////////////////////////////////////////////////////
		// Receive and Concatenate Array Segments
		///////////////////////////////////////////////////////////////////////////

		start = 0;
		MPI_Status status;
		for (int i = 1; i < nProcs; i++) {
			MPI_Recv(C + start, frag_n, MPI_DOUBLE, i, ARRAY_C, MPI_COMM_WORLD, &status);
			start += frag_n;
		}

		printf("Computed C = A + B for vectors of size %d.\n", n);

		///////////////////////////////////////////////////////////////////////////
		// Free Memory
		///////////////////////////////////////////////////////////////////////////

		delete[] A;
		delete[] B;
		delete[] C;

	}

	//Slave Processors
	else {

		///////////////////////////////////////////////////////////////////////////
		// Receive info from Master Processor
		///////////////////////////////////////////////////////////////////////////

		int segSize;
		MPI_Status status;
		MPI_Recv(&segSize, 1, MPI_INT, 0, SIZE_N, MPI_COMM_WORLD, &status);

		double* aArray = new double[segSize];
		double* bArray = new double[segSize];
		double* cArray = new double[segSize];

		MPI_Recv(aArray, segSize, MPI_DOUBLE, 0, ARRAY_A, MPI_COMM_WORLD, &status);
		MPI_Recv(bArray, segSize, MPI_DOUBLE, 0, ARRAY_B, MPI_COMM_WORLD, &status);


		///////////////////////////////////////////////////////////////////////////
		// Compute C = A + B, Send Result, and Free Memory
		///////////////////////////////////////////////////////////////////////////

		for (int i = 0; i < segSize; i++) {
			cArray[i] = aArray[i] + bArray[i];
		}

		MPI_Send(cArray, segSize, MPI_DOUBLE, 0, ARRAY_C, MPI_COMM_WORLD);

		delete[] aArray;
		delete[] bArray;
		delete[] cArray;

	}

	MPI_Finalize();

	return EXIT_SUCCESS;
}