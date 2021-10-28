#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

/*

* Matheus Morais Almeida de Modesti - 4199569-4
* Bárbara Teixeira Duarte – 41615190


*/


#ifdef _OpenMP
int my_rank = omp_get_thread_num();
int thread_count = omp_get_num_threads();
#else
int my_rank = 1;
int thread_count = 1;
#endif

#define N_ARRAY 10000  // Número de elementos do vetor
#define QNT_BUCKET 100  // Quantidade de bucketss
#define INTERVALO 100  // INTERVALO de elementos para cada buckets

struct Node {
  int data;
  struct Node *next;
};

void BucketSort(int sort[]);
struct Node *InsertionSort(struct Node *list);
void print(int sort[]);
void printBuckets(struct Node *list);
int getBucketIndex(int valor);

void BucketSort(int sort[]) {
  int i, j;
  struct Node **buckets;

  // Criação dos buckets
  buckets = (struct Node **)malloc(sizeof(struct Node *) * QNT_BUCKET);

  // Começo da região paralela
  #pragma omp parallel num_threads(omp_get_num_threads())
  {
    // Inicializa os bucketss vazios
    #pragma omp for schedule(static)
    for (i = 0; i < QNT_BUCKET; ++i) {
      buckets[i] = NULL;
    }

    // Preenche os bucketss vazios com seus respectivos elementos pertencentes ao seu INTERVALO.
    #pragma omp for schedule(static)
    for (i = 0; i < N_ARRAY; ++i) {
        struct Node *atual;
        int pos = getBucketIndex(sort[i]);
        atual = (struct Node *)malloc(sizeof(struct Node));
        atual->data = sort[i];
        atual->next = buckets[pos];
        buckets[pos] = atual;
    }

    // Imprime os elementos de cada buckets
    #pragma omp single
    for (i = 0; i < QNT_BUCKET; i++) {
      printf("Bucket[%d]: ", i);
      printBuckets(buckets[i]);
      printf("\n");
    }

    // Ordena os elementos de cada buckets
    #pragma omp for schedule(dynamic)
    for (i = 0; i < QNT_BUCKET; ++i) {
      buckets[i] = InsertionSort(buckets[i]);
    }

    printf("-------------\n");
    printf("Bucktets after sorting\n");
    #pragma omp single
    for (i = 0; i < QNT_BUCKET; i++) {
      printf("Bucket[%d]: ", i);
      printBuckets(buckets[i]);
      printf("\n");
    }

    // Concantenação dos elementos no vetor
    #pragma omp single
    for (j = 0, i = 0; i < QNT_BUCKET; ++i) {
      struct Node *node;
      node = buckets[i];
      while (node) {
        sort[j++] = node->data;
        node = node->next;
      }
  }

  }
  return;
}

// Ordenação de cada buckets feite pelo Insertion Sort
struct Node *InsertionSort(struct Node *list) {
  struct Node *k, *nodeList;
  if (list == 0 || list->next == 0) {
    return list;
  }

  nodeList = list;
  k = list->next;
  nodeList->next = 0;
  while (k != 0) {
    struct Node *ptr;
    if (nodeList->data > k->data) {
      struct Node *tmp;
      tmp = k;
      k = k->next;
      tmp->next = nodeList;
      nodeList = tmp;
      continue;
    }

    for (ptr = nodeList; ptr->next != 0; ptr = ptr->next) {
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
  return nodeList;
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

// Imprime os buckets
void printBuckets(struct Node *list) {
  struct Node *cur = list;
  while (cur) {
    printf("%d ", cur->data);
    cur = cur->next;
  }
}


int main(void) {
  int array[N_ARRAY], i;

   srand(time(NULL));
   for(i=0; i < N_ARRAY; i++)
      array[i]= (int)rand() / (int)(RAND_MAX/ 10000);

  printf("Array Inicial: ");
  print(array);
  printf("-------------\n");

  BucketSort(array);
  printf("-------------\n");
  printf("Array Ordenado: ");
  print(array);
  printf("-------------\n");
  printf("\nFIM DE LEITURA");
  return 0;
}