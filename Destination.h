/* Arquivo dedicado para a classe dos destinos */

#ifndef _DESTINATION_H_
#define _DESTINATION_H_

#include <string>

#define MAX_DESTINATIONS 1000

std::string const DESTIONATIONS_CFG_PATH = "cfg//Destinations.cfg";

class Destination {
  public:
	std::string name; // Sigla com três letras do nome do destino
	int distance;     // Distância do destino até nosso aeroporto

	// Construtor padrão
	Destination();

	// Contrutor já com nome e distância
	Destination(std::string t_name, int t_distance);
};

// Função que deve ser chamada para criar todos os destinos possíveis
// Ela lê um arquivo com eles e os coloca no array 'dests'
void initDestinations();

// Array com todos os destinos possíveis
extern Destination existingDests[MAX_DESTINATIONS];

// Número de destinos possíveis
extern int numExistingDest;

#endif