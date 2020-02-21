all : main
	@echo "Compilado corretamente"

main: Priority_Queue.h
	g++ main.cpp -o main

clean: 
	rm main
	rm *.o