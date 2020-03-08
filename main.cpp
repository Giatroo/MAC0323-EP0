#include <functional>
#include <iostream>

#include "AirCompany.h"
#include "Airport.h"
#include "Destination.h"
#include "Plane.h"
#include "Priority_Queue.h"
#include "Queue.h"

using namespace std;

class Option {
	string name;
	function<void()> func;
};

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

int main() {
	int k, t;
	init();

	cout << "Bem-vindo ao simulador de torre de aeroporto:\n";
	cout << "Por favor, digite o máximo de aviões por tempo: ";
	cin >> k;
	cout << "Por favor, digite por quanto tempo você quer que a simulação dure: ";
	cin >> t;
	Airport airport(t, k);

	return 0;
}