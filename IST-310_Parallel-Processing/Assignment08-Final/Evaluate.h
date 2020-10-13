#pragma once

#include "Operator.h"

/**
 * Subclass of Operator, used to evaluate the fitnesses of the individuals in
 * the genetic algorithm's population.
 *
 * \author Mark M. Meysenburg
 * \version 3/13/2020
 */
class Evaluate : public Operator
{
public:
    /**
     * Constructor. Create the Evaluate Operator, composing it with the Problem
     * being solved, population size, and array of doubles holding the 
     * fitnesses.
     *
     * \param pProb Pointer to the Problem being solved.
     * \param popSize Population size.
     * \param pFits Pointer to array of doubles, of same size as the population,
     *          holding the fitnesses of each individual in the population.
     */
	Evaluate(Problem* pProb, int popSize, double* pFits) : 
		Operator(pProb, popSize), pFitnesses(pFits) { }

    /**
     * Perform the evaluation operation on the population. Evaluate the fitness
     * of each individual in the population and save in the fitnesses array.
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