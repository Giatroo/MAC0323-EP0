/* Arquivo para a classe de um avião. */

#ifndef _PLANE_H_
#define _PLANE_H_

#include <iostream>
#include "AirCompany.h"

#define EMERGENCY -1

// Qual a chance de um avião ser uma emergência em %
#define EMERGENCY_RATE 3

// Máximo de combustível que um avião pode ter
#define MAX_FUEL 200

class Plane {
  private:
	std::string company; // Código de 2 letras para a compania do avião
	int planeNumber;     // Número de três dígitos do avião
	std::string destiny; // Código de 3 letras para o destino do voo
	int fuel;            // Combustível do avião
	int timeWaiting;     // Tempo que o avião está esperando para decolar
	int flyTime;         // Tempo de voo estimado
	bool VIP;            // Very important plane (emergência)
	int priority;        // Número da prioridade do avião
	// Variável que indica se o avião está voando ou se está esperando para
	// decolar
	bool flying;

  public:
	// Construtor padrão
	Plane();

	// Construtor1
	Plane(std::string name, int flyTime, bool VIP, bool flying, int fuel = 0);

	// Construtor2
	Plane(AirCompany airCompany, bool VIP, bool flying, int fuel = 0);

	void setName(std::string name = "");

	// Atualiza uma unidade de tempo para o avião
	void update();

	// Atualiza a prioridade do avião baseando-se em suas características
	void updatePriority();

	// Overload no operador < para determinar se um avião tem mais ou menos
	// prioridade que outro
	// Ao fazer p1 < p2, retorna true se p1.priority < p2.priority e false caso
	// contrário
	bool operator<(Plane &p);
	bool operator<=(Plane &p);
	bool operator>(Plane &p);
	bool operator>=(Plane &p);

	// ostream& operator << (ostream &os);
	friend std::ostream &operator<<(std::ostream &os, Plane &p);
};

// Função que cria um avião randômico e retorna seu endereço
Plane *createRandomPlane();

#endif