all: semaphore
semaphore: semaphore.o
	g++ -pthread semaphore.o -o run
semaphore.o: semaphore.cpp
	g++ -c semaphore.cpp