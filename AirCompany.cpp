#include <cstdlib>
#include <set>

#include "AirCompany.h"

using namespace std;

AirCompany::AirCompany(string name, int numDestinations) {
	this->name = name;
	// Deve ser o mínimo entre o número informado e
	// o máximo de destinos que existem
	this->numDestinations = min(numDestinations, numExistingDest);

	srand(time(0));
	// Crio um conjunto de números que armazenará os números já sorteados
	set<int> s;

	while (s.size() < numDestinations) {
		int random = rand() % numExistingDest;

		// Se eu já sorteei anteriormente o número, eu continuo
		if (s.find(random) != s.end()) continue;

		// Se não sorteei o número, adiciono esse destino aos destinos
		// da minha companhia
		this->addDestination(existingDests[random]);
	}
}

void AirCompany::addDestination(Destination d) {
	this->destinations[numDestinations].name = d.name;
	this->destinations[numDestinations].distance = d.distance;
	numDestinations++;
}