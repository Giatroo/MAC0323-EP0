#EP0 - Aeroporto

*Relatório do Exerício-Projeto 0 - Aeroporto - da matéria de MAC0323 - Algoritmos e Estruturas de Dados 2 - da Universidade de São Paulo, ministrada pelo Prof. Dr. Carlos E. Ferreira no ano de 2020. Esse relatório foi redigido pelo aluno Lucas Paiolla Forastiere - 11221911 - informando sobre as decisões de projeto e comportamento do programa em testes.*

## Sobre as decisões de projeto

### Implementação da Fila

Solicitou-se que os aviões fossem atendidos em uma estrutura de *fila*, onde o primeiro a fazer contato é o primeiro a sair da fila. Mas o problema conta com diversos casos emergenciais em que é preciso passar aviões na frente de outros e possivelmente realocar aviões que já estavam alocados nas filas.

Por esses motivos, a primeira decisão de projeto foi de fazer uma fila convencional, mas com acesso a pontos especificios para remoção e insersão no meio da fila. Todas essas decisões podem ser vistas no arquivo `Queue.h`. Nele há quatro classes: `QueueNode`, `Queue`, `Pointer_Queue` e `Queue_Iterator`.

Ou seja, implementou-se uma fila em lista ligada com tipo de dado abstrato, como o professor ensinou nas primeiras aulas do semestre. O único problema encontrado foi o fato de que a fila conteria ponteiros para aviões e, por isso, foi preciso criar uma classe `Pointer_Queue`, herdada de `Queue`, que dá override nos métodos necessários para adaptar o comportamento esperado de uma fila para uma fila de ponteiros.

Além disso, implementou-se a classe `Queue_Iterator` inspirada nos iteradores que são encontrados na *STL* do C++. Um *Queue_Iterator* é um "ponteiro" com a capacidade de percorrer a fila e guardar posições estratégicas para o funcionamento do programa. Com eles, foram possíveis de implementar os métodos de remoção e inserção no meio da fila: 
 * `T removeFrom(Queue_Iterator<T> &it);`
 * `void addIn(Queue_Iterator<T> *it, T e);`

Além do método
* `void iterate(const std::function<void(T)> &func);` , 
 
que usa internamente os iteradores para aplicar a cada elemento da fila a função `func`.

### Implementação do Avião

A implementação dos aviões pode ser encontrada nas classes `Plane.h` e `Plane.cpp`. O avião está implementado como uma classe que possui uma série de propriedades listadas e explicadas no arquivo `Plane.h`. A única que se deve chamar atenção é a propriedade `priority` que acabou por não ser utilizada na implementação final do Aeroporto (inicialmente a ideia era utilizar uma Fila de Prioridades, mas isso foi descartado).

Os únicos métodos que valem a pena ser mencionados são:
 * `void update();`
 * `Plane *createRandomPlane();`
 * `Plane *createUserPlane();`

, onde os dois últimos não fazem parte propriamente da classe `Plane`.

O `update` é um método que deve ser chamado a cada instante de tempo e é responsável por atualizar as propriedades do avião com base em sua categoria (se ele está pousando ou decolando - campo `flying`). Uma responsabilidade importante dele é fazer com que o avião se torne uma emergência caso a quantidade de combustível seja menor que o tempo que ele vai esperar para pousar (caso esteja voando) e também caso seu tempo de espera para decolagem passe de 10% do tempo de viagem esperado (caso esteja decolando).

Os métodos `createRadomPlane` e `createUserPlane` são usados respectivamente caso a simulação seja randômica ou não. O primeiro cria um avião randômico com base no banco de dados de possíveis companhias aéreas e destinos (além de sortear o número do avião etc.). Já o segundo vai pedir todas as informações para o usuário. Ambos retornam um ponteiro para o avião criado.

### Implementação do Aeroporto

A implementação do aeroporto foi feita em uma classe que pode ser encontrada nos arquivos `Airport.cpp` e `Airport.h`. A classe conta com uma série enorme de variáveis para mostrar estatísticas ao usuário e suas explicações podem ser encontradas em `Airport.h`. 

Vale a pena falar apenas dos campos:

