
#include <array>
#include <algorithm>    // for STL sort
#include <chrono>       // for seeding PRNG
#include <iostream>
#include <random>       // for Mersenne Twister PRNG

// A iterative binary search function. It returns location of x in 
// given array arr[l..r] if present, otherwise -1 
int iBinarySearch(int arr[], int l, int r, int x)
{
	while (l <= r)
	{
		int m = l + (r - l) / 2;

		// Check if x is present at mid 
		if (arr[m] == x)
			return m;

		// If x greater, ignore left half   
		if (arr[m] < x)
			l = m + 1;

		// If x is smaller, ignore right half  
		else
			r = m - 1;
	}

	// if we reach here, then element was not present 
	return -1;
}

int rBinarySearch(int arr[], int l, int r, int x)
{
	if (r >= l)
	{
		int mid = l + (r - l) / 2;

		// If the element is present at the middle itself 
		if (arr[mid] == x)  return mid;

		// If element is smaller than mid, then it can only be present 
		// in left subarray 
		if (arr[mid] > x) return rBinarySearch(arr, l, mid - 1, x);

		// Else the element can only be present in right subarray 
		return rBinarySearch(arr, mid + 1, r, x);
	}

	// We reach here when element is not present in array 
	return -1;
}

int main() {
	using namespace std;

	// create Mersenne Twister PRNG
	mt19937 prng(chrono::system_clock::now().time_since_epoch().count());

	int search = prng();

	// array to hold random values
	int n;


	for (n = 6; n < 27; n++) {
		int exp = exp2(n);
		int *arr = new int[exp];
		for (int i = 0; i < exp; i++) {
			arr[i] = prng();
		}

		sort(arr, arr + n);

		rBinarySearch(arr, 0, n, search);
		iBinarySearch(arr, 0, n, search);

		delete[] arr;
	}

	return EXIT_SUCCESS;
}