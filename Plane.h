/* Arquivo para a classe de um avião. */

#ifndef _PLANE_H_
#define _PLANE_H_

#include <iostream>
using namespace std;

#define EMERGENCY -1

class Plane {
private:
    string company; // Código de 2 letras para a compania do avião
    int planeNumber; // Número de três dígitos do avião
    string destiny; // Código de 3 letras para o destino do voo
    int fuel;   // Combustível do avião
    int timeWaiting; // Tempo que o avião está esperando para decolar
    int flyTime; // Tempo de voo estimado
    bool VIP; // Very important plane (emergência)
    int priority; // Número da prioridade do avião
    // Variável que indica se o avião está voando ou se está esperando para decolar
    bool flying; 
public:
    // Construtor padrão
    Plane();

    // Construtor 
    Plane(string name, int flyTime, bool VIP, bool flying, int fuel);

    void setName(string name);

    // Atualiza uma unidade de tempo para o avião
    void update();

    // Atualiza a prioridade do avião baseando-se em suas características
    void updatePriority();

    // Overload no operador < para determinar se um avião tem mais ou menos prioridade que outro
    // Ao fazer p1 < p2, retorna true se p1.priority < p2.priority e false caso contrário
    bool operator < (Plane &p);
    bool operator <= (Plane &p);
    bool operator > (Plane &p);
    bool operator >= (Plane &p);

    // ostream& operator << (ostream &os);
    friend ostream& operator << (ostream &os, Plane &p);
};

Plane::Plane() {
    fuel = flyTime = timeWaiting = planeNumber = priority = 0;
    company = destiny = "";
    flying = VIP = false;
}

// O parametro name deve ser do padrão AA00AAA onde A indica uma letra e 0 indica um dígito
Plane::Plane(string name, int flyTime, bool VIP, bool flying, int fuel = 0) {
    setName(name);
    this->fuel = fuel;
    this->flyTime = flyTime;
    this->VIP = VIP;
    this->flying = flying;
    this->timeWaiting = 0;
    this->priority = 0;
}

void Plane::setName(string name) {
    company = name.substr(0, 2);
    planeNumber = stoi(name.substr(2, 3));
    destiny = name.substr(5, 3);
}

void Plane::update() {
    timeWaiting++;
    
    if (flying) {
        if (fuel > 0) fuel--;
        else VIP = true;
    } else if (timeWaiting > 0.1 * flyTime) VIP = true;
    
    updatePriority();
}

void Plane::updatePriority() {
    if (VIP) priority = EMERGENCY;
    else priority = timeWaiting;
}

bool Plane::operator < (Plane &p) {
    if (this->VIP ^ p.VIP) return p.VIP;
    else return this->timeWaiting < p.timeWaiting;
}

bool Plane::operator <= (Plane &p) {
    if (this->VIP ^ p.VIP) return p.VIP;
    else return this->timeWaiting <= p.timeWaiting;
}

bool Plane::operator > (Plane &p) {
    if (this->VIP ^ p.VIP) return this->VIP;
    else return this->timeWaiting > p.timeWaiting;
}

bool Plane::operator >= (Plane &p) {
    if (this->VIP ^ p.VIP) return this->VIP;
    else return this->timeWaiting >= p.timeWaiting;
}

// Overload no operador << para poder imprimir um avião usando cout
ostream& operator << (ostream &os, Plane &p) {
    os << "Avião " << p.company << p.planeNumber << p.destiny << ":" << endl;
    os << "\tCombustível - " << p.fuel << endl;
    os << "\tEsperando por " << p.timeWaiting << " unidades de tempo" << endl;
    os << "\tTempo de voo estimado - " << p.flyTime << endl;
    os << "\tPrioridade - " << p.priority << endl;
}

#endif