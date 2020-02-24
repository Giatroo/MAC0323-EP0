CC = g++
OBJ = ./obj

all : main
	mv *.o ${OBJ}
	mv *.gch ${OBJ}
	@echo "Compilado corretamente"

main : main.cpp Priority_Queue.h Plane.o Destination.o AirCompany.o
	${CC} $^ -o $@

Plane.o : Plane.cpp Plane.h
	${CC} $^ -c $<

AirCompany.o : AirCompany.cpp AirCompany.h Destination.h
	${CC} $^ -c $< 

Destination.o : Destination.cpp Destination.h
	${CC} $^ -c $< 

clean: 
	rm main
	rm *.o