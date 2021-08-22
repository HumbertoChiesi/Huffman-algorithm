#include<stdio.h>
#include<stdlib.h>

struct Celula{
    char caracter;
    int qntd;
    struct Celula* next;
};

void mostrarLista(struct Celula *);
void inserir(struct Celula **, char, int);

void mostrarLista(struct Celula *node){
    while(node != NULL)
    {
        printf("%c   %d\n", node->caracter, node->qntd);
        node = node -> next;
    }
}

void inserir(struct Celula** head_ref, char ch, int qntd){
    struct Celula* new_node = (struct Celula*) malloc(sizeof(struct Celula));
    new_node->caracter  = ch;
    new_node->qntd = qntd;
    new_node->next = (*head_ref);
    (*head_ref) = new_node;
}



