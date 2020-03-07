/* Arquivo para implementação de uma fila de tipos genéricos */

#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "MyException.h"

#include <iostream>

// Classe de tipo genérica para um nó de uma lista ligada.
// A nossa fila será implementada através de uma lista ligada.
template <class T>
class QueueNode {
  private:
	T element; // O elemento do nó
  public:
	QueueNode *nextNode; // Um ponteiro para o próximo nó
	QueueNode *prevNode; // Um ponteiro para o nó anterior

	// Construtor padrão
	QueueNode();

	// Construtor com um elemento que será colocado dentro do nó
	QueueNode(T t_element);

	// Método para retornar o elemento
	T getElement() const;

	// Método para atribuir algo ao elemento
	void setElement(T t_element);

	bool operator==(QueueNode node);
	bool operator!=(QueueNode node);
};

// Protótipo para um iterador sobre nossa fila que será declaro mais abaixo
template <class T>
class Queue_Iterator;

/* Implementação clássica de uma fila utilizando uma Linked List */
template <class T>
class Queue {
  private:
	// Ponteiro para a célula dummy que aponta para a frente da fila
	// front->nextNode deve ser nulo e front->prevNode é o primeiro da fila
	QueueNode<T> *front;

	// Ponteiro para a célula dummy que aponta para o fim da fila
	// back->nextNode é o fim da fila e back->prevNode deve ser nulo
	QueueNode<T> *back;

	// Número de nós da fila (sem contar com os dois nós dummies)
	int numNodes;

  public:
	// Construtor
	Queue();
	// Destrutor
	~Queue();

	// Método para adicionar um elemento ao fim da fila
	void enqueue(T t_element);

	// Método para remover um elemento do fim da fila
	// (O método livra a memória que o elemento estava ocupando)
	void dequeue();

	// Método para acessar o primeiro elemento da fila
	T getFront();

	// Método para acessar o último elemento da fila
	T getBack();

	// Método para saber o número de elementos da fila
	int size();

	// Método para saber se a fila está vazia
	bool empty();

	// Método para retornar um iterador para o primeiro elemento da fila
	Queue_Iterator<T> getFrontIterator();

	// Método para retornar um iterador para o último elemento da fila
	Queue_Iterator<T> getBackIterator();

	// Esse método itera sobre todos os elementos da fila aplicando a função
	// func sobre eles
	void iterate(void (*func)(T element));

	// Método que remove um elemento da fila apontado por um iterador
	T removeFrom(Queue_Iterator<T> &it);

	// Método que adiciona um elemento na frente de um elemento apontado por um
	// iterador
	void addIn(Queue_Iterator<T> *it, T e);

	// Método para imprimir todos os elementos da fila
	// Imprime do primeiro para o último a menos que 'false' seja passado como
	// parâmetro
	// O segundo é para mudar onde imprimir
	virtual void PrintQueue(bool firstToLast = true, std::ostream &out = std::cout);
};

/* Classe que é um iterador para uma fila
 * Podemos inicializar um iterador com os métodos getFrontIterator ou
 * getBackIterator de um objeto Queue<T>
 *
 * Os operadores ++, -- e * estão disponívels para, respectivamente,
 * avançar o iterador (ir para nextElement), voltar o iterador (ir para prevElement)
 * e
 * acessar o conteúdo do iterador. (Versões += e -= também estão disponíveis)
 */
template <class T>
class Queue_Iterator {
  private:
	// Um apontador para o elemento que o iterador está atualmente
	QueueNode<T> *curNode;

	// Essa linha faz com que a classe Queue possa ter acesso à variável curNode
	// (o contrário não é verdade, essa classe não tem acesso aos
	// membros privado de Queue)
	friend class Queue<T>;

  public:
	// O operador * deve retornar *curNode para o usuário
	QueueNode<T> operator*();

	// O operador ++ deve avançar o iterador uma posição
	// (esse operador é ++(*this) )
	void operator++();
	// (esse operador é (*this)++ )
	void operator++(int);

	// O operador -- deve voltar o iterador uma posição
	// (esse operador é --(*this) )
	void operator--();
	// (esse operador é (*this)-- )
	void operator--(int);

	// O operador += deve avançar o iterador n posições
	// (ou parar no começo da fila se n for muito grande)
	void operator+=(int n);