* `Pointer_Queue<Plane *> queue[3]`;
* `int timeToBeFree[3]`; 
* `Queue_Iterator<Plane *> lastVIP[3]`;

No primeiro campo tempos um array com três filas, uma para cada pista. Essa é uma decisão de projeto bastante crucial, pois significa que vamos decidir em qual fila o avião vai pousar assim que ele se comunica com a torre. Outra solução possível poderia ser a de ter apenas uma única fila e passar a decisão de qual pista um avião utiliza para o momento em que ele está saindo da fila.

Prefiriu-se optar pela primeira solução. Dessa forma, pode-se saber exatamente quanto tempo levará para um avião sair da fila:

    * Se queue[i] está vazia, então o avião levará o tempo de manutenção da pista i,
    * caso contrário, o avião levará o tempo do último avião na fila + 3 unidades.

O `+3` refere-se a uma unidade para pouso/decolagem mais duas unidades de manutenção da fila.

Para que essa implementação dê certo, utilizamos o array `timeToBeFree[]` que é o tempo para que a pista `i` volte ao seu serviço. Um avião pode utilizar a pista `i` se, e somente se, `timeToBeFree[i] == 0` e, ao fazer isso, fazemos `timeToBeFree[i] = 3`, pois a fila fica duas unidades em manutenção e decrementamos seu valor ao fim de cada instante (ou seja, ao final do instante em que o avião pousou, `timeToBeFree[i]` tornar-se-á `2`). 

#### * Decisões sobre aviões emergenciais

Por fim, sobre os iteradores `lastVIP`. Como foram utilizadas uma fila para cada pista, uma decisão de projeto que segue naturalmente é manter a região frontal da fila para os aviões emergenciais (também chamados de *Very Important Planes* dentro do código). Ou seja, sempre que um avião emergencial contacta a torre, devemos escolher a fila em que ele levará menos tempo para sair e inseri-lo imediatamente após o último avião emergencial daquela fila (ou no começo da fila, caso ela não tenha nenhum avião emergencial).

Pois o array `lastVIP[i]` são iteradores que apontam exatamente para onde o último avião emergencial está localizado na fila `i` (ou para a cabeça frontal, caso não existam emergências em `queue[i]`).

Disso, segue o algoritmo que deduz o tempo que o avião emergencial levará para sair da fila `i`: 

    * Se lastVIP[i] == queue[i].getFrontIterator(), 
         então o avião levará o tempo de manutenção da pista i,
    * caso contrário, o avião levará o tempo de *lastVIP[i] + 3 unidades.

, onde `queue[i].getFrontIterator()` representa o iterador para a cabeça frontal e `*lastVIP[i]` indica o último avião emergencial da fila `i`.

Daí surgem algumas possibilidades e coisas a serem feitas:

Como um os aviões emergenciais entram na frente de outros aviões não-emergenciais, é preciso atualizar o tempo em que estes aviões levarão na fila e ver se algum deles passa a se tornar uma emergência. Ou seja, caso o avião esteja numa fila e esteja sobrevoando o aeroporto, mas, por conta de uma emergência, passou a não ter combustível o suficiente para esperar, então nós infelizmente o enviamos para outro aeroporto. Perceba que ao fazer isso, não deixamos que o avião chegue a zero de combustível, pois estamos sempre verificando se os aviões que estão na fila têm combustível suficiente para esperar sobrevoando até sua vez chegar.

Além disso, existe a possibilidade de o avião emergencial que acabou de chegar não ter combustível suficiente nem mesmo para esperar as outras emergências. Nesse caso, como recomendado pelo próprio professor, infelizmente enviamos essa emergência a outro aeroporto.

#### Sobre os métodos do aeroporto

Vale a pena citar apenas três:
 * `void addPlane(Plane *p);`
 * `void removePlane(int t_index);`
 * `void update();`

Os dois primeiros servem para adicionar e remover aviões das filas do aeroporto. O primeiro adiciona um avião (através de seu ponteiro) escolhendo a fila de acordo com os critérios já mencionados. O segundo remove um avião da fila `t_index` se a fila não estiver fora de serviço.

