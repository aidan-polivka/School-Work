#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <list>
#include <random>
#include <mpi.h>
#include "Evaluate.h"
#include "Operator.h"
#include "PointCrossover.h"
#include "PointMutation.h"
#include "TournamentSelection.h"

#include "Ackley.h"

#define PI 3.1415926535897932384626433832795
#define E 2.7182818284590452353602874713527

///////////////////////////////////////////////////////////////////////////////
// Implementations for Ackley functions
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Constructor
///////////////////////////////////////////////////////////////////////////////
Ackley::Ackley(std::mt19937* pRNG) : Problem(2, pRNG)
{
	// genes in the chromosome are in [-5, 5]
	pGeneDist = new std::uniform_real_distribution<double>(-5, 5);
	// there are two genes, indexed [0, 1]
	pGeneLocDist = new std::uniform_int_distribution<int>(0, 1);
}

///////////////////////////////////////////////////////////////////////////////
// Destructor
///////////////////////////////////////////////////////////////////////////////
Ackley::~Ackley()
{
	if (pGeneDist != 0) delete pGeneDist;
	if (pGeneLocDist != 0) delete pGeneLocDist;
}

///////////////////////////////////////////////////////////////////////////////
// Create an individual
///////////////////////////////////////////////////////////////////////////////
double* Ackley::createChromosome()
{
	double* pChromosome = new double[length];
	for (int i = 0; i < length; i++)
	{
		pChromosome[i] = getRandomGene();
	}
	return pChromosome;
}

///////////////////////////////////////////////////////////////////////////////
// Evaluate an individual
///////////////////////////////////////////////////////////////////////////////
double Ackley::evaluateChromosome(double* pChrom)
{
	double a = 20.0 * exp(-0.2 * sqrt(0.5 *
		(pChrom[0] * pChrom[0] + pChrom[1] * pChrom[1])));
	double b = exp(0.5 * (cos(2 * PI * pChrom[0]) + cos(2 * PI * pChrom[1])));
	return a + b - E - 20.0;
}

///////////////////////////////////////////////////////////////////////////////
// Get a random gene value
///////////////////////////////////////////////////////////////////////////////
double Ackley::getRandomGene()
{
	return (*pGeneDist)(*pPRNG);
}

///////////////////////////////////////////////////////////////////////////////
// Get a random location in the chromosome
///////////////////////////////////////////////////////////////////////////////
int Ackley::getRandomGeneLoc()
{
	return (*pGeneLocDist)(*pPRNG);
}

