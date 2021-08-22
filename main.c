#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Lista.h"
#include "Arvore.h"

char *lerArquivo(char *);
int listaFrequencia(char *, struct Celula **);
Arvore arvoreHuffman(struct Celula *, int);
int listaHuff(Arvore, int, int, struct Celula **);
void mergeSort(struct Celula **);
struct Celula *sortedMerge(struct Celula *, struct Celula*);
void splitList(struct Celula *, struct Celula **, struct Celula **);

int main() {
    struct Celula * a = NULL, * listaCodigos = NULL;
    int tamanho;
    char * texto;

    texto = lerArquivo("..\\texto\\texto.txt");
    tamanho = listaFrequencia(texto, &a);
    Arvore huff = arvoreHuffman(a, tamanho);
    listaHuff(huff, 0, 0, &listaCodigos);
    mostrarLista(listaCodigos);
    return 0;
}

char *lerArquivo(char * fileName) {
    FILE *file = fopen(fileName, "r");
    char *code;
    size_t n = 0;
    int c;

    if (file == NULL) return NULL;
    fseek(file, 0, SEEK_END);
    long f_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    code = malloc(f_size);

    while ((c = fgetc(file)) != EOF) {
        code[n++] = (char)c;
    }

    code[n] = '\0';

    return code;
}

//funcao que analiza quais e quantas vezes os caracteres aparecem em uma string
//devolve uma lista ligada em ordem crescente em relacao a quantidade de vezes que o caracter aparece no texto
//parametros = uma string para ser analizada

int listaFrequencia(char *texto, struct Celula ** listaFrequencia){
    struct Celula * aux, * auxQntd;
    int i;
    char auxChar;
    size_t nT = strlen(texto), charEncontrados = 0;
    bool charNovo;

    //loop que itera sobre o texto e guarda em um array de caracteres e em um array de inteiros os caracteres e
    // suas frequencias respectivamente
    for (i = 0; i < nT; ++i) {
        charNovo = TRUE;
        aux = *listaFrequencia;

        while (aux != NULL && charNovo) {
            if (texto[i] == aux->caracter) charNovo = FALSE;
            auxQntd = aux;
            aux = aux->next;
        }
        if (charNovo){
            inserir(listaFrequencia, texto[i], 1);
            charEncontrados++;
        } else auxQntd->qntd = auxQntd->qntd + 1;
    }

    //organiza a lista encadeada que guarda os caracteres e suas frequencia com o algoritmo mergeSort
    mergeSort(listaFrequencia);

    return charEncontrados;
}

Arvore arvoreHuffman(struct Celula * listaFrequencia, int tamanho){
    int i;
    int inicioListaHuffman = 0;
    Arvore huffman[tamanho];
    struct Celula * aux;
    Arvore auxE, auxD;

    aux = listaFrequencia;
    for (i = 0; i < tamanho; i++) {
        huffman[i] = construirArvore(aux->caracter, aux->qntd, NULL, NULL);
        aux = aux->next;
    }

    while (inicioListaHuffman < (tamanho-1)){
        int qntd = huffman[inicioListaHuffman]->num + huffman[inicioListaHuffman+1]->num;
        huffman[inicioListaHuffman+1] = construirArvore('\0', qntd, huffman[inicioListaHuffman], huffman[inicioListaHuffman+1]);
        inicioListaHuffman++;

        int k = inicioListaHuffman;
        Arvore auxHuff;
        while (k + 1 < tamanho && huffman[k]->num > huffman[k+1]->num){
            if (huffman[k]->num > huffman[k+1]->num){
                auxHuff = huffman[k];
                huffman[k] = huffman[k+1];
                huffman[k+1] = auxHuff;
            }
            k++;
        }
    }

    return huffman[inicioListaHuffman];
}

int listaHuff(Arvore ap, int e, int codigo, struct Celula ** lp){
    int cod = (codigo*10) + e;
    if (!isFolha(ap)){
        cod = listaHuff(ap->esq, 0, cod, lp);
        cod = listaHuff(ap->dir, 1, cod, lp);
        cod = cod/10;
    } else {
        inserir(lp, ap->item, cod);
        cod = cod/10;
    }
    return cod;
}

void mergeSort(struct Celula** referenciaInicio){
    struct Celula* inicio = *referenciaInicio;
    struct Celula* esquerda, * direita;
    if ((inicio != NULL) && (inicio->next != NULL)){
        splitList(inicio, &esquerda, &direita);
        mergeSort(&esquerda);
        mergeSort(&direita);
        *referenciaInicio = sortedMerge(esquerda, direita);
    }
}

struct Celula* sortedMerge(struct Celula* e, struct Celula* d){
    struct Celula* aux = NULL;

    if (e == NULL){
        aux = d;
    } else if (d == NULL){
        aux = e;
    } else{
        if ( e->qntd <= d->qntd){
            aux = e;
            aux->next = sortedMerge(e->next, d);
        }
        else{
            aux = d;
            aux->next = sortedMerge(e, d->next);
        }
    }

    return aux;
}

void splitList(struct Celula* inicio, struct Celula** listaE, struct Celula** listaD){
    struct Celula* rapido;
    struct Celula* devagar;
    if (inicio==NULL || inicio->next==NULL){
        *listaE = inicio;
        *listaD = NULL;
    }
    else{
        devagar = inicio;
        rapido = inicio->next;
        while (rapido != NULL){
            rapido = rapido->next;
            if (rapido != NULL){
                devagar = devagar->next;
                rapido = rapido->next;
            }
        }

        *listaE = inicio;
        *listaD = devagar -> next;
        devagar->next = NULL;
    }
}