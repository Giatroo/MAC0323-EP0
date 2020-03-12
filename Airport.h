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

	// Iterador que aponta para o último VIP em cada fila
	// Se não há VIPs, aponta para o começo da fila (front)
	Queue_Iterator<Plane *> lastVIP[3];

	// Nos arrays a baixo, cada índice se refere a uma fila
	int totalTimeToDeparture[3];    // Soma de tempos estimados para decolagem
	int totalTimeToLand[3];         // Soma de tempos estimados para pouco
	int totalVIP[3];                // Número de VIPs por pista
	int totalFuelOnPlanesToLand[3]; // Combustíveis nos aviões que estão
	                                // esperando (por pista)
	int totalFuelOnPlanesThatLanded; // Soma dos combustíveis nos aviões que pousaram
	int totalLendedPlanes;           // Total de aviões pousados

	int cur_time; // Tempo atual da simulação
	int tot_time; // Total de tempo da simulação
	int k;        // Máximo de aviões que podem
	              // aparecer por unidade de tempo

	void addNonVIP(Plane *p); // Adiciona um avião que não é VIP
	void addVIP(Plane *p);    // Adiciona um avião que é VIP

  public:
	Pointer_Queue<Plane *> queue[3]; // As filas para cada pista
	Airport(int t_tot_time, int t_k);
	void addPlane(Plane *p);       // Adiciona um avião na fila
	void removePlane(int t_index); // Remove um avião da fila t_index
	                               // se a pista t_index está disponível
	void showWaitingPlanes();      // Mostra as filas
	void showAllExpectedTimes();   // Imprime todos os tempos esperados para pousar
	void showAvgTimeToDeparture(); // Mostra a média de todos os tempos para decorar
	void showAvgTimeToLand();      // Mostra a média dos tempos para pousar
	void showAvgFuelOnPlanesWaitingToLand(); // Mostra a média do combustível nos
	                                         // aviões esperando para pousar
	void showAvgFuelOnPlanesThatLanded();    // Mostra a média do combustível
	                                         // nos aviões que já pousaram
	void showQntOfVeryImportantPlanes();     // Mostra quantos VIPs há
	                                         // esperando para pousar
	void update(); // Método que deve ser chamado a cada unidade de tempo passada

	bool simulating(); // Retorna se a simulação deve continuar ou não
	                   // (se cur_time < tot_time)

	int getCurTime(); // Retorna o tempo atual da simulação
};

#endif