	// O operador -= deve voltar o iterador n posições
	// (ou parar no fim da fila se n for muito grande)
	void operator-=(int n);

	// O operador == retorna true se todos o pointeiro para um
	// no é igual o ponteiro do outro
	bool operator==(Queue_Iterator it);

	// O operador != retorna true se o ponteiro para um nó
	// é diferente do outro
	bool operator!=(Queue_Iterator it);
};

/* Quando T for um pointeiro, é preciso instanciar essa classe */
template <class T>
class Pointer_Queue : public Queue<T> {
  public:
	void PrintQueue(bool firstToLast = true, std::ostream &out = std::cout);
};

// ----==== IMPLEMENTAÇÃO ====----

// QUEUE NODE

template <class T>
QueueNode<T>::QueueNode() : nextNode(nullptr), prevNode(nullptr) {}

// Chama o construtor padrão e depois diz que o elemento é igual
// ao elemento de parâmetro
template <class T>
QueueNode<T>::QueueNode(T t_element) : QueueNode<T>() {
	this->setElement(t_element);
}
template <class T>
T QueueNode<T>::getElement() const {
	if (this == nullptr)
		throw MyException(ErrorTypes[NULL_POINTER], "QueueNode::getElement");

	return this->element;
}

template <class T>
void QueueNode<T>::setElement(T t_element) {
	if (this == nullptr)
		throw MyException(ErrorTypes[NULL_POINTER], "QueueNode::setElement");

	this->element = t_element;
}

template <class T>
bool QueueNode<T>::operator==(QueueNode node) {
	return (this->prevNode == node.prevNode && this->nextNode == node.nextNode &&
	        this->element == node.element);
}

template <class T>
bool QueueNode<T>::operator!=(QueueNode node) {
	return (this->prevNode != node.prevNode || this->nextNode != node.nextNode ||
	        this->element != node.element);
}

// QUEUE

template <class T>
Queue<T>::Queue()
    : front(new QueueNode<T>()), back(new QueueNode<T>()), numNodes(0) {
	front->prevNode = back;
	front->nextNode = nullptr;
	back->prevNode = nullptr;
	back->nextNode = front;
}

template <class T>
Queue<T>::~Queue() {
	QueueNode<T> *p = back, *trash;
	// Deleta todos os nós iterando sobre eles e guardando o próximo nó a ser iterado
	while (p != nullptr) {
		trash = p;
		p = p->nextNode;
		delete trash;
	}
}

template <class T>
void Queue<T>::enqueue(T t_element) {
	// Alocando o novo nó
	QueueNode<T> *newNode = new QueueNode<T>(t_element);

	// O anterior do antigo fim da fila é o novo nó (o novo fim da fila)
	back->nextNode->prevNode = newNode;
	// O anterior do novo nó será o nó dummy das costas da fila
	newNode->prevNode = back;
	// O próximo do novo nó será o antigo final da fila
	newNode->nextNode = back->nextNode;
	// O novo nó é o novo final da fila
	back->nextNode = newNode;

	// Lembre-se, back->nextNode é o final elemento da fila, enquanto
	// *back é um elemento dummy que está lá para facilitar as coisas

	numNodes++;
}

template <class T>
void Queue<T>::dequeue() {
	if (this->empty())
		throw MyException(ErrorTypes[EMPTY_DATA_STRUCTURE], "Queue::dequeue");

	// Vamos remover o anterior à frente (o começo da fila)
	QueueNode<T> *trash = front->prevNode;

	// O novo começo da fila deve apontar para o nó dummy da frente
	trash->prevNode->nextNode = front;
	// O começo da fila agora é o anterior ao antigo começo
	front->prevNode = trash->prevNode;

	// Comando não realmente necessário
	trash->prevNode = trash->nextNode = nullptr;
	delete trash; // Deletando o nó

	// Lembre-se, front->prevNode é o começo da fila, enquanto
	// *front é um elemento dummy que está lá para facilitar as coisas

	numNodes--;
}

template <class T>
T Queue<T>::getFront() {
	if (front == nullptr)
		throw MyException(ErrorTypes[NULL_POINTER], "Queue::getFront");
	if (this->empty())
		throw MyException(ErrorTypes[EMPTY_DATA_STRUCTURE], "Queue::getFront");

	return this->front->prevNode->getElement();
}