///////////////////////////////////////////////////////////////////////////////
// Application entry point for a GA 
///////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
	using namespace std;

	// Tags for sending and receiving comand line arguments
	int TAG_POPSIZE = 0;		// constant tag for sending population size
	int TAG_NUM_GENS = 1;		// constant tag for sending number of generations
	int TAG_CROSS_PROB = 2;		// constant tag for sending crossover probability
	int TAG_MUT_PROB = 3;		// constant tag for sending mutation probability
	int TAG_TOURNSIZE = 4;		// constant tag for sending tournament size
	int TAG_SEED = 5;			// constant tag for sending seed value
	int TAG_MIGRATION = 6;		// constant tag for sending migration iteration value

	// Tags for sending best and worst items between ranks
	int TAG_MIG_INDV_BEST = 7;		// constant tag for sending best individual
	int TAG_MIG_FIT_BEST = 8;		// constant tag for sending best fitness
	int TAG_MIG_INDV_WORST = 9;		// constant tag for sending worst individual
	int TAG_MIG_FIT_WORST = 10;		// constant tag for sending worst fitness

	// Tags for sending final best fits, individuals, and size
	int TAG_INDV_RESULT = 11;		// constant tag for final individual
	int TAG_FIT_RESULT = 12;		// constant tag for final fitness
	int TAG_SIZE_RESULT = 13;		// constant tag for final size

	// Rank and Number of Processors
	int rank;		// processor's rank ID
	int nProcs;		// number of processors in the communicator

	// initialize MPI constructs
	MPI_Init(&argc, &argv);					// set up the communicator
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);	// which processor am I?
	MPI_Comm_size(MPI_COMM_WORLD, &nProcs);	// how many processors are there?

	// Master Rank Function
	if (rank == 0) {

		// sanity check on command-line parameters
		if (argc != 8) {
			printf("usage: bin/DeJong01 N G chi mu k seed migration\n");
			return EXIT_FAILURE;
		}

		///////////////////////////////////////////////////////////////////////
		//// parse command-line parameters
		///////////////////////////////////////////////////////////////////////

		int N = atoi(argv[1]);			// population size
		int G = atoi(argv[2]);			// number of generations
		double chi = atof(argv[3]);		// crossover probability
		double mu = atof(argv[4]);		// mutation probability
		int k = atoi(argv[5]);			// tournament size
		int seed = atoi(argv[6]);		// PRNG seed value
		int migration = atoi(argv[7]);	// migration iteration value

		///////////////////////////////////////////////////////////////////////
		//// send command-line parameters from master
		///////////////////////////////////////////////////////////////////////

		for (int i = 1; i < nProcs; i++) {
			MPI_Send(&N, 1, MPI_INT, i, TAG_POPSIZE, MPI_COMM_WORLD);
			MPI_Send(&G, 1, MPI_INT, i, TAG_NUM_GENS, MPI_COMM_WORLD);
			MPI_Send(&chi, 1, MPI_DOUBLE, i, TAG_CROSS_PROB, MPI_COMM_WORLD);
			MPI_Send(&mu, 1, MPI_DOUBLE, i, TAG_MUT_PROB, MPI_COMM_WORLD);
			MPI_Send(&k, 1, MPI_INT, i, TAG_TOURNSIZE, MPI_COMM_WORLD);
			MPI_Send(&seed, 1, MPI_INT, i, TAG_SEED, MPI_COMM_WORLD);
			MPI_Send(&migration, 1, MPI_INT, i, TAG_MIGRATION, MPI_COMM_WORLD);
		}


		///////////////////////////////////////////////////////////////////////
		//// receive best individuals from each sub-processor
		///////////////////////////////////////////////////////////////////////

		MPI_Status status;

		double* fitness = new double[nProcs - 1];
		int* size = new int[nProcs - 1];
		int bestSize = 0;
		int successfulRank = 0;
		double maxFitness = -numeric_limits<double>::max();
		for (int i = 1; i < nProcs; i++) {
			MPI_Recv(&fitness[i], 1, MPI_DOUBLE, i, TAG_FIT_RESULT, MPI_COMM_WORLD, &status);
			MPI_Recv(&size[i], 1, MPI_INT, i, TAG_SIZE_RESULT, MPI_COMM_WORLD, &status);
		}

		double* pUltimateIndividual = new double[size[1]];
		double* buffer = 0;
		double* individuals = new double[size[1]];
		for (int i = 1; i < nProcs; i++) {
			for (int j = 0; j < size[i]; j++) {
				MPI_Recv(&individuals[j], 1, MPI_DOUBLE, i, TAG_INDV_RESULT, MPI_COMM_WORLD, &status);
			}
			if (maxFitness < fitness[i]) {
				maxFitness = fitness[i];
				successfulRank = i;
				bestSize = size[i];
				buffer = pUltimateIndividual;
				pUltimateIndividual = new double[bestSize];
				for (int j = 0; j < bestSize; j++) {
					pUltimateIndividual[j] = individuals[j];
				}
				delete[] buffer;
			}
			buffer = individuals;
			individuals = new double[size[i + 1]];
			delete[] buffer;
		}


		///////////////////////////////////////////////////////////////////////
		//// report the best-ever individual and its fitness
		///////////////////////////////////////////////////////////////////////


		// print results
		cout << "Here is the solution:" << endl;
		cout << successfulRank << " = (";
		for (int i = 0; i < bestSize - 1; i++) {
			cout << pUltimateIndividual[i] << ", ";
		}
		cout << pUltimateIndividual[bestSize - 1] << ") : " << maxFitness << endl;
		cout << endl;

		// free memory
		delete[] individuals;
		delete[] fitness;
		delete[] size;
		delete[] pUltimateIndividual;

	}

	// Sub ranks method
	else {

		//////////////////////////////////////////////////////////////////////
		//// receive command line parameters from master-processor
		//////////////////////////////////////////////////////////////////////
		MPI_Status status;

		int N;			// population size
		int G;			// number of generations
		double chi;		// crossover probability
		double mu;		// mutation probability
		int k;			// tournament size
		int seed;		// PRNG seed value
		int migration;	// migration iteration

		MPI_Recv(&N, 1, MPI_INT, 0, TAG_POPSIZE, MPI_COMM_WORLD, &status);
		MPI_Recv(&G, 1, MPI_INT, 0, TAG_NUM_GENS, MPI_COMM_WORLD, &status);
		MPI_Recv(&chi, 1, MPI_DOUBLE, 0, TAG_CROSS_PROB, MPI_COMM_WORLD, &status);
		MPI_Recv(&mu, 1, MPI_DOUBLE, 0, TAG_MUT_PROB, MPI_COMM_WORLD, &status);
		MPI_Recv(&k, 1, MPI_INT, 0, TAG_TOURNSIZE, MPI_COMM_WORLD, &status);
		MPI_Recv(&seed, 1, MPI_INT, 0, TAG_SEED, MPI_COMM_WORLD, &status);
		MPI_Recv(&migration, 1, MPI_INT, 0, TAG_MIGRATION, MPI_COMM_WORLD, &status);


		/////////////////////////////////////////////////////////////////////
		//// initialize variables and create seed value
		/////////////////////////////////////////////////////////////////////

		int bestIndividualLocation = 0;
		int worstIndividualLocation = 0;

		// create PRNG
		mt19937 PRNG((seed + rank));
		// create problem
		Problem* pProblem = new Ackley(&PRNG);

		// allocate fitnesses array
		double* pFitnesses = new double[N];

		// allocate population and temporary population arrays
		double** ppPop = new double* [N];

		// initialize population with random chromosomes
		for (int i = 0; i < N; i++) {
			ppPop[i] = pProblem->createChromosome();
		}

		// initialize problem length
		int chromosomesLength = pProblem->getLength();

		// create linked list to hold operators
		list<Operator*> operators;
		// add the operators to the list: mutation, crossover, evaluation,
		// and selection. Selection has to be last, and evaluation right before
		// selection. 
		operators.push_back(new PointMutation(pProblem, mu, N));
		operators.push_back(new PointCrossover(pProblem, chi, N, &PRNG));
		operators.push_back(new Evaluate(pProblem, N, pFitnesses));
		operators.push_back(new TournamentSelection(pProblem, N, k, pFitnesses,
			&PRNG));

		//////////////////////////////////////////////////////////////////////
		//// execute GA as in uni-process
		//////////////////////////////////////////////////////////////////////

		// do the GA! For each generation,
		for (int i = 0; i < G; i++) {

			// ... perform each operation on the population
			for (Operator* op : operators) {
				op->operate(ppPop);
			}

			// if it's a migration iteration
			if (i % migration == 0) {

				// find best fitness and associated chromosome
				// find worst fitness and associated chromosome
				double maxFitness = -numeric_limits<double>::max();
				double* pBestIndividual = 0;
				double minFitness = numeric_limits<double>::max();
				double* pWorstIndividual = 0;
				for (int j = 0; j < N; j++) {
					if (pFitnesses[j] > maxFitness) {
						maxFitness = pFitnesses[j];
						pBestIndividual = ppPop[j];
						bestIndividualLocation = j;
					}

					if (pFitnesses[j] < minFitness) {
						minFitness = pFitnesses[j];
						pWorstIndividual = ppPop[j];
						worstIndividualLocation = j;
					}
				}

				// send to next processor (or if it's proc last send to proc 1
				int who = 1;
				if (rank != nProcs - 1) who = rank + 1;
				MPI_Send(pBestIndividual, chromosomesLength, MPI_DOUBLE, who, TAG_MIG_INDV_BEST, MPI_COMM_WORLD);
				MPI_Send(pWorstIndividual, chromosomesLength, MPI_DOUBLE, who, TAG_MIG_INDV_WORST, MPI_COMM_WORLD);
				MPI_Send(&maxFitness, 1, MPI_DOUBLE, who, TAG_MIG_FIT_BEST, MPI_COMM_WORLD);
				MPI_Send(&minFitness, 1, MPI_DOUBLE, who, TAG_MIG_FIT_WORST, MPI_COMM_WORLD);

				// receive from previous processor (or if it's proc 1 receive
				// from last processor
				who = nProcs - 1;
				if (rank != 1) who = rank - 1;
				MPI_Recv(ppPop[bestIndividualLocation], chromosomesLength, MPI_DOUBLE,
					who, TAG_MIG_INDV_BEST, MPI_COMM_WORLD, &status);
				MPI_Recv(ppPop[worstIndividualLocation], chromosomesLength, MPI_DOUBLE,
					who, TAG_MIG_INDV_WORST, MPI_COMM_WORLD, &status);
				MPI_Recv(&pFitnesses[bestIndividualLocation], 1, MPI_DOUBLE,
					who, TAG_MIG_FIT_BEST, MPI_COMM_WORLD, &status);
				MPI_Recv(&pFitnesses[worstIndividualLocation], 1, MPI_DOUBLE,
					who, TAG_MIG_FIT_WORST, MPI_COMM_WORLD, &status);

			}

		}

		// find best-in-processor fitness and associated chromosome
		// find worst-in-processor fitness and associated chromosome
		double finalMaxFitness = -numeric_limits<double>::max();
		double* pFinalBestIndividual = 0;
		for (int i = 0; i < N; i++) {
			if (pFitnesses[i] > finalMaxFitness) {
				finalMaxFitness = pFitnesses[i];
				pFinalBestIndividual = ppPop[i];
			}
		}

		// send to master
		MPI_Send(&finalMaxFitness, 1, MPI_DOUBLE, 0, TAG_FIT_RESULT, MPI_COMM_WORLD);
		MPI_Send(&chromosomesLength, 1, MPI_INT, 0, TAG_SIZE_RESULT, MPI_COMM_WORLD);
		for (int i = 0; i < chromosomesLength; i++) {
			MPI_Send(&pFinalBestIndividual[i], 1, MPI_DOUBLE, 0, TAG_INDV_RESULT, MPI_COMM_WORLD);
		}


		// free memory
		for (int i = 0; i < N; i++) {
			delete[] ppPop[i];
		}
		delete[] ppPop;
		delete[] pFitnesses;

		for (Operator* op : operators) {
			delete op;
		}

		delete pProblem;

	}

	MPI_Finalize();

	exit(EXIT_SUCCESS);
}

