/* Arquivo para a classe do aeroporto, que contêm as pistas e os
 * métodos para mostrar, a cada momento, como estão as pistas.
 */

#ifndef _AIRPORT_
#define _AIRPORT_

#include "Plane.h"
#include "Queue.h"

/* A classe para o aeroporto.
 *
 * O primeiro ponto a considerar é que usamos uma fila para cada pista
 * A pista 3 (de índice 2) é a exclusiva para decolagens.
 *
 * Além disso, temos variáveis para guardar uma série de informações
 * sobre cada pista.
 *
 * Por fim, temos as variáveis que dizem respeito à simulação.
 *
 */
class Airport {
  private:
	// Pointer_Queue<Plane *> queue[3]; // As filas para cada pista
	int timeToBeFree[3]; // Tempo até que a i-ésima pista fique livre

	// Nos arrays a baixo, cada índice se refere a uma fila
	int totalTimeToDeparture[3];    // Soma de tempos estimados para decolagem
	int totalTimeToLand[3];         // Soma de tempos estimados para pouco
	int totalVIP[3];                // Número de VIPs por pista
	int totalFuelOnPlanesToLand[3]; // Combustíveis nos aviões que estão
	                                // esperando (por pista)
	int VIPWaitingTime[3];          // Retorna o tempo que um avião VIP terá
	                                // que esperar caso seja colocado na fila i
	int totalFuelOnPlanesThatLanded; // Soma dos combustíveis nos aviões que pousaram

	int cur_time; // Tempo atual da simulação
	int tot_time; // Total de tempo da simulação
	int k;        // Máximo de aviões que podem
	              // aparecer por unidade de tempo

  public:
	Pointer_Queue<Plane *> queue[3]; // As filas para cada pista

	Airport(int t_tot_time, int t_k);
	void addPlane(Plane *p);       // Adiciona um avião na fila
	void removePlane(int t_index); // Remove um avião da fila t_index
	                               // se a pista t_index está disponível
	void showRegisters();
	void showWaitingPlanes();
	void showAllExpectedTimes();
	void showAvgTimeToDeparture();
	void showAvgTimeToLand();
	void showAvgFuelOnPlanesWaitingToLand();
	void showAvgFuelOnPlanesThatLanded();
	void showQntOfVeryImportantPlanes();
	void update(); // Método que deve ser chamado a cada unidade de tempo passada
};

#endif