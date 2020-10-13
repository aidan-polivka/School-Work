#pragma once

#include "Problem.h"

/**
* Sample GA application: Ackley's function, using double chromosomes.
* Maximize the function
*
* f(x, y) = 20 e^{-0.2 \sqrt{1/2 (x^2 + y^2)}} + 
*	e^{1/2 (cos(2 pi x) + cos(2 pi y))} - e - 20
*
* (This is the negation of the standard Ackley's function, since this
* GA maximizes instead of minimizes.)
*
* Maximum is 0, at (0, 0).
*
* \author Mark M. Meysenburg
* \version 04/20/2020
*/
class Ackley : public Problem
{
public:
	Ackley(std::mt19937* pRNG);
	~Ackley();
	double* createChromosome();
	double evaluateChromosome(double* pChrom);
	double getRandomGene();
	int getRandomGeneLoc();
};