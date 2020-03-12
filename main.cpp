#include <functional>
#include <iostream>

#include "AirCompany.h"
#include "Airport.h"
#include "Destination.h"
#include "Plane.h"
#include "Priority_Queue.h"
#include "Queue.h"

using namespace std;

// Esse método é chamado para inicializar a nosso programa
void init() {
	srand(/*time(0)*/ 5); // 3 está dando segfault

	try {
		initDestinations();
		initAirCompanies();
	} catch (MyException e) {
		e.what();
		cerr << "Exiting the program." << endl;
		exit(-1);
	}
}

int readInput(bool simulationOver) {
	int input = INT32_MAX;
	const int size = 8;

	while (input < 0 || input > size) {
		system("clear");
		if (input != INT32_MAX) cout << "Deve ser um número válido\n\n";

		if (simulationOver)
			cout << "A simulação acabou, mas você ainda pode ver as estatísticas:"
			     << endl;

		cout << "0 - Sair\n";
		cout << "1 - Mostrar todas as filas\n";
		cout << "2 - Mostrar tempo médio para sair das filas\n";
		cout << "3 - Mostrar média de combustível nos aviões sobrevoando\n";
		cout << "4 - Mostrar média de combustível nos aviões pousados\n";
		cout << "5 - Mostrar tempo médio para pouso\n";
		cout << "6 - Mostrar tempo médio para decolagem\n";
		cout << "7 - Mostrar quantidade de emergências nas filas\n";
		if (!simulationOver) cout << "8 - Atualizar o aeroporto\n";
		cout << "Escolha uma opção: ";
		cin >> input;
		if (input == 8 && simulationOver) input++; // Só para dizer que é inválido
	}

	return input;
}

int main() {
	int k, t;
	init();

	cout << "Bem-vindo ao simulador de torre de aeroporto:\n";
	cout << "Por favor, digite o máximo de aviões por tempo: ";
	cin >> k;
	cout << "Por favor, digite por quanto tempo você quer que a simulação dure: ";
	cin >> t;

	Airport airport(t, k); // O aeroporto
	auto it = airport.queue[0].getFrontIterator();
	std::cout << it.curNode << std::endl
	          << (*it).prevNode << std::endl
	          << (*it).nextNode << std::endl;

	int userOption = readInput(false);
	while (airport.simulating() && userOption != 0) {
		switch (userOption) {
			case 0:
				system("clear");
				cout << "Saindo...\n";
				return 0;
				break;
			case 1: airport.showWaitingPlanes(); break;
			case 2: airport.showAllExpectedTimes(); break;
			case 3: airport.showAvgFuelOnPlanesWaitingToLand(); break;
			case 4: airport.showAvgFuelOnPlanesThatLanded(); break;
			case 5: airport.showAvgTimeToLand(); break;
			case 6: airport.showAvgTimeToDeparture(); break;
			case 7: airport.showQntOfVeryImportantPlanes(); break;
			case 8: airport.update(); break;
			default: break;
		}

		userOption = readInput(false);
	}

	// Quando a simulação acaba, o usuário deve ainda poder ver as estatísticas
	while (userOption != 0) {
		switch (userOption) {
			case 0:
				system("clear");
				cout << "Saindo...\n";
				return 0;
				break;
			case 1: airport.showWaitingPlanes(); break;
			case 2: airport.showAllExpectedTimes(); break;
			case 3: airport.showAvgFuelOnPlanesWaitingToLand(); break;
			case 4: airport.showAvgFuelOnPlanesThatLanded(); break;
			case 5: airport.showAvgTimeToLand(); break;
			case 6: airport.showAvgTimeToDeparture(); break;
			case 7: airport.showQntOfVeryImportantPlanes(); break;
			default: break;
		}
		it = airport.queue[0].getBackIterator();

		userOption = readInput(true);
	}

	return 0;
}