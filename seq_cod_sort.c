#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N_ARRAY 10000  // tamanho arry
#define QNT_BUCKET 100  // numero de buckets
#define INTERVALO 100  // capacidade bucket 

/*

* Matheus Morais Almeida de Modesti - 4199569-4
* Bárbara Teixeira Duarte – 41615190


*/

struct Node {
  int data;
  struct Node *next;
};

void BucketSort(int sort[]);
struct Node *InsertionSort(struct Node *listaa);
void print(int sort[]);
void printBuckets(struct Node *lista);
int getBucketIndex(int valor);

// funcao de ordenacao 
void BucketSort(int sort[]) {
  int i, j;
  struct Node **buckets;

  // criando buckets
  buckets = (struct Node **)malloc(sizeof(struct Node *) * QNT_BUCKET);

  // iniciando buckets vazios

  for (i = 0; i < QNT_BUCKET; ++i) {
    buckets[i] = NULL;
  }

  // preenche os buckets com elementos correspondentes
   for (i = 0; i < N_ARRAY; ++i) {
      struct Node *atual;
      int pos = getBucketIndex(sort[i]);
      atual = (struct Node *)malloc(sizeof(struct Node));
      atual->data = sort[i];
      atual->next = buckets[pos];
      buckets[pos] = atual;
   }



  // imprime buckets com seus respectivos elementos
  printf("\nBucktets Iniciais: \n");
  for (i = 0; i < QNT_BUCKET; i++) {
    printf("Bucket[%d]: ", i);
    printBuckets(buckets[i]);
    printf("\n");
  }

  // classifica os elementos nos buckets
  for (i = 0; i < QNT_BUCKET; ++i) {
    buckets[i] = InsertionSort(buckets[i]);
  }

  printf("-------------\n");
  printf("\nBucktets Ordenados\n");
  for (i = 0; i < QNT_BUCKET; i++) {
    printf("Bucket[%d]: ", i);
    printBuckets(buckets[i]);
    printf("\n");
  }

  // coloca elementos em sort
  for (j = 0, i = 0; i < QNT_BUCKET; ++i) {
    struct Node *node;
    node = buckets[i];
    while (node) {
      sort[j++] = node->data;
      node = node->next;
    }
  }

  
  return;
}

//funcao que classifica elementos dos buckets
struct Node *InsertionSort(struct Node *lista) {
  struct Node *k, *nodelista;
  if (lista == 0 || lista->next == 0) {
    return lista;
  }

  nodelista = lista;
  k = lista->next;
  nodelista->next = 0;
  while (k != 0) {
    struct Node *ptr;
    if (nodelista->data > k->data) {
      struct Node *tmp;
      tmp = k;
      k = k->next;
      tmp->next = nodelista;
      nodelista = tmp;
      continue;
    }

    for (ptr = nodelista; ptr->next != 0; ptr = ptr->next) {
      if (ptr->next->data > k->data)
        break;
    }

    if (ptr->next != 0) {
      struct Node *tmp;
      tmp = k;
      k = k->next;
      tmp->next = ptr->next;
      ptr->next = tmp;
      continue;
    } else {
      ptr->next = k;
      k = k->next;
      ptr->next->next = 0;
      continue;
    }
  }
  return nodelista;
}

int getBucketIndex(int valor) {
  return valor / INTERVALO;
}

void print(int ar[]) {
  int i;
  for (i = 0; i < N_ARRAY; ++i) {
    printf("%d ", ar[i]);
  }
  printf("\n");
}

// imprime buckets
void printBuckets(struct Node *lista) {
  struct Node *cur = lista;
  while (cur) {
    printf("%d ", cur->data);
    cur = cur->next;
  }
}

// codigo main
int main(void) {
  int array[N_ARRAY], i;

   srand(time(NULL));
   for(i=0; i < N_ARRAY; i++)
      array[i]= (int)rand() / (int)(RAND_MAX/ 10000);

  printf("\nArray Incial: \n");
  print(array);
  printf("-------------\n");

  BucketSort(array);
  printf("-------------\n");
  printf("\nArray Ordenado: ");
  print(array);
  printf("-------------\n");
  printf("\nFIM DE LEITURA");
  return 0;
}


