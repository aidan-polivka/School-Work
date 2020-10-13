#include "PointCrossover.h"

///////////////////////////////////////////////////////////////////////////////
// PointCrossover function implementations
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// operate implementation
///////////////////////////////////////////////////////////////////////////////
void PointCrossover::operate(double** ppPop)
{
	for (int i = 0; i < popSize; i++)
	{
		// each population member undergoes crossover with probability chi
		if (pProblem->getRandomPercentage() <= chi)
		{
			// if we're doing crossover, pick a mate and a crossover point 
			// randomly
			int mateIdx = (*pMateDist)(*pPRNG);
			int xOverPoint = pProblem->getRandomGeneLoc();
			int len = pProblem->getLength();
			double d;

			// then swap genes between the current individual and the mate,
			// starting at the crossover point
			for (int j = xOverPoint; j < len; j++)
			{
				d = ppPop[i][j];
				ppPop[i][j] = ppPop[mateIdx][j];
				ppPop[mateIdx][j] = d;
			} // swapping loop
		} // perform crossover if
	} // for each individual loop
}