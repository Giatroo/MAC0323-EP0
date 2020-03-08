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
		lastVIP[i] = queue[i].getFrontIterator();
	}
}

void Airport::addPlane(Plane *p) {
	if (p->isVIP())
		addVIP(p);
	else
		addNonVIP(p);
}

void Airport::addNonVIP(Plane *p) {
	// Vamos buscar qual é a fila que fará com que esse avião
	// o mínimo possível
	int minWaitingTime = INT32_MAX;
	int insertQueue; // Índice da fila em que vamos inserir o avião

	// Se o avião está para pousar, só olhamos as duas primeiras
	int totQueues = (p->isFlying()) ? 2 : 3;

	for (int i = 0; i < totQueues; i++) {
		if (!queue[i].empty()) {
			if (minWaitingTime > queue[i].getBack()->getAvgTimeToLeaveQueue() + 2) {
				minWaitingTime = queue[i].getBack()->getAvgTimeToLeaveQueue() + 2;
				// +2 por causa do tempo de serviço
				insertQueue = i;
			}
		} else {
			minWaitingTime = timeToBeFree[i];
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

	if (p->isFlying()) {
		totalTimeToLand[insertQueue] += 2;
		totalFuelOnPlanesToLand[insertQueue] += p->getFuel();
	} else {
		totalTimeToDeparture[insertQueue] += 2;
	}
	queue[insertQueue].enqueue(p);
}

void Airport::addVIP(Plane *p) {
	int minWaitingTime = INT32_MAX; // O tempo mínimo de espera desse VIP
	int insertionQueue = 0;         // A fila em que vou inserir o VIP
	// Se dois lugares na fila são igualmente bons, então o desempate é feito
	// pelo menos tempo de espera do último avião na fila (assim atrapalhamos menos
	// aviões que já estão esperando)
	int minTroubleTime = INT32_MAX;

	// Escolhemos a fila onde o VIP demorará menos tempo para pousar
	for (int i = 0; i < 3; i++) {
		// Se a fila está sem nenhum VIP
		if (lastVIP[i] == queue[i].getFrontIterator()) {
			// Se o tempo para a pista ficar livre é menor que o menor tempo até agr
			if (minWaitingTime > timeToBeFree[i]) {
				minWaitingTime = timeToBeFree[i];
				insertionQueue = i;

				minTroubleTime =
				    (queue[i].empty()) ?
				        0 :
				        queue[i].getBack()->getAvgTimeToLeaveQueue() + 2;
			} else if (minWaitingTime == timeToBeFree[i]) {
				// Se o menor tempo até agr é igual ao da fila ficar livre (empate)
				int thisTroubleTime =
				    (queue[i].empty()) ?
				        0 :
				        queue[i].getBack()->getAvgTimeToLeaveQueue() + 2;
				// Desempatamos pelo menor tempo do último avião da fila
				if (thisTroubleTime <= minTroubleTime) {
					insertionQueue = i;
					minTroubleTime = thisTroubleTime;
				}
			}

		} else { // Se a fila já possui algo VIP
			// Se o menor tempo é maior que o do último avião (mais 2)
			if (minWaitingTime >
			    (*lastVIP[i]).getElement()->getAvgTimeToLeaveQueue() + 2) {
				minWaitingTime =
				    (*lastVIP[i]).getElement()->getAvgTimeToLeaveQueue() + 2;
				insertionQueue = i;

				// Não precisamos ver se a fila está vazia, pois sabemos que há pelo
				// menos um VIP nela
				minTroubleTime = queue[i].getBack()->getAvgTimeToLeaveQueue() + 2;
			} else if (minWaitingTime ==
			           (*lastVIP[i]).getElement()->getAvgTimeToLeaveQueue() + 2) {
				// Se rolou empate
				int thisTroubleTime =
				    queue[i].getBack()->getAvgTimeToLeaveQueue() + 2;
				// Desempatamos pelo menor tempo do último avião da fila
				if (thisTroubleTime <= minTroubleTime) {
					insertionQueue = i;
					minTroubleTime = thisTroubleTime;
				}
			}
		}
	}

	// std::cout << "\nFila escolhida " << insertionQueue << "\n";
	// std::cout << "minWaitingTime " << minWaitingTime << "\n";
	// if (lastVIP[insertionQueue] != queue[insertionQueue].getFrontIterator()) {
	// 	std::cout
	// 	    << "tempo do da frente "
	// 	    << (*lastVIP[insertionQueue]).getElement()->getAvgTimeToLeaveQueue()
	// 	    << "\n";
	// 	std::cout << "O da frente é "
	// 	          << (*lastVIP[insertionQueue]).getElement()->getName() << " \n";
	// }
	// std::cout << "tempo to be free " << timeToBeFree[insertionQueue] << "\n";
	// char c;
	// std::cin >> c;

	// Se ele está tentando pousar e não tem combustível para esperar
	if (p->isFlying() && p->getFuel() < minWaitingTime) {
		std::cout << "Descardando avião\n";
		delete p; // A gente manda ele embora (mesmo que seja VIP)
		return;
	}

	// Agora vemos as estatísticas
	p->setAvgTimeToLeaveQueue(minWaitingTime);
	totalVIP[insertionQueue]++;

	if (p->isFlying()) {
		totalTimeToLand[insertionQueue] += 2;
		totalFuelOnPlanesToLand[insertionQueue] += p->getFuel();
	} else {
		totalTimeToDeparture[insertionQueue] += 2;
	}

	// Adicionamos ele na fila
	Queue_Iterator<Plane *> insertionPoint = lastVIP[insertionQueue];
	insertionPoint--; // Decrementamos pois addIn vai adicionar na fente de
	                  // insertionPoint
	// std::cout << "InsertionPoint = ";
	// if (insertionPoint != queue[insertionQueue].getBackIterator())
	// 	std::cout << (*insertionPoint).getElement()->getName() << std::endl;
	// else
	// 	std::cout << "Inserindo no fim da fila\n";
	queue[insertionQueue].addIn(&insertionPoint, p);
	lastVIP[insertionQueue]--; // Decrementamos o ponteiro do último VIP, pois
	// há mais um VIP adicionado

	// Agora precisamos atualizar o tempo médio de espera de todos os aviões
	// atrás do VIP inserido
	while (insertionPoint != queue[insertionQueue].getBackIterator()) {
		(*insertionPoint)
		    .getElement()
		    ->setAvgTimeToLeaveQueue(
		        (*insertionPoint).nextNode->getElement()->getAvgTimeToLeaveQueue() +
		        2);

		insertionPoint--;
	}
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

	auto it = queue[t_index].getFrontIterator();
	it--;
	if (queue[t_index].empty() || !(*it).getElement()->isVIP())
		lastVIP[t_index] = queue[t_index].getFrontIterator();
}

void Airport::update() {
	// Primeiro vamos aleatoriamente colocar de 0 a K aviões em contato com a torre
	// Depois vamos remover os aviões dos começos das filas, caso as pistas estejam
	// em serviço
	// Por fim, vamos passar pelas filas checando se nenhum avião deve ser
	// transformado em VIP (seja pq um VIP entrou na frente dele e ele vai ficar sem
	// combustível ou porque ele atingiu 10% do tempo esperado de vôo)

	int cur_k = rand() % (this->k + 1); // Como é até k, fazemos (mod k+1)

	for (int i = 0; i < 3; i++) {}

	try {
		// A função add que se vira em como adicionar esses aviões na fila
		// Ela também tem a posibilidade de rejeitar um avião (mandá-lo embora)
		for (int i = 0; i < cur_k; i++) {
			this->addPlane(createRandomPlane());
			this->showWaitingPlanes();
		}
		// Mandamos remover aviões de todas as filas
		// A função remove que se vira com se é possível ou não remover o avião
		// do começo da fila i (devido o tempo de espera das pistas)
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

	// E, por fim, precisamos atualizar a fila em si
	// Se algum avião que estava sobrevoando o aeroporto se tornou emergencial, então
	// nós mandamos ele para outro aeroporto
	// E se é algum avião que estava esperando para decolar, nós removemos ele da
	// fila em que estava e chamamos o comando addVIP passando ele (assim ele vai
	// para a fila que vai fazê-lo sair o mais rápido possível)
	for (int i = 0; i < 3; i++) {
		// std::cout << "Fila " << i << " ";
		// if (lastVIP[i] != queue[i].getFrontIterator())
		// 	std::cout << "ultimo vip é " << (*lastVIP[i]).getElement()->getName()
		// 	          << " \n";
		// else
		// 	std::cout << "vazia\n";
		Queue_Iterator<Plane *> it = lastVIP[i];
		it--;
		while (it != queue[i].getBackIterator()) {
			// std::cout << "Cur it = " << (*it).getElement()->getName() << "\n";
			if ((*it).getElement()->isFlying() && (*it).getElement()->isVIP()) {
				// std::cout << "Removendo cara q virou VIP e estava voando\n";
				Plane *p = queue[i].removeFrom(it); // Removemos da fila
				delete p;                           // E mandamos embora
			} else if (!(*it).getElement()->isFlying() &&
			           (*it).getElement()->isVIP()) {
				// std::cout << "Remanejando um cara q virou VIP e estava no solo\n";
				Plane *p = queue[i].removeFrom(it); // Removemos da fila
				this->addPlane(p); // E adicionamos ele no aeroport dnv
			}
			it--;
		}
		// char c;
		// std::cin >> c;
	}
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