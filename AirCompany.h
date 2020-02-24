/* Arquivo dedicado à classe de companhias de avião */

#ifndef _AIR_COMPANY_H_
#define _AIR_COMPANY_H_

#include <string>
#include "Destination.h"

class AirCompany {
  private:
	// Nome da companhia aérea
	std::string name;

	// Todos os possíveis destinos que essa companhia faz
	Destination destinations[MAX_DESTINATIONS];

	// O número de destinos que a companhia tem
	int numDestinations;

  public:
	// Construtor onde o nome será obrigatório, mas não o número de destinos
	// Se o número de destinos for informado,
	// a companhia será inicializada comdestinos aleatórios
	AirCompany(std::string name, int numDestinations = 0);

	// Método para adiocionar
	void addDestination(Destination d);
};

#endif