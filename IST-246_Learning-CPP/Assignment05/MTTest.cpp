#include <algorithm>    // for STL sort
#include <chrono>       // for seeding PRNG
#include <iostream>
#include <random>       // for Mersenne Twister PRNG

int main() {
    using namespace std;
    
    // create Mersenne Twister PRNG
    mt19937 prng(chrono::system_clock::now().time_since_epoch().count());
    
    //begin printing top of results table
    cout << "n \t" << "iter \t" << "recur \t" << "lg(n) \n";
    
    // instantiate initial array size
    int n = 64;
    
    //create main loop for program
    for (int i = 6; i <=27; i++) {
        
        //instantiate array
        int *pA = new int[n];
        
        //fill array with random values
        for(int j = 0; j < n; j++) {
            pA[j] = prng();
        }
        
        //sort array
        sort(pA, pA + n);
        
        
        //total comparisons made from Iteration binary search
        int sumIter = 0;
        //call iteration binary search function 1000 times
        for(int j = 0; j < 1000; j++) {
            sumIter += iter(pA, prng());
        }
        //Average comparisons made from iteration
        int avgIter = sumIter/1000;
        
        
        //total comparisons made from Recursion binary search
        int sumRecur = 0;
        //call recursion binary search function 1000 times
        for(int j = 0; j < 1000; j++) {
            sumRecur += recur(pA, prng());
        }
        //Average comparisons made from recursion
        int avgRecur = sumRecur/1000;
        
        
        //output n, average comparisons made from iteration, average
        //comparisons made from recursion, and the time complexity.
        cout << n << "\t" << avgIter << "\t" << avgRecur << "\t" << i << "\n";
        
        //delete array
        delete [] pA;
        
        //increase n, 2^i
        n = n*2;
    }
    
    return EXIT_SUCCESS;
}