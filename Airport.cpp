#include "Airport.h"
#include <fstream>

Airport::Airport()
    : totalVIP(0),
      totalTimeToLand(0),
      totalTimeToDeparture(0),
      totalFuelOnPlanesThatLanded(0),
      totalFuelOnPlanesToLand(0),
      cur_time(0) {}

void Airport::addPlane(Plane *p) {
	if (p->isFlying()) {
		totalFuelOnPlanesToLand += p->getFuel();
		totalTimeToLand += p->getAvgTimeToLeaveQueue();
	} else {
		totalTimeToDeparture += p->getAvgTimeToLeaveQueue();
	}

	if (p->isVIP()) totalVIP++;
	queue.insert(p);
}

void Airport::update() {
	cur_time++;
	int numPlanes = rand() % (k + 1);
	for (int i = 0; i < numPlanes; i++) {
		Plane *p = createRandomPlane();
		addPlane(p);
	}
}

void Airport::showWaitingPlanes() {
	std::ofstream file("temp.txt");
	if (file.is_open()) {
		file << "Exibindo conteúdo da fila:" << std::endl;
		queue.debug(file);
		file.close();
		system("less temp.txt");
		system("rm temp.txt");
	} else
		std::cerr << "Não foi possível imprimir" << std::endl;
}