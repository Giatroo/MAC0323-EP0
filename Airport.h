/* Arquivo para a classe do aeroporto, que contêm as pistas e os
 * métodos para mostrar, a cada momento, como estão as pistas.
 */

#ifndef _AIRPORT_
#define _AIRPORT_

#include "Plane.h"
#include "Pointer_Priority_Queue.h"

class Airport {
  private:
	int totalTimeToDeparture;
	int totalTimeToLand;
	int totalVIP;
	int totalFuelOnPlanesToLand;
	int totalFuelOnPlanesThatLanded;
	Plane *runway[3];
	Pointer_Priority_Queue<Plane *> queue;
	int cur_time;
	int k; // Máximo de aviões que podem aparecer por unidade de tempo

  public:
	Airport();
	void addPlane(Plane *p); // Adiciona um avião na fila
	void showRegisters();
	void showWaitingPlanes();
	void showAllExpectedTimes();
	void showAvgTimeToDeparture();
	void showAvgTimeToLand();
	void showAvgFuelOnPlanesWaitingToLand();
	void showAvgFuelOnPlanesThatLanded();
	void showQntOfVeryImportantPlanes();
	void update(); // Método que atualiza um na unidade de tempo e atualiza tudo
};

#endif