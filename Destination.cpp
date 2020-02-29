#include <fstream>
#include <iostream>

#include "Destination.h"
#include "MyException.h"

using namespace std;

// Array com todos os destinos possíveis
Destination existingDests[MAX_DESTINATIONS];

// Número de destinos possíveis
int numExistingDest;

Destination::Destination(string t_name, int t_distance)
    : name(t_name), distance(t_distance) {}

Destination::Destination() : name(""), distance(0) {}

string Destination::getName() { return this->name; }
int Destination::getDistance() { return this->distance; }

void initDestinations() {
	string destName;
	int destDist;

	// input file stream do arquivo .cfg com informações sobre os destinos
	ifstream cfg(DESTIONATIONS_CFG_PATH);

	// Se não foi possível abrir, jogando uma exceção
	if (!cfg.is_open())
		throw MyException(ErrorTypes[FILE_NOT_FOUND], "initDestinations",
		                  "File " + DESTIONATIONS_CFG_PATH + " could not be open.");

	// Começamos o número de destinos de 0 e vamos lendos eles
	// ou até acabarem ou até o limite ser atingido
	numExistingDest = 0;
	while (cfg >> destName >> destDist && numExistingDest < MAX_DESTINATIONS)
		existingDests[numExistingDest++] = Destination(destName, destDist);

	// Se não chegamos em um EndOfFile é porque o número máximo de destinos foi
	// atingido
	if (!cfg.eof()) std::cerr << "Maximum number of destinations reached!" << endl;
}