#include <iostream>
#include "Destination.h"
#include "Plane.h"
#include "Priority_Queue.h"
#include "Pointer_Priority_Queue.h"

using namespace std;

int main() {
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

    return 0;
}