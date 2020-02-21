#include <iostream>
#include "Priority_Queue.h"

using namespace std;

int main() {
    Priority_Queue<int> pq;

    pq.debug();
    pq.Insert(3);
    pq.debug();
    pq.Insert(4);
    pq.debug();
    pq.Insert(10);
    pq.debug();
    pq.Insert(2);
    pq.debug();
    pq.RemoveFront();
    pq.debug();
    cout << pq.Front() << endl;

    return 0;
}