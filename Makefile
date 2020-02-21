CXX = g++

all : main
	@echo "Compilado corretamente"

main : main.cpp Priority_Queue.h Plane.h
	g++ $^ -o $@

clean: 
	rm main
	rm *.o