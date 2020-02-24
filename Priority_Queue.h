/* Arquivo com toda a implementação de uma Fila de Prioridades */

#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_

#include <algorithm>
#include <iostream>

template <class T>
class Priority_Queue {
  protected:
	T *item;      // Heap implementado em array (1-indexed)
	int numItems; // Num de elementos no heap
	int MAX;      // Tamanho do array

  private:
	// Aloca um array novo de tamanho size e copia o antigo no novo
	void resize(int size);
	// Funções sobe-heap e desce-heap vistas em ED1
	virtual void upheap(int i);
	virtual void downheap(int i);

  public:
	// Construtor padrão
	Priority_Queue();

	// Construtor a partir de uma lista de itens
	Priority_Queue(T arr[], int len);

	// Destrutor
	~Priority_Queue();

	// Inserir elemento
	void insert(T e);

	// Remove o maior da fila
	void removeFront();

	// Retorna o primeiro da fila
	T front();

	// Diz se a fila está vazia
	bool isEmpty();

	// Imprime todos os elementos do heap
	virtual void debug();
};

template <class T>
Priority_Queue<T>::Priority_Queue() : numItems(0), MAX(2), item(new T[2]) {}

template <class T>
Priority_Queue<T>::Priority_Queue(T arr[], int len) {
	for (int i = 0; i < len; i++) this->insert(arr[i]);
}

template <class T>
Priority_Queue<T>::~Priority_Queue() {
	if (item != nullptr) delete[] item;
	numItems = MAX = 0;
}

template <class T>
void Priority_Queue<T>::upheap(int i) {
	if (i < 0 || i >= MAX) {
		std::cerr << "Erro no upheap: " << i << " não é uma posição válida!"
		          << std::endl;
		return;
	}

	T v = item[i];
	while (i != 1 && item[i / 2] <= v) {
		item[i] = item[i / 2];
		i = i / 2;
	}
	item[i] = v;
}

template <class T>
void Priority_Queue<T>::downheap(int i) {
	if (i < 0 || i >= MAX) {
		std::cerr << "Erro no upheap: " << i << " não é uma posição válida!"
		          << std::endl;
		return;
	}

	T v = item[i];
	while (2 * i <= numItems) {
		int max = 2 * i;
		if (max < numItems && item[max] < item[max + 1]) max++;
		if (v >= item[max]) break;
		item[i] = item[max];
		i = max;
	}
	item[i] = v;
}

template <class T>
void Priority_Queue<T>::insert(T e) {
	if (numItems == MAX - 1) this->resize(2 * MAX);

	item[++numItems] = e;
	upheap(numItems);
}

template <class T>
void Priority_Queue<T>::removeFront() {
	if (this->isEmpty()) return;

	std::swap(item[1], item[numItems--]);
	downheap(1);
}

template <class T>
T Priority_Queue<T>::front() {
	if (!this->isEmpty()) return item[1];
}

template <class T>
bool Priority_Queue<T>::isEmpty() {
	return numItems == 0;
}

template <class T>
void Priority_Queue<T>::resize(int size) {
	T *newList = new T[size];
	for (int i = 0; i < std::min(numItems + 1, size); i++) newList[i] = item[i];
	delete[] item;
	item = newList;
	MAX = size;
}

template <class T>
void Priority_Queue<T>::debug() {
	std::cout << "Heap: [ ";
	for (int i = 1; i <= numItems; i++)
		std::cout << item[i] << ((i != numItems) ? ", " : "");
	std::cout << " ]\n";
}
#endif