template <class T>
T Queue<T>::getBack() {
	if (back == nullptr)
		throw MyException(ErrorTypes[NULL_POINTER], "Queue::getBack");
	if (this->empty())
		throw MyException(ErrorTypes[EMPTY_DATA_STRUCTURE], "Queue::getBack");

	return this->back->nextNode->getElement();
}

template <class T>
int Queue<T>::size() {
	return this->numNodes;
}

template <class T>
bool Queue<T>::empty() {
	return this->numNodes == 0;
}

// QUEUE_ITERATOR
template <class T>
QueueNode<T> Queue_Iterator<T>::operator*() {
	return *(this->curNode);
}

template <class T>
void Queue_Iterator<T>::operator++() {
	if (this->curNode->nextNode != nullptr) this->curNode = this->curNode->nextNode;
}
template <class T>
void Queue_Iterator<T>::operator++(int) {
	if (this->curNode->nextNode != nullptr) this->curNode = this->curNode->nextNode;
}

template <class T>
void Queue_Iterator<T>::operator--() {
	if (this->curNode->prevNode != nullptr) this->curNode = this->curNode->prevNode;
}

template <class T>
void Queue_Iterator<T>::operator--(int) {
	if (this->curNode->prevNode != nullptr) this->curNode = this->curNode->prevNode;
}

template <class T>
void Queue_Iterator<T>::operator+=(int n) {
	for (int i = 0; i < n; i++) this->operator++();
}

template <class T>
void Queue_Iterator<T>::operator-=(int n) {
	for (int i = 0; i < n; i++) this->operator--();
}

template <class T>
bool Queue_Iterator<T>::operator==(Queue_Iterator it) {
	return (*this->curNode == *it);
}

template <class T>
bool Queue_Iterator<T>::operator!=(Queue_Iterator it) {
	return (*this->curNode != *it);
}

// QUEUE (métodos que usam iteradores)

template <class T>
Queue_Iterator<T> Queue<T>::getFrontIterator() {
	Queue_Iterator<T> it;
	it.curNode = front;
	return it;
}

template <class T>
Queue_Iterator<T> Queue<T>::getBackIterator() {
	Queue_Iterator<T> it;
	it.curNode = back;
	return it;
}

template <class T>
T Queue<T>::removeFrom(Queue_Iterator<T> &it) {
	QueueNode<T> n = *it;
	n.prevNode->nextNode = n.nextNode;
	n.nextNode->prevNode = n.prevNode;
	T e = n.getElement();
	numNodes--;
	return e;
}

template <class T>
void Queue<T>::addIn(Queue_Iterator<T> *it, T e) {
	// QueueNode<T> n = *it;
	// Criamos um novo nó com o elemento e
	QueueNode<T> *newNode;
	newNode = new QueueNode<T>(e);

	(*it).curNode->nextNode->prevNode = newNode;
	newNode->nextNode = (*it).curNode->nextNode;
	newNode->prevNode = (*it).curNode;
	(*it).curNode->nextNode = newNode;

	numNodes++;
}

template <class T>
void Queue<T>::iterate(void (*func)(T element)) {
	Queue_Iterator<T> it = getFrontIterator();
	it--;
	while ((*it).prevNode != nullptr) {
		func((*it).getElement());
		it--;
	}
}

template <class T>
void Queue<T>::PrintQueue(bool firstToLast, std::ostream &out) {
	Queue_Iterator<T> it;
	if (firstToLast) {
		it = this->getFrontIterator();
		it--;
		while ((*it).prevNode != nullptr) {
			out << (*it).getElement() << " ";
			it--;
		}
	} else {
		it = this->getBackIterator();
		it++;
		while ((*it).nextNode != nullptr) {
			out << (*it).getElement() << " ";
			it++;
		}
	}
}

template <class T>
void Pointer_Queue<T>::PrintQueue(bool firstToLast, std::ostream &out) {
	Queue_Iterator<T> it;
	if (firstToLast) {
		it = this->getFrontIterator();
		it--;
		while ((*it).prevNode != nullptr) {
			out << *(*it).getElement() << " ";
			it--;
		}
	} else {
		it = this->getBackIterator();
		it++;
		while ((*it).nextNode != nullptr) {
			out << *(*it).getElement() << " ";
			it++;
		}
	}
}

#endif // !_QUEUE_H_
