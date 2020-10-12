#include <iostream>
using namespace std

int recur(int param[n], int d) {
    
    
}

int iter(int param[n], int d) {
    int count = 0;
    int cursor = n;
    int first = 0;
    int middle = (first + last)/2;
    int last = cursor - 1;
    
    while(first <= last){
        if(param[middle] < d){
            first = middle + 1;
            count++;
        }
        if(param[middle] > d){
            last = middle - 1;
            count++;
        }
        if(param[middle] == d){
            count++;
            return count;
        }
        if(first > last){
            count++;
            return count;
        }
    }
}