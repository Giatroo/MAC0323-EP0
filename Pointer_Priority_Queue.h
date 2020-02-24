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

#include "Priority_Queue.h"

template <class T>
class Pointer_Priority_Queue : public Priority_Queue<T> {
	// Precisamos dar overwrite nas funções upheap, downheap e debug
	// pois são as únicas que usam os conteúdos dos ponteiros que estamos
	// armazenando
  private:
	void upheap(int i);
	void downheap(int i);

  public:
	void debug();
};

template <class T>
void Pointer_Priority_Queue<T>::upheap(int i) {
	if (i < 0 || i >= this->MAX) {
		std::cout << "Erro no upheap: " << i << " não é uma posição válida!"
		          << std::endl;
		return;
	}

	T v = this->item[i];
	while (i != 1 && *this->item[i / 2] <= *v) {
		this->item[i] = this->item[i / 2];
		i = i / 2;
	}
	this->item[i] = v;
}

template <class T>
void Pointer_Priority_Queue<T>::downheap(int i) {
	if (i < 0 || i >= this->MAX) {
		std::cout << "Erro no upheap: " << i << " não é uma posição válida!"
		          << std::endl;
		return;
	}

	T v = this->item[i];
	while (2 * i <= this->numItems) {
		int max = 2 * i;
		if (max < this->numItems && *this->item[max] < *this->item[max + 1])
			max++;
		if (*v >= *this->item[max]) break;
		this->item[i] = this->item[max];
		i = max;
	}
	this->item[i] = v;
}

template <class T>
void Pointer_Priority_Queue<T>::debug() {
	std::cout << "Heap: [ ";
	for (int i = 1; i <= this->numItems; i++)
		std::cout << *this->item[i] << ((i != this->numItems) ? ", " : "");
	std::cout << " ]\n";
}