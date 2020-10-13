#pragma once

#include "Problem.h"

/**
 * Abstract base class for operators in a simple genetic algorithm. Operators
 * inheriting from this class know how to perform their operation (mutation,
 * crossover, selection, etc.) on a two-dimensional array of doubles 
 * representing a population of individuals.
 *
 * \author Mark M. Meysenburg
 * \version 3/13/2020
 */
class Operator
{
public:
    /**
     * Constructor. Create the Operator, composing it with the Problem being 
     * solved and the population size. 
     * 
     * \param pProb Pointer to the Problem being solved; the problem can be 
     *          used to get new genes, pick crossover points, etc.
     *  \param n Population size.
     */
	Operator(Problem* pProb, int n) : pProblem(pProb), popSize(n) { }

    /**
     * Destructor. Subclasses need to have their own destructors if they 
     * do any dynamic memory allocation.
     */
	virtual ~Operator() { }

    /**
     * Abstract operate function. Subclasses implement this function to 
     * do their operation on the population.
     *
     * \param ppPop Pointer to a two-dimensional array of doubles, 
     *          representing the individuals in the genetic algorithm
     *          population.
     */
	virtual void operate(double** ppPop) = 0;

protected:
    /** Pointer to the Problem being solved. */
	Problem* pProblem;

    /** Population size. */
	int popSize;
};