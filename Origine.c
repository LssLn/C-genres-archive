/******************************************************************************
Un archivio contiene i dati relativi ai film trasmessi su un canale telvisivo. I file sono di quattro generi
diversi: "COMMEDIA" "THRILLER" "MUSICAL" e "FANTSCIENZA".
L'archivio � costituito da un file testo in cui per ogni film sono memorizzati uno per ciscuna riga le
sguenti informazioni
- titolo del film - una stringa di 255 caratteri che include caratteri bianche
- una stringa contenente il genere
- una stringa contenente la data della prima proiezione nel formato aaaa-mm-gg
Attenzione per il genere nella lista verra memorizzato un codice intero. Vedere il codice
per la corrispondenza.
Scrivere un programma che consente di effettuare le seguenti operazioni
- caricamento dei dati in una lista ordinata rispetto alla data e stampa della lista
- ricerca del film pi� nuovo e piu vecchio
- estrazioni dei film di un dato genere

*******************************************************************************/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define leng 226
#define NOME_FILE "elenco.txt"

/*struttura lista*/
typedef struct s {
    int genere; //0 per COMMEDIA, 2 per TRHILLER, 2 per MUSICAL e 3 per FANTSCIENZA
    char data[11]; //aaaa-mm-gg
    char titolo[leng];
}elemento;

typedef struct n {
    elemento info;
    struct n* next;
} nodo, * lista;

lista make_null_lista() { return NULL; }

int empty_list(lista l) { return l == NULL; }

int cons(lista* pL, elemento E) {
    //preso da LISTE -  INSERISCI E INSERISCI ORDINATO RICORSIVO (BRISCOLA)
    //aggiungi
    lista aux = (lista)malloc(sizeof(nodo));
    if (aux == NULL)    return 0;

    //inserimento in testa
    aux->info = E;
    aux->next = *pL;
    *pL = aux;
    return 1;
}

int inserimento_ordinamento(lista* pL, elemento E) {
    //LISTE -  INSERISCI ORDINATO ITERATIVO CARRELLO
    int gg, mm, aa;
    int ggx, mmx, aax=0;

    lista aux = (lista)malloc(sizeof(nodo));
    sscanf(E.data, "%d/%d/%d", &aa, &mm, &gg);
    if (*pL != NULL) {
        sscanf((*pL)->info.data, "%d/%d/%d", &aax, &mmx, &ggx);
    } // se lo faccio con *pL NULL ho violazione di accesso

    if (*pL == NULL ) {//inserimento in testa
        aux->info = E; //ci entro anche quando scorro tutta la L
        aux->next = NULL;
        *pL = aux; 
    }else if(aax>=aa){ //se entro nell'if else, e metto un E
        aux->info = E; // nel mezzo, quelli prima li recupero
        aux->next = *pL; //tornando sull'else
        *pL = aux;
    }else   inserimento_ordinamento(&(*pL)->next, E);
}



void stampa(lista L) {
    printf("\n");
    while (L != NULL) {
        printf("%s  | %s\n",L->info.data, L->info.titolo);
        L = L->next;
    }
}

/*funzioni di servizio*/
int ritorna_indice(char* genere) {
    if (strcmp(genere, "COMMEDIA") == 0) return 0;
    if (strcmp(genere, "THRILLER") == 0) return 1;
    if (strcmp(genere, "MUSICAL") == 0) return 2;
    if (strcmp(genere, "FANTASCIENZA") == 0) return 3;
    return -1;
}

char* ritorna_stringa(int indice) {
    switch (indice) {
    case 0:return "COMMEDIA";
    case 1:return "THRILLER";
    case 2:return "MUSICAL";
    case 3:return "FANTASCIENZA";
    }
    return "NoN";
}


int Menu()
{
    int scelta;
    printf("\n\n1) questito1:carica e stampa\n");
    printf("2) questito2:ricerca film pi� nuovo e piu' vecchio\n");
    printf("3) questito3:film di un dato genenre\n");
    scanf("%d", &scelta);

    return scelta;
}

/*quesito1*/
void quesito1(lista* pL) {
    FILE* f;
    elemento e;
    int cont = 0;
    char buffer[leng];

    f = fopen(NOME_FILE, "r");
    while (fscanf(f, "%[^\n]\n", buffer ) != EOF) {
        e.genere = ritorna_indice(buffer);
        fscanf(f, "%[^\n]\n", e.data);
        fscanf(f, "%[^\n]\n", e.titolo);
        cont++;
        printf("\n%d] %s | %s (%d) - %s", 
        cont,e.titolo, ritorna_stringa(e.genere),e.genere, e.data);
        inserimento_ordinamento(pL, e);
    }
}


/*quesito2*/
//-ricerca del film pi� nuovo e piu vecchio
//nel main uso 2 liste contenenti questi due elementi
//lavoro quindi per riferimento (return tipo void)
void quesito2(lista l, lista* old, lista* new) {
    lista aux = (lista)malloc(sizeof(nodo));
    //aux = make_null_lista(); //TANTO AUX=L
    int gg,mm,aa,aamax= 0;
    aux = l;
    //old sar� il primo elemento (lista ordinata)
    inserimento_ordinamento(old,l->info);
    //new invece l'ultimo

    int cont = 0;
    while (aux != NULL) {
        cont++;
        aux = aux->next;
    }

    for (int i = 0; i < cont - 1; i++)
        l = l->next;
    inserimento_ordinamento(new,l->info);
}

/*quesito3*/
//- estrazioni dei film di un dato genere
lista quesito3(lista l,char* genere) {
    lista aux = (lista)malloc(sizeof(nodo));
    aux = make_null_lista(); //se no violazione d'accesso
    int genereint = ritorna_indice(genere); //intero
    while (l != NULL) {
        if (l->info.genere == genereint) {
            inserimento_ordinamento(&aux, l->info);
        }
        l = l->next;
    }
    return aux;
}



//main 
int main(void) {
    int scelta;
    lista L = make_null_lista();
    lista old, new, result;
    old = make_null_lista();
    new = make_null_lista();
    result = make_null_lista();

    char g[20];
    do {
        scelta = Menu();
        switch (scelta) {
        case 1: quesito1(&L);
            stampa(L);
            break;
        case 2: quesito2(L,&old,&new);
            printf("\nil film piu' vecchio e' %s - %s\n", old->info.titolo,old->info.data);
            printf("\nil film piu' nuovo e' %s - %s\n", new->info.titolo,new->info.data);
            break;
        case 3: printf("inserisci il genere\n");
            scanf("%s", g);
            result = quesito3(L,g);
            stampa(result);
            break;
        case 4:
            stampa(L);
    }
} while (scelta!=0);
}




