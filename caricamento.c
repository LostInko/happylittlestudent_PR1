#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "caricamento.h"


/**
 * conta il numero di righe presenti in un file
 *
 * @param nomeFile nome del file da controllare
 * @return il numero di righe del file
 *
 * @exit "-1" se c'è un errore nella lettura del file
 * @exit "1" se il file è vuoto
 */
int contaRigheFile(char nomeFile[]){

    int cont = 0;
    //creo un buffer per assicurarmi di leggere ogni riga di un file
    char buffer[BUFFER];

    //apro il file in sola lettura
    FILE *fp = NULL;
    fp = fopen(nomeFile, "r" );
    if(fp == NULL) { exit(-1);}

    //leggo ogni riga finché non arriva alla fine del file
    while (fscanf(fp, " %[^\n]c", buffer) == 1){
        cont++;
    }
    if(cont <= 1) exit(1);

    //restituisco il numero di righe del file
    return cont;
}

///personaggio
/**
 * La funzione allocaPersonaggio alloca dinamicamente un array
 * di strutture Personaggio
 *
 * @param num il numero di elementi da allocare nell'array
 * @return un puntatore a un array di strutture personaggio
 *
 * @exit "2" se l'allocazione di memoria fallisce
**/
Personaggio *allocaPersonaggio(int num) {
    Personaggio *personaggio = NULL;

    //alloco la memoria per num strutture Personaggio
    personaggio = (Personaggio *) calloc(num, sizeof(Personaggio));

    //controllo se posso allocare la memoria dinamicamente ed esco con 2 se non è possibile
    if (personaggio == NULL) exit(2);

    //restituisco il puntatore all'array
    return personaggio;
}

/**
 * La funzione caricaPersonaggio legge un file .txt e carica i dati dei personaggi
 * restituendoli come un array di strutture
 *
 * @param dim dimensione dell'array dei bonus e malus nella struttura Personaggio
 * @param num puntatore a una variabile intera che restituisce il numero dei personaggi caricati
 * @return un puntatore a un array di strutture Personaggio con i dati caricati dal file
 *
 * @exit "-1" se il file "personaggi.txt" non puo' essere aperto
 * @exit "2" se lo spazio per l'array struttura Personaggio non puo essere allocato
**/
Personaggio* caricaPersonaggio(int dim, int *num){
    char nomeFile[] = "personaggi.txt";
    Personaggio *personaggio = NULL;

    //conto il numero di righe nel file
    *num = contaRigheFile(nomeFile);

    //alloco la memoria per array struttura Personaggio e
    personaggio = allocaPersonaggio(*num);
    if(personaggio == NULL) exit(2);

    //apro il file in modalità lettura ed esco con "-1" se il file non è leggibile
    FILE *fp = NULL;
    fp = fopen(nomeFile, "r" );
    if(fp == NULL) { exit(-1);}

    //leggo i dati del file e li memorizzo nell'array struttura
    for (int i = 0; i < *num; ++i) {
        for (int j = 0; j < dim; ++j) {
            fscanf(fp, "%d ", &personaggio[i].arrayBonMal[j]);
        }
        fscanf(fp, "%[^\n]s", personaggio[i].nomePersonaggio);
    }
    fclose(fp);

    //restituisco il puntatore all'array di strutture
    return personaggio;
}

///ostacolo
/**
 * Aggiungo un nuovo ostacolo all'inizio di una lista di ostacoli
 *
 * @param listaOstacolo puntatore alla testa della lista ostacolo
 * @param index indica quale tipo di ostacolo si sta caricando
 * @param fp puntatore al file da cui stiamo leggendo i dati degli ostacoli
 * @return il puntatore alla nuova testa della lista ostacolo
 *
 * @exit "3" in caso di fallimento dell'allocazione di memoria per l'ostacolo
 * **/
Ostacolo *aggiungiOstacoloInTesta(Ostacolo *listaOstacolo, int index, FILE *fp) {

    //alloco la memoria del nuovo ostacolo
    Ostacolo *nuovaTesta = (Ostacolo *) malloc(sizeof(Ostacolo));
    if(nuovaTesta == NULL) {
        exit(3);
    }

    //leggo i dati da file e li carico nel nuovo nodo
    fscanf(fp, "\n");
    fscanf(fp, "%[^\n]s", nuovaTesta->nomeOstacolo);
    fscanf(fp, "\n");
    fscanf(fp, "%[^\n]s", nuovaTesta->descOstacolo);

    //imposto l'indice del tipo di ostacolo e punto all'ostacolo successivo
    nuovaTesta->tipoOstacolo = index;
    nuovaTesta->nextOstacolo = listaOstacolo;

    //restituisco il puntatore alla nuova testa
    return nuovaTesta;
}