Já o método `update` é o método que vai gerenciar tudo o que deve ser feito em um instante de tempo dentro da simulação:
 * Saber (do usuário ou randômicamente) quantos aviões contatarão a torre;
 * Adicionar esses aviões gerados nas filas adequadas;
 * Remover os aviões que estão na frente de cada fila (se possível);
 * Decrementar o tempo de serviço das pistas;
 * Iterar sobre as filas atualizando cada avião (chamando o `update` de cada avião);
 * E, por fim, vendo se algum avião virou emergência e tomar a ação adequada.

## Sobre os testes solicitados pelos monitores

### Exemplo 1

#### Instante 0

##### Adicionando:

* O avião LA329 é adicionado na fila 2;
* O avião LA563 é adicionado na fila 1;
* O avião LA923 é adicionado na fila 1;
* O avião LA734 é adicionado na fila 2;

##### Removendo:

* O avião LA329 pousa na pista 2;
* O avião LA563 pousa na pista 1;


#### Instante 1

##### Filas:

```Instante: 1
Fila da pista 1:
Tempo de espera: 2
Avião LA923GRU: 
        Esperando por 1 unidades de tempo
        Tempo estimado para sair da fila 2 unidades de tempo
        Combustível - 3
 
Fila da pista 2:
Tempo de espera: 2
Avião LA734GRU: 
        Esperando por 1 unidades de tempo
        Tempo estimado para sair da fila 2 unidades de tempo
        Combustível - 3
 
Fila da pista 3:
Tempo de espera: 0
```
##### Adicionando:

Nenhum avião é adicionado.

##### Removendo:

Nenhum avião é removido.


#### Instante 2

##### Filas:

```
Instante: 2
Fila da pista 1:
Tempo de espera: 1
Avião LA923GRU: 
        Esperando por 2 unidades de tempo
        Tempo estimado para sair da fila 1 unidades de tempo
        Combustível - 2
 
Fila da pista 2:
Tempo de espera: 1
Avião LA734GRU: 
        Esperando por 2 unidades de tempo
        Tempo estimado para sair da fila 1 unidades de tempo
        Combustível - 2
 
Fila da pista 3:
Tempo de espera: 0
```
##### Adicionando:

Nenhum avião é adicionado.

##### Removendo:

Nenhum avião é removido.


#### Instante 3

##### Filas:

```
Instante: 3
Fila da pista 1:
Tempo de espera: 0
Avião LA923GRU: 
        Esperando por 3 unidades de tempo
        Tempo estimado para sair da fila 0 unidades de tempo
        Combustível - 1
 
Fila da pista 2:
Tempo de espera: 0
Avião LA734GRU: 
        Esperando por 3 unidades de tempo
        Tempo estimado para sair da fila 0 unidades de tempo
        Combustível - 1
 
Fila da pista 3:
Tempo de espera: 0
```

##### Adicionando:

Nenhum avião é adicionado.

##### Removendo:

* O avião LA923 pousa na pista 1;
* O avião LA734 pousa na pista 2;


### Exemplo 2:

#### Instante 0:

##### Adicionando:

* O avião LA329 é adicioando na fila 2;
* O avião LA563 é adicionado na fila 1;
* O avião LA923 é adicionado (como emergência) na fila 3;
* O avião LA734 é adicionado (como emergência) na fila 2 (na frente de LA563);

##### Removendo:

* O avião LA563 pousa na fila 1;
* O avião LA734 pousa na fila 2 (emergência);
* O avião LA923 pousa na fila 3 (emergência);


#### Instante 1:

##### Filas:

```
Instante: 1
Fila da pista 1:
Tempo de espera: 2

Fila da pista 2:
Tempo de espera: 2

Fila da pista 3:
Tempo de espera: 2
```

(Note que o avião LA563 é desviado para aeroporto vizinho)

##### Adicionando:

Nenhum avião é adicionado.

##### Removendo:

Nenhum avião é removido.


### Exemplo 3

#### Instante 0:

##### Adicionando:

* O avião LA329 é adicionado na fila 3;
* O avião LA563 é adicionado na fila 2;
* O avião LA923 é adicionado na fila 1;
* O avião LA734 é adicionado na fila 1;
* O avião LA140 é adicionado na fila 2;
* O avião JB666 é adicionado na fila 3 (na frente de LA329);
* O avião LA832 é adicionado na fila 2 (na frente de LA563 e LA140)

##### Removendo:

* O avião LA923 decola na pista 1;
* O avião LA832 pouca emergencialmente na pista 2;
* O avião JB666 decola emergencialmente na pista 3;

#### Instante 1:

##### Fila:

```
Instante: 1
Fila da pista 1:
Tempo de espera: 2
Avião LA734AQP: 
        Esperando por 1 unidades de tempo
        Tempo estimado para sair da fila 2 unidades de tempo
        Tempo de voo estimado - 40
 
Fila da pista 2:
Tempo de espera: 2
Avião LA563ADZ: 
        Esperando por 1 unidades de tempo
        Tempo estimado para sair da fila 2 unidades de tempo
        Tempo de voo estimado - 20
 Avião LA140BOG: 
        Esperando por 1 unidades de tempo
        Tempo estimado para sair da fila 5 unidades de tempo
        Tempo de voo estimado - 60
 
Fila da pista 3:
Tempo de espera: 2
Avião LA329ACA: 
        Esperando por 1 unidades de tempo
        Tempo estimado para sair da fila 2 unidades de tempo
        Tempo de voo estimado - 20

```

##### Adicionando:

Nenhum avião é adicionado.

##### Removendo:

Nenhum avião é removido.

#### Instante 2:

##### Fila:

```
Instante: 2
Fila da pista 1:
Tempo de espera: 1
Avião LA734AQP: 
        Esperando por 2 unidades de tempo
        Tempo estimado para sair da fila 1 unidades de tempo
        Tempo de voo estimado - 40
 
Fila da pista 2:
Tempo de espera: 1
Avião LA563ADZ: 
        Esperando por 2 unidades de tempo
        Tempo estimado para sair da fila 1 unidades de tempo
        Tempo de voo estimado - 20
 Avião LA140BOG: 
        Esperando por 2 unidades de tempo
        Tempo estimado para sair da fila 4 unidades de tempo
        Tempo de voo estimado - 60
 
Fila da pista 3:
Tempo de espera: 1
Avião LA329ACA: 
        Esperando por 2 unidades de tempo
        Tempo estimado para sair da fila 1 unidades de tempo
        Tempo de voo estimado - 20
```

##### Adicionando:

Nenhum avião é adicionado.

##### Removendo:

Nenhum avião é removido.

#### Instante 3:

##### Fila:

```
Instante: 3
Fila da pista 1:
Tempo de espera: 0
Avião LA329ACA:  VIP
        Esperando por 3 unidades de tempo
        Tempo estimado para sair da fila 0 unidades de tempo
        Tempo de voo estimado - 20
 Avião LA734AQP: 
        Esperando por 3 unidades de tempo
        Tempo estimado para sair da fila 3 unidades de tempo
        Tempo de voo estimado - 40
 
Fila da pista 2:
Tempo de espera: 0
Avião LA140BOG: 
        Esperando por 3 unidades de tempo
        Tempo estimado para sair da fila 3 unidades de tempo
        Tempo de voo estimado - 60
 
Fila da pista 3:
Tempo de espera: 0
Avião LA563ADZ:  VIP
        Esperando por 3 unidades de tempo
        Tempo estimado para sair da fila 0 unidades de tempo
        Tempo de voo estimado - 20
```

Note que LA563 e LA329 se tornaram emergenciais, pois atingiram 10% do tempo de vôo. Devido a isso foram realocados.

##### Adicionando:

Nenhum avião é adicionado.

##### Removendo:

* LA329 decola na pista 1;
* LA140 decola na pista 2;
* LA563 decola na pista 3;

#### Instante 4:

##### Fila:

```
Instante: 4
Fila da pista 1:
Tempo de espera: 2
Avião LA734AQP: 
        Esperando por 4 unidades de tempo
        Tempo estimado para sair da fila 2 unidades de tempo
        Tempo de voo estimado - 40
 
Fila da pista 2:
Tempo de espera: 2

Fila da pista 3:
Tempo de espera: 2
```

