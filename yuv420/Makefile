CC = g++
CCFLAG = -std=c++11
ARCHIVES = main

build: main.o
	$(CC) $(CCFLAG) $^ -o $(ARCHIVES)

main.o: main.cpp
	$(CC) $(CCFLAG) -c $^

clean:
	-rm -rf *.o
	-rm -rf *.out
	-rm -rf $(ARCHIVES)
	-rm -rf output