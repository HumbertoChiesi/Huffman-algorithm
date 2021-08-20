//
// Created by user on 18/08/2021.
//
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "ListaPointer.h"
#include "Booleano.h"
#include "ArvorePointer.h"

#define ASCII 256

//estrutura lista ligada


Lista listaFrequencia(char *);
Arvore arvoreHuffman(Lista);


int main(){
    char *texto;
    texto = "TAATTAATTAATTAATTAGATAGATAGATAGAACGTACGTACGTACGTTATATATAACCGACCG";
    //texto = "ABRACADABRA!";
    int p[ASCII];
    Lista humberto = newLista();
    Arvore huff;
    humberto = listaFrequencia(texto);
    mostrarLista(humberto);
    huff = arvoreHuffman(humberto);
    mostrarDicionarioHuff(huff, 0, 0);
}

//devolve uma lista ligada em ordem crescente em relacao a quantidade de vezes que o caracter aparece no texto
Lista listaFrequencia(char *texto){
    char caracteres[ASCII];
    int frequenciaChar[ASCII];
    Lista listaFrequencia = newLista();
    int i, k, aux;
    char auxChar;
    size_t nT = strlen(texto), charEncontrados = 0;
    bool charNovo = TRUE;

    //loop que itera sobre o texto e guarda quais caracteres e quantas vezes eles apareceram no texto
    for (i = 0; i < nT; ++i) {
        charNovo = TRUE;
        k = 0;

        while (k < charEncontrados && charNovo) {
            if (texto[i] == caracteres[k]) charNovo = FALSE;
            k++;
        }
        if (charNovo){
            caracteres[charEncontrados] = texto[i];
            frequenciaChar[charEncontrados] = 1;
            charEncontrados++;
        } else frequenciaChar[k-1]++;
    }

    //loop para ordenar os caracteres em ordem crescente em relacao a frequencia no texto
    for (i = 0; i < charEncontrados; i++) {
        k = i;
        while (k>0 && frequenciaChar[k] < frequenciaChar[k-1]){
            aux = frequenciaChar[k];
            auxChar = caracteres[k];
            frequenciaChar[k] = frequenciaChar[k-1];
            frequenciaChar[k-1] = aux;
            caracteres[k] = caracteres[k-1];
            caracteres[k-1] = auxChar;
            k--;
        }
    }

    for (i = (charEncontrados-1); i>=0; i--) {
        listaFrequencia = inserirLista(listaFrequencia, caracteres[i], frequenciaChar[i]);
    }
    return listaFrequencia;
}

Arvore arvoreHuffman(Lista listaFrequencia){
    int nLista = obterTamanhoLista(listaFrequencia);
    int i;
    int inicioListaHuffman = 0;
    Arvore huffman[nLista];
    Lista aux;
    Arvore auxE, auxD;

    aux = listaFrequencia;
    for (i = 0; i < nLista; i++) {
        huffman[i] = construirArvore(aux->caracter, aux->qntd, NULL, NULL);
        aux = aux->next;
    }

    while (inicioListaHuffman < (nLista-1)){
        int qntd = huffman[inicioListaHuffman]->num + huffman[inicioListaHuffman+1]->num;
        huffman[inicioListaHuffman+1] = construirArvore('\0', qntd, huffman[inicioListaHuffman], huffman[inicioListaHuffman+1]);
        inicioListaHuffman++;

        int k = inicioListaHuffman;
        Arvore auxHuff;
        while (k + 1 < nLista && huffman[k]->num > huffman[k+1]->num){
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




