#include <cstdio>
#include <limits>
#include "StandardStats.h"

///////////////////////////////////////////////////////////////////////////////
// StandardStats function implementations
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// operate implementation
///////////////////////////////////////////////////////////////////////////////
void StandardStats::operate(double** ppPop)
{
	double min = std::numeric_limits<double>::max();
	double max = -min;
	double mean = 0.0;
	for (int i = 0; i < popSize; i++)
	{
		// track min, max; accumulate sum of fitnesses
		if (pFitnesses[i] < min) min = pFitnesses[i];
		if (pFitnesses[i] > max) max = pFitnesses[i];
		mean += pFitnesses[i];
	}
	// calculate mean and print results to standard output
	mean /= popSize;
	printf("%f,%f,%f\n", min, max, mean);
}