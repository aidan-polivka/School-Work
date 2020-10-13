#include <limits>
#include "TournamentSelection.h"

///////////////////////////////////////////////////////////////////////////////
// TournamentSelection function implementations
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// operate implementation
///////////////////////////////////////////////////////////////////////////////
void TournamentSelection::operate(double** ppPop) 
{
	int chromLen = pProblem->getLength();

	// make sure most fit individual survives
	double maxFitness = -std::numeric_limits<double>::max();
	int maxFitIdx = popSize + 1;
	double fitness;
	for (int i = 0; i < popSize; i++)
	{
		fitness = pFitnesses[i];
		if (fitness > maxFitness)
		{
			maxFitness = fitness;
			maxFitIdx = i;
		}
	}
	for (int i = 0; i < chromLen; i++)
	{
		ppTempPop[0][i] = ppPop[maxFitIdx][i];
	}
	pTempFitnesses[0] = maxFitness;

	// hold a tournament for the rest of the new population
	for (int i = 1; i < popSize; i++)
	{
		int participantIdx;
		maxFitness = -std::numeric_limits<double>::max();
		maxFitIdx = popSize + 1;
		for (int j = 0; j < k; j++)
		{
			participantIdx = (*pParticipantDist)(*pPRNG);
			fitness = pFitnesses[participantIdx];
			if (fitness > maxFitness)
			{
				maxFitness = fitness;
				maxFitIdx = participantIdx;
			}
		} // tournament for

		for (int j = 0; j < chromLen; j++)
		{
			ppTempPop[i][j] = ppPop[maxFitIdx][j];
		}
		pTempFitnesses[i] = maxFitness;
	} // selection for

	// copy selected individuals back to the original population array
	for (int i = 0; i < popSize; i++)
	{
		for (int j = 0; j < chromLen; j++)
		{
			ppPop[i][j] = ppTempPop[i][j];
		}
		pFitnesses[i] = pTempFitnesses[i];
	}
}