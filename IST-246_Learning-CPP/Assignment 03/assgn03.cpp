
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "SLL.h"

int main() {
    using namespace std;
    
    // initialize lists L and M with pseudorandom numbers
    srand(time(0));
    SLL<int> L;
    SLL<int> M;
    
    // case 1: L and M have the same number of elements
    cout << "Case 1: " << endl;
    for(int i = 0; i < 5; i++) {
        L.add(rand() % 100);
        M.add(rand() % 100 + 100);
    }
    cout << "\tL:\t" << L << endl;
    cout << "\tM:\t" << M << endl;
    SLL<int> LM1(L, M);
    cout << "\tLM:\t" << LM1 << endl;
    
    // case 2: |L| < |M|
    cout << "\nCase 2: " << endl;
    L.clear();
    M.clear();
    for(int i = 0; i < 5; i++) {
        L.add(rand() % 100);
        M.add(rand() % 100 + 100);
    }
    M.add(rand() % 100 + 100);
    M.add(rand() % 100 + 100);
    cout << "\tL:\t" << L << endl;
    cout << "\tM:\t" << M << endl;
    SLL<int> LM2(L, M);
    cout << "\tLM:\t" << LM2 << endl;
    
    // case 3: |L| < |M|; should cause an exception
    cout << "\nCase 3: " << endl;
    L.clear();
    M.clear();
    for(int i = 0; i < 5; i++) {
        L.add(rand() % 100);
        M.add(rand() % 100 + 100);
    }
    L.add(rand() % 100);
    cout << "\tL:\t" << L << endl;
    cout << "\tM:\t" << M << endl;
    try {
        SLL<int> LM3(L, M);
        cout << "\tLM:\t" << LM3 << endl;
    } catch(out_of_range oor) {
        cout << "\tCannot merge L and M!" << endl;
        cout << "\t" << oor.what() << endl;
    }
    
    
    return EXIT_SUCCESS;
}