##### Adicionando:

Nenhum avião é adicionado.

##### Removendo:

Nenhum avião é removido.

#### Instante 5:

##### Fila:

```
Instante: 5
Fila da pista 1:
Tempo de espera: 1

Fila da pista 2:
Tempo de espera: 1

Fila da pista 3:
Tempo de espera: 1
Avião LA734AQP:  VIP
        Esperando por 5 unidades de tempo
        Tempo estimado para sair da fila 1 unidades de tempo
        Tempo de voo estimado - 40
```

Note que LA734 acabou sendo realocado, pois se tornou uma emergência.

##### Adicionando:

* O avião LA485 foi adicionado na fila 2;
* O avião LA300 foi adicionado na fila 1;
* O avião LA887 foi adicionado na fila 1;
* O avião LA993 foi adicionado na fila 2;
* O avião LA554 foi adicionado na fila 1;
* O avião LA111 foi adicionado na fila 3;

##### Removendo:

Nenhum avião é removido.

#### Instante 6:

##### Fila:

```
Instante: 6
Fila da pista 1:
Tempo de espera: 0
Avião LA300GRU: 
        Esperando por 1 unidades de tempo
        Tempo estimado para sair da fila 0 unidades de tempo
        Combustível - 9
 Avião LA887GRU: 
        Esperando por 1 unidades de tempo
        Tempo estimado para sair da fila 3 unidades de tempo
        Combustível - 9
 Avião LA554GRU: 
        Esperando por 1 unidades de tempo
        Tempo estimado para sair da fila 6 unidades de tempo
        Combustível - 9
 
Fila da pista 2:
Tempo de espera: 0
Avião LA485GRU: 
        Esperando por 1 unidades de tempo
        Tempo estimado para sair da fila 0 unidades de tempo
        Combustível - 9
 Avião LA993GRU: 
        Esperando por 1 unidades de tempo
        Tempo estimado para sair da fila 3 unidades de tempo
        Combustível - 9
 
Fila da pista 3:
Tempo de espera: 0
Avião LA734AQP:  VIP
        Esperando por 6 unidades de tempo
        Tempo estimado para sair da fila 0 unidades de tempo
        Tempo de voo estimado - 40
 Avião LA111CCM: 
        Esperando por 1 unidades de tempo
        Tempo estimado para sair da fila 3 unidades de tempo
        Tempo de voo estimado - 100
```

##### Adicionando:

* Nenhum avião adicionado;

##### Removendo:

* LA300 pousa na pista 1;
* LA485 pousa na pista 2;
* LA734 decola na pista 3;

#### Instante 7:

##### Fila:

```
Instante: 7
Fila da pista 1:
Tempo de espera: 2
Avião LA887GRU: 
        Esperando por 2 unidades de tempo
        Tempo estimado para sair da fila 2 unidades de tempo
        Combustível - 8
 Avião LA554GRU: 
        Esperando por 2 unidades de tempo
        Tempo estimado para sair da fila 5 unidades de tempo
        Combustível - 8
 
Fila da pista 2:
Tempo de espera: 2
Avião LA993GRU: 
        Esperando por 2 unidades de tempo
        Tempo estimado para sair da fila 2 unidades de tempo
        Combustível - 8
 
Fila da pista 3:
Tempo de espera: 2
Avião LA111CCM: 
        Esperando por 2 unidades de tempo
        Tempo estimado para sair da fila 2 unidades de tempo
        Tempo de voo estimado - 100
```

##### Adicionando:

* O avião LA344 foi adicionado na fila 2;
* O avião LA461 foi adicionado na fila 1;
* O avião LA875 foi adicionado na fila 3;

##### Removendo:

* Nenhum avião removido;

#### Instante 8:

##### Fila:

```
Instante: 8
Fila da pista 1:
Tempo de espera: 1
Avião LA887GRU: 
        Esperando por 3 unidades de tempo
        Tempo estimado para sair da fila 1 unidades de tempo
        Combustível - 7
 Avião LA554GRU: 
        Esperando por 3 unidades de tempo
        Tempo estimado para sair da fila 4 unidades de tempo
        Combustível - 7
 Avião LA461GRU: 
        Esperando por 1 unidades de tempo
        Tempo estimado para sair da fila 7 unidades de tempo
        Combustível - 8
 
Fila da pista 2:
Tempo de espera: 1
Avião LA993GRU: 
        Esperando por 3 unidades de tempo
        Tempo estimado para sair da fila 1 unidades de tempo
        Combustível - 7
 Avião LA344GRU: 
        Esperando por 1 unidades de tempo
        Tempo estimado para sair da fila 4 unidades de tempo
        Combustível - 4
 
Fila da pista 3:
Tempo de espera: 1
Avião LA111CCM: 
        Esperando por 3 unidades de tempo
        Tempo estimado para sair da fila 1 unidades de tempo
        Tempo de voo estimado - 100
 Avião LA875CGB: 
        Esperando por 1 unidades de tempo
        Tempo estimado para sair da fila 4 unidades de tempo
        Tempo de voo estimado - 40
```

##### Adicionando:

* Nenhum avião adicionado;

##### Removendo:

* Nenhum avião removido;

#### Instante 9:

##### Fila:

```
Instante: 9
Fila da pista 1:
Tempo de espera: 0
Avião LA887GRU: 
        Esperando por 4 unidades de tempo
        Tempo estimado para sair da fila 0 unidades de tempo
        Combustível - 6
 Avião LA554GRU: 
        Esperando por 4 unidades de tempo
        Tempo estimado para sair da fila 3 unidades de tempo
        Combustível - 6
 Avião LA461GRU: 
        Esperando por 2 unidades de tempo
        Tempo estimado para sair da fila 6 unidades de tempo
        Combustível - 7
 
Fila da pista 2:
Tempo de espera: 0
Avião LA993GRU: 
        Esperando por 4 unidades de tempo
        Tempo estimado para sair da fila 0 unidades de tempo
        Combustível - 6
 Avião LA344GRU: 
        Esperando por 2 unidades de tempo
        Tempo estimado para sair da fila 3 unidades de tempo
        Combustível - 3
 
Fila da pista 3:
Tempo de espera: 0
Avião LA111CCM: 
        Esperando por 4 unidades de tempo
        Tempo estimado para sair da fila 0 unidades de tempo
        Tempo de voo estimado - 100
 Avião LA875CGB: 
        Esperando por 2 unidades de tempo
        Tempo estimado para sair da fila 3 unidades de tempo
        Tempo de voo estimado - 40
```

##### Adicionando:

* Nenhum avião adicionado;

##### Removendo:

* O avião LA887 pousa na pista 1;
* O avião LA993 pousa na pista 2;
* O avião LA111 decola na pista 3;

#### Instante 10:

##### Fila:

```
Instante: 10
Fila da pista 1:
Tempo de espera: 2
Avião LA554GRU: 
        Esperando por 5 unidades de tempo
        Tempo estimado para sair da fila 2 unidades de tempo
        Combustível - 5
 Avião LA461GRU: 
        Esperando por 3 unidades de tempo
        Tempo estimado para sair da fila 5 unidades de tempo
        Combustível - 6
 
Fila da pista 2:
Tempo de espera: 2
Avião LA344GRU: 
        Esperando por 3 unidades de tempo
        Tempo estimado para sair da fila 2 unidades de tempo
        Combustível - 2
 
Fila da pista 3:
Tempo de espera: 2
Avião LA875CGB: 
        Esperando por 3 unidades de tempo
        Tempo estimado para sair da fila 2 unidades de tempo
        Tempo de voo estimado - 40
```

##### Adicionando:

* Nenhum avião adicionado;

##### Removendo:

* Nenhuma avião removido;


#### Instante 11:

##### Fila:

