CC = g++
OBJ = ./obj

all : main
	mv *.o ${OBJ}
	mv *.gch ${OBJ}
	@echo "Compilado corretamente"

main : main.cpp Priority_Queue.h Queue.h Plane.o Destination.o AirCompany.o Airport.o MyException.o
	${CC} $^ -o $@

Plane.o : Plane.cpp Plane.h MyException.h
	${CC} $^ -c $<

AirCompany.o : AirCompany.cpp AirCompany.h Destination.h MyException.h
	${CC} $^ -c $< 

Destination.o : Destination.cpp Destination.h MyException.h
	${CC} $^ -c $< 

Airport.o : Airport.cpp Airport.h MyException.h
	${CC} $^ -c $< 

MyException.o : MyException.cpp MyException.h
	${CC} $^ -c $< 

clean: 
	rm main
	rm ${OBJ}/*.o
	rm ${OBJ}/*.gch