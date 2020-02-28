#include "AirCompany.h"
#include "Destination.h"
#include "MyException.h"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <set>

using namespace std;

// Todas as companhias aéras existentes
AirCompany existingCompanies[MAX_COMPANIES];

// O número de companhias existentes
int numExistingCompanies;

AirCompany::AirCompany() : name(""), numDestinations(0) {}

AirCompany::AirCompany(string t_name, int t_numDestinations) {
	this->name = t_name;
	// Deve ser o mínimo entre o número informado e
	// o máximo de destinos que existem
	int tot_destinations = min(t_numDestinations, numExistingDest);
	numDestinations = 0; // Começa de zero e vamos incrementando
	                     // conforme adicionamos novos destinos

	// Crio um conjunto de números que armazenará os números já sorteados
	set<int> s;

	while (s.size() < tot_destinations) {
		int random = rand() % numExistingDest;

		// Se eu já sorteei anteriormente o número, eu continuo
		if (s.find(random) != s.end()) continue;

		// Se não sorteei o número, adiciono esse destino aos destinos
		// da minha companhia
		this->addDestination(random);
		// e coloco o número na lista de números sorteados
		s.insert(random);
	}
}

void AirCompany::addDestination(int t_dist) {
	if (t_dist < 0 || t_dist > numExistingDest)
		throw MyException(ErrorTypes[INDEX_OUT_OF_BOUNDS], "AirCompany::addDestination");

	this->destinations[numDestinations++] = &existingDests[t_dist];
}

std::string AirCompany::getName() { return this->name; }
int AirCompany::getNumDestinations() { return this->numDestinations; }
Destination *AirCompany::getDestination(int i) {
	if (i < 0 || i > this->numDestinations)
		throw MyException(ErrorTypes[INDEX_OUT_OF_BOUNDS], "AirCompany::addDestination");

	return this->destinations[i];
}

void initAirCompanies() {
	string name;
	int numDestinations;

	// input file stream do arquivo .cfg com informações sobre as companias
	ifstream cfg(COMPANIES_CFG_PATH);

	if (!cfg.is_open()) throw MyException(ErrorTypes[FILE_NOT_FOUND], "initAirCompanies");

	while (cfg >> name >> numDestinations && numExistingCompanies < MAX_COMPANIES)
		existingCompanies[numExistingCompanies++] = AirCompany(name, numDestinations);

	// Se não chegamos em um EndOfFile é porque o número máximo de destinos foi
	// atingido
	if (!cfg.eof()) std::cerr << "Maximum number of air companies reached!" << endl;
}
