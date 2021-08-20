#include <stdio.h>
#include <stdlib.h>
#include "Booleano.h"

#define fantasma '$'

typedef struct Celula {
    char caracter;
    int qntd;
    struct Celula *next;
}Celula;
typedef Celula * Lista;

Lista newLista();
bool ListaVazia(Lista);
int obterTamanhoLista(Lista);
char obterElemento(Lista, int);
Celula* buscarItem(Lista, int);
void mostrarLista(Lista);
Lista inserirLista(Lista, char, int);
Lista removerPrimeiro(Lista);
Lista esvaziarLista(Lista);
Lista removerUltimo(Lista);

Lista newLista(){
    return NULL;
}

bool ListaVazia(Lista ap){
    bool vazio = FALSE;
    if(ap == NULL) vazio = TRUE;
    return vazio;
}

int obterTamanhoLista(Lista ap){
    int tamanho;
    tamanho = 0;

    if (ap != NULL){
        Lista aux = ap;
        do {
            tamanho++;
            aux = aux->next;
        } while (aux != NULL);
    }

    return tamanho;
}


void mostrarLista(Lista ap){
    if(ap != NULL){
        Lista aux = ap;
        do{
            printf(" %c   %d\n", aux->caracter, aux->qntd);
            aux = aux->next;
        } while (aux != NULL);
    }
    printf("\n");
}

Lista inserirLista(Lista ap, char ch, int qntd){
    Lista aux;
    aux = (Lista) malloc(sizeof(Celula));
    aux->caracter = ch;
    aux->qntd = qntd;
    aux->next = NULL;

    if (ap == NULL){
        ap = aux;
    } else{
        aux->next = ap;
        ap = aux;
    }

    return ap;
}

Lista removerPrimeiro(Lista ap){
    if(ap != NULL){
        Lista aux = ap;
        ap = ap->next;
        free(aux);
    }
    return ap;
}

Lista esvaziarLista(Lista ap){
    Lista aux, auxFree;
    aux = ap;

    while(aux != NULL){
        auxFree = aux;
        aux = aux->next;
        free(auxFree);
    }

    return NULL;
}

Lista removerUltimo(Lista ap){
    Lista aux, auxIteracao1, auxIteracao2;
    aux = ap;

    if (ap == NULL){
    }
    else if (aux->next == NULL){
        free(aux);
        ap = NULL;
    }
    else {
        auxIteracao1 = ap;
        while (auxIteracao1->next != NULL){
            auxIteracao2 = auxIteracao1;
            auxIteracao1 = auxIteracao1->next;
        }
        free(auxIteracao1);
        auxIteracao2->next = NULL;
    }

    return ap;
}




