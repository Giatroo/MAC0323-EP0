/* Arquivo dedicado à classe de companhias de avião */

#ifndef _AIR_COMPANY_H_
#define _AIR_COMPANY_H_

#include <string>
#include "Destination.h"

#define MAX_COMPANIES 100

std::string const COMPANIES_CFG_PATH = "cfg//AirCompanies.cfg";

class AirCompany {
  private:
	// Nome da companhia aérea
	std::string name;

	// Todos os possíveis destinos que essa companhia faz
	// Vamos ter um pointeiro para o array de todos os destinos existentes
	Destination *destinations[MAX_DESTINATIONS];

	// O número de destinos que a companhia tem
	int numDestinations;

  public:
	// Construtor padrão
	AirCompany();

	// Construtor onde o nome será obrigatório, mas não o número de destinos
	// Se o número de destinos for informado,
	// a companhia será inicializada comdestinos aleatórios
	AirCompany(std::string t_name, int t_numDestinations = 0);

	// Método para adiocionar
	void addDestination(int t_dest);

	// Métodos de acesso
	std::string getName();
	int getNumDestinations();
	Destination *getDestination(int i);
};

// Todas as companhias aéras existentes
extern AirCompany existingCompanies[MAX_COMPANIES];

// O número de companhias existentes
extern int numExistingCompanies;

void initAirCompanies();

#endif