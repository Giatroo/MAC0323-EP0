/* Arquivo com toda a implementação de uma Fila de Prioridades */

#ifndef _PRIORITY_QUEUE_H_
#define _PRIORITY_QUEUE_H_

#include "MyException.h"

#include <algorithm>
#include <fstream>
#include <iostream>

/* Classe para uma fila de prioridades para dados abstratos
 */
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

	// Retorna o tamanho do heap
	int getSize();

	// Imprime todos os elementos do heap
	virtual void debug(std::ostream &t_stream);
};

/* Classe que herda de uma fila de prioridades
 * Criamos essa classe pois se criamos uma fila de prioridade que contém
 * ponteiros para um certo tipo de elemento, então a fila de prioridade
 * acabará ordenando pelo valor do endereço, o que não queremos.
 * Queremos que ela ordene usando os operadores <, >, <= e >= definidos
 * na classe dos elementos.
 *
 * Sendo assim, precisamos de uma Priority_Queue a parte só para caso
 * nós queiramos fazer uma fila de prioridade de ponteiros.
 *
 * Nessa classe, a fila armazenará ponteiros, deixando o código eficiente,
 * mas fará todas as comparações para saber quem é maior que quem usando
 * os operadores comparativos definidos dentro da classe.
 */
template <class T>
class Pointer_Priority_Queue : public Priority_Queue<T> {
	// Precisamos dar overwrite nas funções upheap, downheap e debug
	// pois são as únicas que usam os conteúdos dos ponteiros que estamos
	// armazenando
  private:
	void upheap(int i);
	void downheap(int i);

  public:
	void debug(std::ostream &t_stream);
};

// ----==== IMPLEMENTAÇAO ====----

// PRIORITY QUEUE

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
	if (i < 0 || i >= MAX)
		throw MyException(ErrorTypes[INDEX_OUT_OF_BOUNDS], "Priority_Queue::upheap");

	T v = item[i];
	while (i != 1 && item[i / 2] <= v) {
		item[i] = item[i / 2];
		i = i / 2;
	}
	item[i] = v;
}

template <class T>
void Priority_Queue<T>::downheap(int i) {
	if (i < 0 || i >= MAX)
		throw MyException(ErrorTypes[INDEX_OUT_OF_BOUNDS],
		                  "Priority_Queue::downheap");

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
	if (this->isEmpty())
		throw MyException(ErrorTypes[EMPTY_DATA_STRUCTURE],
		                  "Priority_Queue::removeFront");

	std::swap(item[1], item[numItems--]);
	downheap(1);
}

template <class T>
T Priority_Queue<T>::front() {
	if (this->isEmpty())
		throw MyException(ErrorTypes[EMPTY_DATA_STRUCTURE],
		                  "Priority_Queue::removeFront");

	return item[1];
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
int Priority_Queue<T>::getSize() {
	return numItems;
}

template <class T>
void Priority_Queue<T>::debug(std::ostream &t_stream) {
	t_stream << "Heap: [ ";
	for (int i = 1; i <= numItems; i++)
		t_stream << item[i] << ((i != numItems) ? ", " : "");
	t_stream << " ]\n";
}

// POINTER PRIORITY QUEUE

template <class T>
void Pointer_Priority_Queue<T>::upheap(int i) {
	if (i < 0 || i >= this->MAX)
		throw MyException(ErrorTypes[INDEX_OUT_OF_BOUNDS],
		                  "Pointer_Priority_Queue::upheap");

	T v = this->item[i];
	while (i != 1 && *this->item[i / 2] <= *v) {
		this->item[i] = this->item[i / 2];
		i = i / 2;
	}
	this->item[i] = v;
}

template <class T>
void Pointer_Priority_Queue<T>::downheap(int i) {
	if (i < 0 || i >= this->MAX)
		throw MyException(ErrorTypes[INDEX_OUT_OF_BOUNDS],
		                  "Pointer_Priority_Queue::downheap");

	T v = this->item[i];
	while (2 * i <= this->numItems) {
		int max = 2 * i;
		if (max < this->numItems && *this->item[max] < *this->item[max + 1]) max++;
		if (*v >= *this->item[max]) break;
		this->item[i] = this->item[max];
		i = max;
	}
	this->item[i] = v;
}

template <class T>
void Pointer_Priority_Queue<T>::debug(std::ostream &t_stream) {
	t_stream << "Heap: [ ";
	for (int i = 1; i <= this->numItems; i++)
		t_stream << *this->item[i] << ((i != this->numItems) ? ", " : "");
	t_stream << " ]\n";
}

#endif