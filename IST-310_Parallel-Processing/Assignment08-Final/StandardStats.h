#pragma once

#include "Operator.h"

/**
 * Subclass of Operator, used to gather simple statistics about the simple GA's
 * population (min, max, average fitness) and print them to the standard 
 * output. Used only if you want generation-by-generation statistics printed to
 * the screen.
 *
 * \author Mark M. Meysenburg
 * \version 03/13/2020
 */
class StandardStats : public Operator
{
public:
    /**
     * Constructor. Create this StandardStats Operator, composing it with the 
     * Problem being solved, the population size, and the fitnesses of the 
     * individuals in the population.
     *
     * \param pProb Pointer to the Problem being solved.
     * \param popSize Population size.
     * \param pFits Pointer to array of doubles, of same size as the population,
     *          holding the fitnesses of each individual in the population.
     */
	StandardStats(Problem* pProb, int popSize, double* pFits) : 
		Operator(pProb, popSize), pFitnesses(pFits) { }

    /**
     * Gather and report statistics.
     *
     * \param ppPop Pointer to a two-dimensional array of doubles,
     *          representing the individuals in the genetic algorithm
     *          population.
     */
	void operate(double** ppPop);

private:
    /**
     * Pointer to the array of doubles, of same size as the population,
     * holding the fitnesses of each individual in the population.
     */
    double* pFitnesses;
};