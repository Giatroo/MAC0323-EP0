#include "Plane.h"

using namespace std;

Plane::Plane()
    : fuel(0),
      flyTime(0),
      timeWaiting(0),
      planeNumber(0),
      priority(0),
      company(""),
      destiny(""),
      flying(false),
      VIP(false) {}

// O parametro name deve ser do padrão AA00AAA onde A indica uma letra e 0
// indica um dígito
Plane::Plane(string name, int flyTime, bool VIP, bool flying, int fuel)
    : fuel(fuel),
      flyTime(flyTime),
      VIP(VIP),
      flying(flying),
      timeWaiting(0),
      priority(0) {
	setName(name);
}

void Plane::setName(string name) {
	company = name.substr(0, 2);
	planeNumber = stoi(name.substr(2, 3));
	destiny = name.substr(5, 3);
}

void Plane::update() {
	timeWaiting++;

	if (flying) {
		if (fuel > 0)
			fuel--;
		else
			VIP = true;
	} else if (timeWaiting > 0.1 * flyTime)
		VIP = true;

	updatePriority();
}

void Plane::updatePriority() { priority = (VIP) ? EMERGENCY : timeWaiting; }

bool Plane::operator<(Plane &p) {
	return (this->VIP ^ p.VIP) ? p.VIP : this->timeWaiting < p.timeWaiting;
}

bool Plane::operator<=(Plane &p) {
	return (this->VIP ^ p.VIP) ? p.VIP : this->timeWaiting <= p.timeWaiting;
}

bool Plane::operator>(Plane &p) {
	return (this->VIP ^ p.VIP) ? this->VIP : this->timeWaiting > p.timeWaiting;
}

bool Plane::operator>=(Plane &p) {
	return (this->VIP ^ p.VIP) ? this->VIP : this->timeWaiting >= p.timeWaiting;
}

// Overload no operador << para poder imprimir um avião usando cout
ostream &operator<<(ostream &os, Plane &p) {
	os << "Avião " << p.company << p.planeNumber << p.destiny << ":" << endl;
	os << "\tCombustível - " << p.fuel << endl;
	os << "\tEsperando por " << p.timeWaiting << " unidades de tempo" << endl;
	os << "\tTempo de voo estimado - " << p.flyTime << endl;
	os << "\tPrioridade - " << p.priority << endl;
	return os;
}
