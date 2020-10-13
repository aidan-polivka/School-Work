#pragma once

#include <random>

/**
 * Abstract base class for problems to be solved with a simple genetic 
 * algorithm. Problems inheriting from this class know how to: 
 *		- make complete chromosomes representing potential soluitons to the 
 *			problem at hand
 *		- evaluate a chromosome's fitness as a solution to the problem (where
 *			a higher value is more fit)
 *		- make a single, random gene value for a chromosome
 *		- pick a random location in a chromosome 
 *
 * \author Mark M. Meysenburg
 * \version 03/13/2020
 */
class Problem
{
public:
	/**
	 * Constructor. Create a problem that produces chromosomes of the 
	 * specified length, using the specified PRNG.
	 * 
	 * \param len Length of the chromosomes used in solution representations for
	 *				this Problem
	 * \param pRNG Pointer to the pseudo-random number generator used by this
	 *				Problem
	 */
	Problem(int len, std::mt19937* pRNG) : length(len), pPRNG(pRNG),
		pGeneDist(0), pGeneLocDist(0) 
	{
		pPercentageDist = new std::uniform_real_distribution<double>(0.0, 1.0);
	}

	/**
	 * Destructor. Subclasses should be sure to delete pointers for any 
	 * dynamically-allocated objects.
	 */
	virtual ~Problem() { if (pPercentageDist != 0) delete pPercentageDist; }

	/**
	 * Function to create random chromosomes representing solutions to the 
	 * Problem. 
	 *
	 * \return An array of random doubles representing a solution to the 
	 *			Problem. 
	 */
	virtual double* createChromosome() = 0;

	/**
	 * Function to evaluate the fitness of a chromosome representing a solution
	 * to the Problem.
	 *
	 * \param pChrom Array of doubles representing a solution to this Problem.
	 * \return The fitness of the chromosome; higher is better.
	 */
	virtual double evaluateChromosome(double* pChrom) = 0;

	/**
	 * Get the length of chromosomes representing solutions to this Problem.
	 *
	 * \return Chromosome length
	 */
	int getLength() const { return length; }

	/**
	 * Get a single random gene for a chromosome representing a solution to the
	 * Problem.
	 *
	 * \return A single, random double-valued gene
	 */
	virtual double getRandomGene() = 0;

	/**
	 * Get a random gene location for chromosomes representing a solution to the
	 * Problem.
	 *
	 * \return A single, randm int value in the range [0, length - 1], where 
	 *			length is the number of genes in a chromosome. 
	 */
	virtual int getRandomGeneLoc() = 0;

	/**
	 * Get a random percentage value in [0, 1].
	 *
	 * \return Random percentage value.
	 */
	double getRandomPercentage() { return (*pPercentageDist)(*pPRNG); }

protected:
	/** Size of a chromosome for this Problem. */
	int length;

	/** Pseudo-random number generator used by this Problem. */
	std::mt19937* pPRNG;

	/** Uniform distribution for creating random genes. */
	std::uniform_real_distribution<double>* pGeneDist;

	/** Uniform distribution for creating random gene locations. */
	std::uniform_int_distribution<int>* pGeneLocDist;

	/** Uniform distribution for creating random percentages. */
	std::uniform_real_distribution<double>* pPercentageDist;
};