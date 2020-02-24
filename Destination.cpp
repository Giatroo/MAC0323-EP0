#include <fstream>
#include <iostream>

#include "Destination.h"

using namespace std;

// Array com todos os destinos possíveis
Destination existingDests[MAX_DESTINATIONS];

// Número de destinos possíveis
int numExistingDest;

Destination::Destination(string t_name, int t_distance)
    : name(t_name), distance(t_distance) {}

Destination::Destination() : name(""), distance(0) {}

void initDestinations() {
	string destName;
	int destDist;

	// input file stream do arquivo .cfg com informações sobre os destinos
	ifstream cfg(DESTIONATIONS_CFG_PATH);

	// Se não foi possível abrir, jogando uma exceção
	if (!cfg.is_open()) throw "initDestinations: File could not be open!\n";

	// Começamos o número de destinos de 0 e vamos lendos eles
	// ou até acabarem ou até o limite ser atingido
	numExistingDest = 0;
	while (cfg >> destName >> destDist && numExistingDest < MAX_DESTINATIONS) {
		existingDests[numExistingDest].name = destName;
		existingDests[numExistingDest].distance = destDist;
		numExistingDest++;
	}

	// Se não chegamos em um EndOfFile é porque o número máximo de destinos foi
	// atingido
	if (!cfg.eof())
		std::cerr << "Maximum number of destinations reached!" << endl;
}