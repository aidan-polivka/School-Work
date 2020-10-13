all:	uni-va mpi-va

uni-va:	uni-va.cpp
	g++ -std=c++11 -Wall -O3 uni-va.cpp -o bin/uni-va

mpi-va:	mpi-va.cpp
	mpic++ -std=c++11 -Wall -O3 mpi-va.cpp -o bin/mpi-va.mpi

clean:
	rm bin/uni-va bin/mpi-va.mpi
