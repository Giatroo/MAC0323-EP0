#include <fstream>

#include "Airport.h"
#include "Plane.h"

Airport::Airport(int t_tot_time, int t_k)
    : cur_time(0), tot_time(t_tot_time), k(t_k), totalFuelOnPlanesThatLanded(0) {
	for (int i = 0; i < 3; i++) {
		timeToBeFree[i] = 0;
		VIPWaitingTime[i] = 0;
		totalTimeToDeparture[i] = 0;
		totalTimeToLand[i] = 0;
		totalVIP[i] = 0;
		totalFuelOnPlanesToLand[i] = 0;
	}
}

void Airport::addPlane(Plane *p) {
	// Vamos buscar qual é a fila que fará com que esse avião
	// o mínimo possível
	int minWaitingTime = INT32_MAX;
	int insertQueue; // Índice da fila em que vamos inserir o avião

	// Se o avião está para pousar, só olhamos as duas primeiras
	int totQueues = (p->isFlying()) ? 2 : 3;

	for (int i = 0; i < totQueues; i++) {
		if (!queue[i].empty()) {
			if (minWaitingTime > queue[i].getBack()->getAvgTimeToLeaveQueue()) {
				minWaitingTime = queue[i].getBack()->getAvgTimeToLeaveQueue() + 2;
				// +2 por causa do tempo de serviço
				insertQueue = i;
			}
		} else {
			minWaitingTime = cur_time + timeToBeFree[i];
			insertQueue = i;
		}
	}

	if (p->isFlying() && minWaitingTime > p->getFuel()) {
		// Se o avião ficará se combustível durante a espera, precisamos
		// mandar ele embora
		delete p; // Por enquanto, mandar embora é só deletá-lo da memória
		return;
	}

	// Se não mandamos o avião embora, devemos colocá-lo na fila desejada
	p->setAvgTimeToLeaveQueue(minWaitingTime);
	if (p->isVIP()) { totalVIP[insertQueue]++; }

	if (p->isFlying()) {
		totalTimeToLand[insertQueue] += 2;
		totalFuelOnPlanesToLand[insertQueue] += p->getFuel();
	} else {
		totalTimeToDeparture[insertQueue] += 2;
	}
	queue[insertQueue].enqueue(p);
}

void Airport::removePlane(int t_index) {
	if (t_index < 0 || t_index >= 3)
		throw MyException(ErrorTypes[INDEX_OUT_OF_BOUNDS], "Airport::removePlane");

	// Se a pista não está livre, não removemos ninguém da fila
	// Se a fila está vazia, não há quem remover
	if (timeToBeFree[t_index] != 0 || queue[t_index].empty()) return;

	// Se o código chega aqui, a pista está livre e tem gente na fila

	// A pista ficará duas unidades de tempo fora de serviço
	timeToBeFree[t_index] = 2;

	Plane *p = queue[t_index].getFront();

	if (p->isFlying()) {
		totalFuelOnPlanesToLand[t_index] -= p->getFuel();
		totalFuelOnPlanesThatLanded += p->getFuel();
	}

	if (p->isVIP()) { totalVIP[t_index]--; }

	// Tiramos o avião da fila
	queue[t_index].dequeue();
}

void Airport::update() {
	// Primeiro vamos aleatoriamente colocar de 0 a K aviões em contato com a torre
	// Depois vamos remover os aviões dos começos das filas, caso as pistas estejam
	// em serviço

	int cur_k = rand() % (this->k + 1); // Como é até k, fazemos (mod k+1)

	try {
		// A função add que se vira em como adicionar esses aviões na fila
		// Ela também tem a posibilidade de rejeitar um avião (mandá-lo embora)
		for (int i = 0; i < cur_k; i++) {
			this->addPlane(createRandomPlane());
			this->showWaitingPlanes();
		}
		// Mandamos remover aviões de todas as filas
		// A função remove que se vira com se é possível ou não remover o avião
		// do começo da fila i
		for (int i = 0; i < 3; i++) {
			removePlane(i);
			this->showWaitingPlanes();
		}
	} catch (MyException e) {
		e.what();
		exit(-1);
	}
	// Uma unidade de tempo passada
	cur_time++;

	// Se a pista i está em serviço, se passou uma unidade de tempo de serviço
	for (int i = 0; i < 3; i++)
		if (timeToBeFree[i] != 0) timeToBeFree[i]--;

	// Agora passamos por todos os aviões em fila atualizando-os
	for (int i = 0; i < 3; i++) queue[i].iterate([](Plane *p) { p->update(); });
}

void Airport::showWaitingPlanes() {
	std::ofstream file("temp.txt");

	if (!file.is_open())
		throw MyException(ErrorTypes[FILE_NOT_FOUND], "Airport::showWaitingPlanes");

	file << "Cur_Time: " << cur_time << std::endl;
	for (int i = 0; i < 3; i++) {
		file << "Fila da pista " << i + 1 << ":\n";
		file << "Tempo de espera: " << timeToBeFree[i] << std::endl;
		queue[i].PrintQueue(true, file);
		file << std::endl;
	}

	file.close();
	system("less temp.txt");
	system("rm temp.txt");
}