#include <stdlib.h>
#include <stdio.h>
#include "Boolean.h"

typedef struct No{
    char item;
    int num;
    struct No * esq;
    struct No * dir;
} No;
typedef No* Arvore;

Arvore criarArvore( );
Arvore construirArvore(char,int, Arvore, Arvore);
int mostrarDicionarioHuff(Arvore, int, int);
int contarNos(Arvore);
bool isFolha(Arvore);


Arvore criarArvore( ){
    Arvore ap;
    ap = NULL;
    return ap;
}

Arvore construirArvore(char ch, int n, Arvore e, Arvore d){
    No *novo, *raiz;
    novo = malloc(sizeof(No));
    novo->item = ch; novo->num = n;
    novo->esq = e;    novo->dir = d;
    raiz = novo;
    return raiz;
}

int mostrarDicionarioHuff(Arvore ap, int e, int codigo){ // eRd
    int cod = (codigo*10) + e;
    if (!isFolha(ap)){
        cod = mostrarDicionarioHuff(ap->esq, 0, cod);
        cod = mostrarDicionarioHuff(ap->dir, 1, cod);
        cod = cod/10;
    } else {
        printf("%c   %d\n", ap->item, cod);
        cod = cod/10;
    }
    return cod;
}

bool isFolha(Arvore ap){
    bool aux = FALSE;
    if (ap->dir == NULL && ap->esq == NULL) aux = TRUE;
    return aux;
}
