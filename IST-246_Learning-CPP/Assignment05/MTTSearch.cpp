// Include Statements
#include <cstdlib>      //
#include <algorithm>    // Used for STL Sorting
#include <chrono>       // Used for Seeding PRNG
#include <iostream>     //
#include <random>       // Used for Mersenne Twister PRNG


//This is our recursive Binary Search.   
int recur(int *parr, int low, int high, int d){
      
    int mid = ((low + high) /2);
    if(parr[mid] == d || high < low){
      return 1;
    }
    
    else if(parr[mid] > d){
      return 1 + recur(parr, low, mid-1, d);
    }
   
    else {
      low = mid;
      return 1 + recur(parr, low + 1, high, d);
    }
}
    
//This is our Iterative Binary Search.
int iter(int *parr, int low, int high, int d){    
  int mid = 0;
  int i = 0;
  while(high>=low){
    mid = (low+high)/2;
    i += 1;

    if (parr[mid] > d){
      high = mid - 1;
      i += 1;
    }

    else if(parr[mid] < d){
      low = mid + 1;
      i += 1;
    }
    
    else{ 
      i += 1;
    }
  }
return i;
}


int main(){
  using namespace std;
  
  //First, we will make our Random Number Generator.
  mt19937 prng(chrono::system_clock::now().time_since_epoch().count());

  //print header
  cout<<"n"<<"\t" << "\t"  <<"iter"<<"\t" << "\t" <<"recur"<<"\t" << "\t" <<"log(n)"<<endl;
  
  //begin for loop, starting at n - 64, var = 6
  for(int n = 64; n <= 134217728; n *= 2){

    //important variables
    double recursivecomp = 0;
    double iterativecomp = 0;
    //int n = 2^i;
    int *pA = new int[n];
    int a = 0;
    int b = 0;
    double val = ceil(log2(n));

    // Next, we'll fill our array with random numbers
    for(int j = 0; j< n; j++) {
      pA[j] = prng();    
    }

    //sort array used in program
    sort(pA, pA+n);     

    //Tcall binary searches
    //We will perform each 1000 times, and count the number of comparisons done by each.
    for(int k = 0; k < 1000; k++){
      a = recur(pA, 0, n - 1, prng());
      recursivecomp+=a;
    }
    for(int l = 0; l < 1000; l++){
      b= iter(pA, 0, n - 1, prng());
      iterativecomp+=b;
    }
    recursivecomp = recursivecomp/1000;
    iterativecomp = iterativecomp/1000;

    //output data
    cout<< n << "\t" << "\t" << iterativecomp << "\t" << "\t" <<recursivecomp<< "\t" << "\t" << val <<endl;

    //free memory by deleting array
    delete [] pA;
  }
}