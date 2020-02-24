#include <iostream>
#include "AirCompany.h"
#include "Destination.h"
#include "Plane.h"
#include "Pointer_Priority_Queue.h"
#include "Priority_Queue.h"

using namespace std;

void testeHeap() {
	Priority_Queue<int> pq;
	Pointer_Priority_Queue<int *> ppq;
	Plane *p1 = new Plane("AB323BRA", 30, false, true, 30);
	Plane *p2 = new Plane("CD333EUA", 30, true, false);
	Plane *p3 = new Plane("CD311CAN", 40, false, true, 10);
	p3->update();
	p3->update();
	Plane *p4 = new Plane("AB111EGI", 10, true, true, 10);
	p4->update();

	int *a, *b, *c, *d;
	a = new int;
	b = new int;
	c = new int;
	d = new int;
	*a = 10;
	*b = 7;
	*c = 11;
	*d = -1;

	int A, B, C, D;
	A = 10;
	B = 7;
	C = 11;
	D = -1;

	// pq.insert(p1);
	// pq.insert(p4);
	// pq.insert(p2);
	// pq.insert(p3);
	ppq.insert(b);
	ppq.insert(d);
	ppq.insert(c);
	ppq.insert(a);
	pq.insert(B);
	pq.insert(D);
	pq.insert(C);
	pq.insert(A);
	pq.debug();
	ppq.debug();
}

void initDestsAndCompanies() {
	cout << "Antes de inicializar os destinos" << endl;

	initDestinations();

	cout << "Destinos inicializados" << endl;
	for (int i = 0; i < numExistingDest; i++)
		cout << existingDests[i].getName() << " " << existingDests[i].getDistance()
		     << endl;

	cout << "Antes de inicializar as companhias" << endl;

	initAirCompanies();

	cout << "Companhias inicializadas" << endl;
	for (int i = 0; i < numExistingCompanies; i++) {
		cout << existingCompanies[i].getName() << " "
		     << existingCompanies[i].getNumDestinations() << ":" << endl;
		for (int j = 0; j < existingCompanies[i].getNumDestinations(); j++)
			cout << "\t" << existingCompanies[i].getDestination(j)->getName() << endl;
	}
}

int main() {
	srand(time(0));
	initDestinations();
	initAirCompanies();

	return 0;
}