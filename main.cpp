#include <iostream>
#include "Priority_Queue.h"
#include "Plane.h"

using namespace std;

int main() {
    Priority_Queue<Plane> pq;

    Plane p1("AB123BRA", 10, true, false);
    p1.update();
    Plane p2("BC321ENG", 20, true, false);
    p2.update();
    Plane p3("AB223EUA", 20, false, true, 10);
    p3.update();
    p3.update();
    Plane p4("AB224BRA", 20, false, true, 10);
    Plane p5("BC332ALE", 4, false, false);
    p5.update();
    p5.update();
    Plane p6("CD111ENG", 1000, false, true, 1);
    p6.update();

    pq.Insert(p4);
    pq.Insert(p2);
    pq.Insert(p5);
    pq.Insert(p1);
    pq.Insert(p6);
    pq.Insert(p3);
    system("clear");
    pq.debug();
    char a;
    while (!pq.isEmpty()) {
        cin >> a;
        system("clear");
        pq.RemoveFront();
        pq.debug();
    }

    return 0;
}