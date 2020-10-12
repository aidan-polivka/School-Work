#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include "IntMinHeap.h"

/**
 * Determine if two arrays of integers, assumed to be of the same size,
 * are equal or not.
 * 
 * \param pA Pointer to the first element of array A
 * \param pB Pointer to the first element of array B
 * \param n Number of elements in both array A and array B
 * \return true if the arrays contain the same values, in the same order; 
 * false otherwise.
 */
bool areEqual(int* pA, int* pB, unsigned n) {
    for(unsigned i = 0; i < n; i++) {
        if(pA[i] != pB[i]) {
            return false;
        }
    }
    return true;
}

/**
 * Application entry point.
 */
int main() {
    using namespace std;
    
    // Mersenne Twister PRNG
    mt19937 prng(chrono::system_clock::now().time_since_epoch().count());
    
    // create two arrays and fill with identical random numbers
    unsigned n = 100000;
    int* pA = new int[n];
    int* pB = new int[n];
    for(unsigned i = 0; i < n; i++) {
        pA[i] = prng() % n;
        pB[i] = pA[i];
    }
    
    // sort array pA using a min-heap
    IntMinHeap heap(pA, n);
    heap.toSortedArray(pA, n);
    
    // sort array pB using the STL sort
    sort(pB, pB + n);
    
    // check to see if the arrays are equal
    cout << "The arrays " <<
        (areEqual(pA, pB, n) ? "are" : "are not") <<
        " equal!" << endl;
    
    // free dynamically allocated memory
    delete [] pA;
    delete [] pB;
    
    return EXIT_SUCCESS;
}