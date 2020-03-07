#include <iostream>
#include "AirCompany.h"
#include "Airport.h"
#include "Destination.h"
#include "Plane.h"
#include "Priority_Queue.h"
#include "Queue.h"

using namespace std;

void init() {
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
	srand(/*time(0)*/ 5); // 3 está dando segfault
	init();

	Airport airport(100, 5);

	for (int i = 0; i < 5; i++) { airport.update(); }

	/*airport.addPlane(createRandomPlane());
	airport.addPlane(createRandomPlane());
	airport.addPlane(createRandomPlane());
	airport.showWaitingPlanes();
	airport.addPlane(createRandomPlane());
	airport.showWaitingPlanes();
	airport.addPlane(createRandomPlane());
	airport.showWaitingPlanes();
	airport.addPlane(createRandomPlane());
	airport.showWaitingPlanes();*/

	/*Pointer_Queue<Plane *> q;
	q.enqueue(createRandomPlane());
	q.enqueue(createRandomPlane());
	q.enqueue(createRandomPlane());
	q.PrintQueue();

	auto it = q.getFrontIterator();
	it--;
	auto jt = it;
	cout << (*it).getElement()->getName() << endl;
	cout << (*jt).getElement()->getName() << endl;
	it--;
	cout << endl;
	cout << (*it).getElement()->getName() << endl;
	cout << (*jt).getElement()->getName() << endl;*/

	return 0;
}