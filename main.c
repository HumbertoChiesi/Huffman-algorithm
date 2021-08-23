#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Lista.h"
#include "Arvore.h"

#define ASCII 256

void binario(char *);
void binarioHuff(char *, int []);
char *lerArquivo(char *);
int listaFrequencia(char *, struct Celula **);
Arvore arvoreHuffman(struct Celula *, int);
int listaHuff(Arvore, int, int, int []);
void mergeSort(struct Celula **);
struct Celula *sortedMerge(struct Celula *, struct Celula*);
void splitList(struct Celula *, struct Celula **, struct Celula **);

int main() {
    struct Celula * a = NULL, * listaCodigos = NULL;
    int tamanho;
    char * texto;
    int codigos[ASCII];

    texto = lerArquivo("..\\texto\\texto.txt");
    tamanho = listaFrequencia(texto, &a);
    Arvore huff = arvoreHuffman(a, tamanho);
    for (int i = 0; i < ASCII; i++) codigos[i] = 0;
    listaHuff(huff, 0, 0, codigos);
    printf("Texto em binario:\n-------------------------\n");
    binario(texto);
    printf("\n\nTexto em binario dps do algoritmo de Huffman:\n-------------------------\n");
    binarioHuff(texto, codigos);

    return 0;
}

//função que imprime uma string em binario
//parametro: uma string
void binario(char * M){
    int n,nb,i,k,r,D;
    char *code;
    n = strlen(M);
    code = malloc((8*n+1)*sizeof(char));
    nb = 8*n;  for(k=0;k<=nb;k++) code[k] = 48;	code[nb]='\0';
    i = n-1;    k = nb-1;
    while (i>=0){
        D = M[i];
        do{
            r = D % 2; D = D/2; code[k] = r + 48;
            k = k-1;
        } while (D != 0);
        code[k] = D + 48;
        k--;
        i--;
    }
    printf("%s", code);
}

//uma funcao que mostra o texto em binarios com os códigos obtidos depois da compressao do Algoritmo de Huffman
//parametros: um texto e um array com os novos valores em binario dos caracteres
//*para acessar novo código de cada caracter basta usar seu codigo do ASCII como índice
void binarioHuff(char * texto, int codigos[]){
    size_t tamanho = strlen(texto);
    int i;

    for (i = 0; i < tamanho; i++) {
        printf("%d", codigos[texto[i]]);
    }

    printf("\n");
}


//le um arquivo e devolve uma string com o conteudo
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
//e aloca no nó de parametro uma lista ligada em ordem crescente em relacao a quantidade de vezes que o caracter aparece no texto
//devolve um inteiro que representa a quantidade de caracteres unicos no texto
//parametros = uma string para ser analizada e um nó da lista ligada
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

    //organiza a lista encadeada que guarda os caracteres e suas frequencia com Merge sort
    mergeSort(listaFrequencia);

    return charEncontrados;
}

//funcao que constroi a arvore de Huffman que sera utilizada para gerar os novos códigos dos caracteres
//parametro: primeiro nó de uma fila encadeada com os caracteres e suas frequencias e a quantidade de caracteres nessa fila
//retorna a arvore de Huffman
Arvore arvoreHuffman(struct Celula * listaFrequencia, int tamanho){
    int i, k;
    int inicioListaHuffman = 0;
    Arvore huffman[tamanho];
    struct Celula * aux;
    Arvore auxE, auxD;

    aux = listaFrequencia;
    //popula o array de Arvores
    for (i = 0; i < tamanho; i++) {
        huffman[i] = construirArvore(aux->caracter, aux->qntd, NULL, NULL);
        aux = aux->next;
    }

    //primeiramente construo e coloco na segunda posição do array uma arvore que tem
    // 2 filhos que são o primeiro e segundo elemento do array de arvores
    //sendo que a variável int k indica onde que esse array começa
    // é incrementado 1 a essa variável toda vez que somamos o primeiro e segundo elemento do array
    while (inicioListaHuffman < (tamanho-1)){
        int qntd = huffman[inicioListaHuffman]->num + huffman[inicioListaHuffman+1]->num;
        huffman[inicioListaHuffman+1] = construirArvore('\0', qntd, huffman[inicioListaHuffman], huffman[inicioListaHuffman+1]);
        inicioListaHuffman++;

        k = inicioListaHuffman;
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

//funcao recurssiva que com a arvore de Huffman acha os novos códigos dos caracteres
//parametros: arvore de Huffman, inteiro e, codigo que é o código atual em função da movimentação na arvore e um array para alocar os códigos
//*e indica a ultima movimentação feita na arvore (1 quando vai para o nó da direito e 0 para o da esquerda)
//retorna um inteiro cod que indica a posição na arvore
int listaHuff(Arvore ap, int e, int codigo, int codigos[ASCII]){
    int cod = (codigo*10) + e;
    if (!isFolha(ap)){
        cod = listaHuff(ap->esq, 0, cod, codigos);
        cod = listaHuff(ap->dir, 1, cod, codigos);
        cod = cod/10;
    } else {
        codigos[(int)ap->item] = cod;
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