/**
 * carico la lista di ostacoli da un file .txt e restituisco il puntatore alla testa
 * una volta completato il caricamento
 *
 * @return puntatore alla testa della lista
 *
 * @exit "-2" se avviene un errore di apertura del file
**/
Ostacolo *caricaOstacolo() {
    Ostacolo *listaOstacolo = NULL;
    char nomeFile[] = "ostacoli.txt";
    int dim;

    //apro il file in modalità lettura
    FILE *fp = fopen(nomeFile, "r");
    if(fp == NULL) {
        exit(-2);
    }

    //carico la lista richiamando la funzione aggiungiOstacoloInTesta
    for (int i = 0; i < ESAME; ++i) {

        fscanf(fp, "%d", &dim); //leggo il numero di ostacoli per ogni tipo
        for (int j = 0; j < dim; ++j) {      //scorro il for della dimensione appena trovata
            listaOstacolo = aggiungiOstacoloInTesta(listaOstacolo, i, fp);
        }
    }
    fclose(fp);

    //restituisco il puntatore alla testa della listaOstacolo
    return listaOstacolo;
}

///carta
/**
 * Aggiungo una nuova carta in testa alla lista di carte
 *
 * @param listaCarta puntatore alla testa della lista di carte
 * @param cartaCopia copia della carta da aggiungere alla lista
 * @return puntatore alla nuova testa della lista di carte
 *
 * @exit "4" se non è possibile allocare la memoria per la nuova carta
**/
Carta *aggiungiCartaInTesta(Carta *listaCarta, Carta cartaCopia) {

    //alloco la memoria per la nuova carta
    Carta *nuovaTesta = (Carta *) malloc(sizeof(Carta));
    if(nuovaTesta == NULL) {
        exit(4);
    }

    //copio i dati della carta nella nuova testa
    nuovaTesta->valore = cartaCopia.valore;
    nuovaTesta->effettoCarta = cartaCopia.effettoCarta;
    strcpy(nuovaTesta->nomeCarta, cartaCopia.nomeCarta);

    //imposto il puntatore alla carta successiva
    nuovaTesta->nextCarta = listaCarta;

    //restituisco il puntatore alla testa della listaCarta
    return nuovaTesta;
}

/**
 * Carico la lista delle carte da un file .txt e restituisco il puntatore alla
 * testa della lista
 *
 * @return puntatore alla testa della listaCarta
 *
 * @exit "-3" in caso di errore nell'apertura del file
**/
Carta *caricaCarta() {
    Carta *listaCarta = NULL;
    //creo una variabile temporanea per memorizzare una copia di una carta letta dal file
    Carta cartaCopia;
    char nomeFile[] = "carte.txt";

    int righe;  //variabile numero di righe del file
    int dim;    //variabile per il numero di carte identiche per ogni carta

    //conto il numero di righe del file
    righe = contaRigheFile(nomeFile);

    //apro il file in modalità lettura
    FILE *fp = fopen(nomeFile, "r");
    if(fp == NULL) {
        exit(-3);
    }

    //leggo i dati di ogni carta finché non terminano le righe
    for (int i = 0; i < righe; ++i) {
        fscanf(fp, "%d", &dim);
        fscanf(fp, "%d", &cartaCopia.effettoCarta);
        fscanf(fp, "%d", &cartaCopia.valore);
        fscanf(fp, "%[^\n]s", cartaCopia.nomeCarta);

        //aggiungo la carta alla lista in numero di volte scritto
        for (int j = 0; j < dim; ++j) {
            listaCarta = aggiungiCartaInTesta(listaCarta, cartaCopia);
        }
    }

    fclose(fp);

    //restituisco il puntatore alla testa della listaCarta
    return listaCarta;
}

///free
/**
 * Elimino tutti gli elementi della lista degli ostacoli e libero la memoria allocato da ogni nodo
 *
 * @param listaOstacolo puntatore alla testa della listaOstacoli da eliminare
 * @return NULL perchè la lista sarà eliminata interamente
 */
Ostacolo *eliminaListaOstacolo(Ostacolo *listaOstacolo){

    //creo un puntatore al successivo della lista
    Ostacolo *next;

    //scorro il while finché non termina
    while (listaOstacolo != NULL){

        //memorizzo il puntatore al successivo nodo della lista
        next = listaOstacolo->nextOstacolo;

        //libero la memoria del nodo corrente
        free(listaOstacolo);

        //passo al nodo successivo
        listaOstacolo = next;
    }

    return NULL;
}

/**
 * Elimino tutti gli elementi della lista delle carte e libero la memoria allocato da ogni nodo
 *
 * @param listaCarta puntatore alla testa della listaCarta da eliminare
 * @return NULL perchè la lista sarà eliminata interamente
 */
Carta *eliminaListaCarta(Carta *listaCarta){

    //creo un puntatore al successivo della lista
    Carta *next;

    //scorro il while finché non termina
    while (listaCarta != NULL){

        //memorizzo il puntatore al successivo nodo della lista
        next = listaCarta->nextCarta;

        //libero la memoria del nodo corrente
        free(listaCarta);

        //passo al nodo successivo
        listaCarta = next;
    }

    return NULL;
}



