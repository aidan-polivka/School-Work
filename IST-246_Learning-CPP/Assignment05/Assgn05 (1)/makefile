OBJS	= MTTSearch.o
SOURCE	= MTTSearch.cpp
HEADER	= 
OUT	= MTTSearch
CC	 = g++
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

MTTSearch.o: MTTSearch.cpp
	$(CC) $(FLAGS) MTTSearch.cpp 


clean:
	rm *.o all