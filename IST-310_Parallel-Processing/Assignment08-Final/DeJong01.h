#pragma once

#include "Problem.h"

/**
* Sample GA application: DeJong's first function, using double chromosomes.
* Maximize the function
*
* f_1 = -\sum_{i = 1}^{3} x_{i}^{2}, -5.12 \leq x_{i} 5.12
*
* (This is the negation of the standard DeJong's first function, since this
* GA maximizes instead of minimizes.)
*
* Maximum is 0, at (0, 0, 0).
*
* \author Mark M. Meysenburg
* \version 03/13/2020
*/
class DeJong01 : public Problem
{
public:
	DeJong01(std::mt19937* pRNG);
	~DeJong01();
	double* createChromosome();
	double evaluateChromosome(double* pChrom);
	double getRandomGene();
	int getRandomGeneLoc();
};