#include <cstdlib>
#include <ctime>
#include <iostream>
#include "SLL.h"

int main() {
    using namespace std;
    srand(time(0));
    
    //create the two individual lists
    SLL<int> list_L;
    SLL<int> list_M;
    SLL<int> list_LM;
    

    
  //Create test arrays
  int case1_L [5] = {32,85,80,83,77};
  int case1_M [5] = {136,113,166,105,171};  
  
  int case2_L[5] = {61,61,44,47,22};
  int case2_M[7] = {148,157,153,138,197,150,125};
  
  int case3_L[6] = {63,92,95,13,24,84};
  int case3_M[5] = {138,123,118,196,198};
  
//Case 1
  cout << "Case 1: \n";
  
  //add elements to the lists
  for (int ints : case1_L){
    list_L.add(ints);
  }
    for (int ints : case1_M){
    list_M.add(ints);
  }
    
    //output lists
    cout << list_L << endl;
    cout << list_M << endl; 
    
    list_LM.merge(list_L,list_M);
    cout << list_LM << endl;
    
    //clear lists
    list_L.clear();
    list_M.clear();
    
    
    
//Case 2
  cout << "Case 2: \n";
  
  //add elements to the lists
  for (int ints : case2_L){
    list_L.add(ints);
  }
    for (int ints : case2_M){
    list_M.add(ints);
  }
    
    //output lists
    cout << list_L << endl;
    cout << list_M << endl; 
    
    list_LM.merge(list_L,list_M);
    cout << list_LM << endl;

    //clear lists
    list_L.clear();
    list_M.clear();
    
//Case 3
  cout << "Case 3: \n";
  
  //add elements to the lists
  for (int ints : case3_L){
    list_L.add(ints);
  }
    for (int ints : case3_M){
    list_M.add(ints);
  }
    
    //output lists
    cout << list_L << endl;
    cout << list_M << endl; 
    
    list_LM.merge(list_L,list_M);
    cout << list_LM << endl;
    
    //clear lists
    list_L.clear();
    list_M.clear();    
    list_LM.clear();
    
    return EXIT_SUCCESS;
}