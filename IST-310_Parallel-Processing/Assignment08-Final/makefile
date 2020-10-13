all:	bin/Ackley bin/DeJong01

bin/Ackley:	bin/Ackley.mpi bin/PointMutation.o bin/Evaluate.o bin/PointCrossover.o bin/TournamentSelection.o
	mpic++ -std=c++11 -Wall -O3 bin/Ackley.mpi bin/PointMutation.o bin/Evaluate.o bin/PointCrossover.o bin/TournamentSelection.o -o bin/Ackley

bin/DeJong01:	bin/DeJong01.mpi bin/PointMutation.o bin/Evaluate.o bin/PointCrossover.o bin/TournamentSelection.o
	mpic++ -std=c++11 -Wall -O3 bin/DeJong01.mpi bin/PointMutation.o bin/Evaluate.o bin/PointCrossover.o bin/TournamentSelection.o -o bin/DeJong01

bin/Ackley.mpi:	Ackley.cpp
	mpic++ -std=c++11 -Wall -c -O3 Ackley.cpp -o bin/Ackley.mpi

bin/DeJong01.mpi:	DeJong01.cpp
	mpic++ -std=c++11 -Wall -c -O3 DeJong01.cpp -o bin/DeJong01.mpi

bin/PointMutation.o:	PointMutation.cpp
	g++ -std=c++11 -Wall -c -O3 PointMutation.cpp -o bin/PointMutation.o

bin/Evaluate.o:	Evaluate.cpp
	g++ -std=c++11 -Wall -c -O3 Evaluate.cpp -o bin/Evaluate.o

bin/PointCrossover.o:	PointCrossover.cpp
	g++ -std=c++11 -Wall -c -O3 PointCrossover.cpp -o bin/PointCrossover.o

bin/TournamentSelection.o:	TournamentSelection.cpp
	g++ -std=c++11 -Wall -c -O3 TournamentSelection.cpp -o bin/TournamentSelection.o

clean:
	rm bin/DeJong01 bin/Ackley bin/*.o
