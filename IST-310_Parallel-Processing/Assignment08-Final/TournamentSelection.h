#pragma once

#include <random>
#include "Operator.h"
#include "Problem.h"

/**
 * Subclass of Operator to perform elitist tournament selection on the
 * population in a simple genetic algorithm.
 *
 * \author Mark M. Meysenburg
 * \version 03/13/2020
 */
class TournamentSelection : public Operator
{
public:
	/**
	 * Constructor. Create this TournamentSelection Operator, composing it with
	 * the Problem being solved, population size, tournament size, fitnesses
	 * array, and pseudo-random number generator being used by the genetic
	 * algorithm.
	 *
	 * \param pProb Pointer to the Problem being solved.
	 * \param popSize Population size.
	 * \param tournSize Number of individuals participating in tournaments.
	 * \param pFits Pointer to array of doubles, of same size as the population,
	 *          holding the fitnesses of each individual in the population.
	 * \param pRNG Pseudo-random number generator being used by the genetic
	 *			algorithm.
	 */
	TournamentSelection(Problem* pProb, int popSize, int tournSize,
		double* pFits, std::mt19937* pRNG) :
		Operator(pProb, popSize), k(tournSize), pFitnesses(pFits), pPRNG(pRNG)
	{
		// allocate distribution used to select tournament participants
		pParticipantDist = new std::uniform_int_distribution<int>(0,
			popSize - 1);

		// allocate temporary population 
		ppTempPop = new double* [popSize];
		for (int i = 0; i < popSize; i++) {
			ppTempPop[i] = new double[pProb->getLength()];
		}

		// allocate temporary fitnesses
		pTempFitnesses = new double[popSize];
	}

	/**
	 * Destructor. Free the distribution used to select tournament 
	 * participants, the temporary population, and the temporary fitnesses.
	 */
	~TournamentSelection() {
		if (pParticipantDist != 0) delete pParticipantDist;

		if (ppTempPop != 0) {
			for (int i = 0; i < popSize; i++) {
				delete[] ppTempPop[i];
			}
			delete[] ppTempPop;
		}

		if (pTempFitnesses != 0) {
			delete[] pTempFitnesses;
		}
	}

	/**
	 * Perform the elitist tournament selection operation. First, make sure at
	 * least one copy of the most-fit individual survives into the next
	 * generation. Then, for each remaining slot in the next generation,
	 * perform a tournament of size k: select k individuals at random; the one
	 * with the highest fitness is copied into the next generation.
	 *
	 * \param ppPop Pointer to a two-dimensional array of doubles,
	 *          representing the individuals in the genetic algorithm
	 *          population.
	 */
	void operate(double** ppPop);

private:
	double** ppTempPop;
	double* pTempFitnesses;
	int k;
	double* pFitnesses;
	std::mt19937* pPRNG;
	std::uniform_int_distribution<int>* pParticipantDist;
};