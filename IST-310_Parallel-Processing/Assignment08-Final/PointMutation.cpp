#include "PointMutation.h"

///////////////////////////////////////////////////////////////////////////////
// PointMutation function implementations
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// operate function
///////////////////////////////////////////////////////////////////////////////
void PointMutation::operate(double** ppPop)
{
	for (int i = 0; i < popSize; i++)
	{
		// each individual undergoes mutation with probability mu
		if (pProblem->getRandomPercentage() <= mu)
		{
			// if mutation happens, pick a random location and replace that 
			// gene with a randomly-selected value. 
			int idx = pProblem->getRandomGeneLoc();
			ppPop[i][idx] = pProblem->getRandomGene();
		} // if mutation happens
	} // for each individual 
}