```
Instante: 11
Fila da pista 1:
Tempo de espera: 1
Avião LA554GRU: 
        Esperando por 6 unidades de tempo
        Tempo estimado para sair da fila 1 unidades de tempo
        Combustível - 4
 Avião LA461GRU: 
        Esperando por 4 unidades de tempo
        Tempo estimado para sair da fila 4 unidades de tempo
        Combustível - 5
 
Fila da pista 2:
Tempo de espera: 1
Avião LA344GRU: 
        Esperando por 4 unidades de tempo
        Tempo estimado para sair da fila 1 unidades de tempo
        Combustível - 1
 
Fila da pista 3:
Tempo de espera: 1
Avião LA875CGB: 
        Esperando por 4 unidades de tempo
        Tempo estimado para sair da fila 1 unidades de tempo
        Tempo de voo estimado - 40
```

##### Adicionando:

* Nenhum avião adicionado;

##### Removendo:

* Nenhuma avião removido;

#### Instante 12:

##### Fila:

```
Instante: 12
Fila da pista 1:
Tempo de espera: 0
Avião LA554GRU: 
        Esperando por 7 unidades de tempo
        Tempo estimado para sair da fila 0 unidades de tempo
        Combustível - 3
 Avião LA461GRU: 
        Esperando por 5 unidades de tempo
        Tempo estimado para sair da fila 3 unidades de tempo
        Combustível - 4
 
Fila da pista 2:
Tempo de espera: 0
Avião LA344GRU: 
        Esperando por 5 unidades de tempo
        Tempo estimado para sair da fila 0 unidades de tempo
        Combustível - 0
 
Fila da pista 3:
Tempo de espera: 0
Avião LA875CGB:  VIP
        Esperando por 5 unidades de tempo
        Tempo estimado para sair da fila 0 unidades de tempo
        Tempo de voo estimado - 40
```

Note que LA875 tornou-se uma emergência e foi realocado (mas calhou de a melhor fila para ele ser a que ele já estava).

##### Adicionando:

* Nenhum avião adicionado;

##### Removendo:

* O avião LA554 pousa na pista 1;
* O avião LA344 pousa (com 0 de combustível) na pista 2;
* O avião LA875 decola na pista 3;

#### Instante 13:

##### Fila:

```
Instante: 13
Fila da pista 1:
Tempo de espera: 2
Avião LA461GRU: 
        Esperando por 6 unidades de tempo
        Tempo estimado para sair da fila 2 unidades de tempo
        Combustível - 3
 
Fila da pista 2:
Tempo de espera: 2

Fila da pista 3:
Tempo de espera: 2
```

##### Adicionando:

* Nenhum avião adicionado;

##### Removendo:

* Nenhum avião removido;

#### Instante 14:

##### Fila:

```
Instante: 14
Fila da pista 1:
Tempo de espera: 1
Avião LA461GRU: 
        Esperando por 7 unidades de tempo
        Tempo estimado para sair da fila 1 unidades de tempo
        Combustível - 2
 
Fila da pista 2:
Tempo de espera: 1

Fila da pista 3:
Tempo de espera: 1
```

##### Adicionando:

* Nenhum avião adicionado;

##### Removendo:

* Nenhum avião removido;

#### Instante 15:

##### Fila:

```
Instante: 15
Fila da pista 1:
Tempo de espera: 0
Avião LA461GRU: 
        Esperando por 8 unidades de tempo
        Tempo estimado para sair da fila 0 unidades de tempo
        Combustível - 1
 
Fila da pista 2:
Tempo de espera: 0

Fila da pista 3:
Tempo de espera: 0
```

##### Adicionando:

* O avião LA673 foi adicionado na fila 3;
* O avião LA899 foi adicionado na fila 2;
* O avião LA505 foi adicionado na fila 1;

##### Removendo:

* O avião LA673 decolou na pista 3;
* O avião LA899 decolou na pista 2;
* O avião LA505 pousou na pista 1;

#### Instante 16:

##### Fila:

```
Instante: 16
Fila da pista 1:
Tempo de espera: 2

Fila da pista 2:
Tempo de espera: 2

Fila da pista 3:
Tempo de espera: 2
```

Note que o avião LA461 foi redirecionado com 1 de combustível para outro aeroporto.

##### Adicionando:

* Nenhum avião adicionado;

##### Removendo:

* Nenhum avião removido;

Daí em diante, nenhum avião chega ou sai do aeroporto.