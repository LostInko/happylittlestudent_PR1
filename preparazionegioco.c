//
// Created by matte on 13/02/2024.
//
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "caricamento.h"
#include "preparazionegioco.h"
#include "utility.h"
#include "ANSI-color-codes.h"

/**
 * Aggiungo un nuovo giocatore in testa alla listaGiocatore
 *
 * @param listaGiocatore puntatore alla testa della lista giocatori
 * @param ruolo il ruolo stesso del giocatore
 * @param personaggio il personaggio scelto per il giocatore
 * @param i indica il numero del giocatore che stiamo aggiungendo
 * @return il puntatore alla nuova testa della listaGiocatore
 *
 * @exit "5" se c'è un errore nell'allocazione di memoria
 */
Giocatore *aggiungiGiocatoreInTesta(Giocatore *listaGiocatore, int ruolo, Personaggio *personaggio, int i) {

    //alloco la memoria per il nuovo giocatore
    Giocatore *nuovaTesta = (Giocatore *) malloc(sizeof(Giocatore));
    if (nuovaTesta == NULL)  exit(5);

    //consumo il carattere residuo nell'input
    getchar();
    printf(" Giocatore %d:", i+1);
    scanf("%[^\n]s", nuovaTesta->nomeUtente);

    //inserisco il personaggio all'indice parametro ruolo
    nuovaTesta->personaggio = personaggio[ruolo];

    //imposto il puntatore al successivo
    nuovaTesta->nextGiocatore = listaGiocatore;

    //restituisco la nuova testa
    return nuovaTesta;
}

/**
 * Aggiungo un nuovo giocatore alla fine della listaGiocatore
 *
 * @param listaGiocatore puntatore alla testa della lista giocatori
 * @param ruolo il ruolo stesso del giocatore
 * @param personaggio il personaggio scelto per il giocatore
 * @param i indica il numero del giocatore che stiamo aggiungendo
 * @return il puntatore alla nuova testa della listaGiocatore
 *
 * @exit "5" se c'è un errore nell'allocazione di memoria
 */
Giocatore *aggiungiGiocatoreInCoda(Giocatore *listaGiocatore, int ruolo, Personaggio *personaggio, int i) {

    //alloco la memoria per il nuovo giocatore
    Giocatore *nuovoGiocatore = (Giocatore *) malloc(sizeof(Giocatore));
    if (nuovoGiocatore == NULL)  exit(5);

    Giocatore *auxTemporaneo = NULL;

    //consumo il carattere residuo nell'input
    getchar();
    printf(" Giocatore %d:", i+1);
    scanf("%[^\n]s", nuovoGiocatore->nomeUtente);

    //inserisco il personaggio all'indice parametro ruolo
    nuovoGiocatore->personaggio = personaggio[ruolo];

    //imposto il puntatore al successivo a NULL perche il nuovo giocatore sarà l'ultimo della list
    nuovoGiocatore->nextGiocatore = NULL;

    //se la lista è vuota, il nuovo giocatore diventa la testa della lista
    if (listaGiocatore == NULL) {
        listaGiocatore = nuovoGiocatore;
    } else {
        //altrimenti, scorri la lista fino all'ultimo elemento e aggiungi il nuovo giocatore
        auxTemporaneo = listaGiocatore;
        while (auxTemporaneo->nextGiocatore != NULL) {
            auxTemporaneo = auxTemporaneo->nextGiocatore;
        }
        auxTemporaneo->nextGiocatore = nuovoGiocatore;
    }
    //restituisco la testa della lista aggiornata
    return listaGiocatore;
}

/**
 * Gestisco l'input in base al numero di giocatori e aggiunge i giocatori inseriti
 * alla lista giocatori
 *
 * @param numPersonaggi il numero di personaggi disponibili
 * @param personaggio il vettore struttura Personaggi
 * @param numGiocatori il puntatore al numero di giocatori inserito dall'utente in modo da renderlo accessibile dal main
 * @return il puntatore alla testa della lista giocatori
 */
Giocatore *inputGiocatori( int numPersonaggi, Personaggio *personaggio, int *numGiocatori) {

    //alloco dinamicamente un array di interi della grandezza del numero dei personaggi
    int *arrayRand = NULL;
    arrayRand = allocaArrayInteri(numPersonaggi);

    Giocatore *listaGiocatore = NULL;

    printf(RED "---------------- Giocatori ----------------\n" RESET);
    do {
        printf("Quanti sono i giocatori?  [MAX 4] [MIN 2]:");
        scanf("%d", numGiocatori);

        //stampa di debug del numero di giocatori
//        printf("%d", *numGiocatori);

        //verifico se il numero di giocatori è valido
        if(*numGiocatori > numPersonaggi || *numGiocatori < MIN_GIOCATORI)
            printf("Errore! Inserisci entro i limiti scritti!\n");

    //ripeto l'operazione finché il numero di giocatori non è valido
    } while (*numGiocatori > numPersonaggi || *numGiocatori < MIN_GIOCATORI);

    //genero un array con numeri casuali non ripetuti che indicheranno il ruolo dei giocatori
    arrayRandNoRipetizione(numPersonaggi, arrayRand);

    //aggiungo i giocatori alla listaGiocatore
    for (int i = 0; i < *numGiocatori; ++i) {
        listaGiocatore = aggiungiGiocatoreInCoda(listaGiocatore, arrayRand[i], personaggio, i);
    }

    //libero la memoria occupata dall'array rand
    free(arrayRand);

    //restituisco il puntatore alla testa dei giocatori
    return listaGiocatore;
}

/**
 * Genero un dei numeri casuali senza ripetizioni e li inserisco in un array
 * passato come parametro
 *
 * @param numPersonaggi dimensione dell'array
 * @param arrayRand array in cui verranno memorizzati i numeri casuali senza ripetizioni
 */
void arrayRandNoRipetizione(int numPersonaggi, int arrayRand[numPersonaggi]){

    bool checkRipetizioni;
    int contRipetizioni;
    int min = 0;    //valore minimo da cui generale i numeri casuali
    
    //scorro l'array con il for mentre genero i numeri casuali
    for (int i = 0; i < DIM; i++) {
        do{
            //genero il numero casuale richiamando la funzione randRangeIntero
            arrayRand[i] = randRangeIntero(min, numPersonaggi-1);
            
            //inizializzo le ripetizioni a 0
            contRipetizioni = 0;
            
            //verifico se ci sono ripetizioni con i numeri precedenti
            for (int j = 0; j < i; ++j) {
                if(arrayRand[i] == arrayRand[j]) {
                    contRipetizioni++;
                }
            }
            
            //se trova una ripetizione checkRipetizioni diventa false
            checkRipetizioni = (contRipetizioni == 0);

            //continua a eseguire finché checkRipetizioni è false, dunque smette quando non trova ripetizioni
        } while(!checkRipetizioni);
    }

    //stampa di debug dell'array completo
//    printf("Vettore completo: \n");
//    for (int i = 0; i < DIM ; ++i) {
//        printf("%d\t", arrayRand[i]);
//    }
}

/**
 * genero un numero intero casuale in un intervallo min max
 * 
 * @param min valore minimo dell'intervallo 
 * @param max valore massimo dell'intervallo
 * @return un numero generato casualmente
 */
int randRangeIntero(int min, int max) {
    int random, aux;
    random = aux = 0;

    //scambio max e min se sono stati inseriti invertiti
    if (min > max) {
        aux = min;
        min = max;
        max = aux;
    }

    //genero il numero casuale
    random = min + rand() % (max - min + 1);

    //restituisco il numero casuale
    return random;
}

/**
 * Elimino tutti gli elementi della lista dei giocatori e libero la memoria allocato da ogni nodo
 *
 * @param listaGiocatore puntatore alla testa della listaGiocatore da eliminare
 * @return NULL perchè la lista sarà eliminata interamente
 */
Giocatore *eliminaListaGiocatore(Giocatore *listaGiocatore){

    //creo un puntatore al successivo della lista
    Giocatore *next;

    //scorro il while finché non termina
    while (listaGiocatore != NULL){

        //memorizzo il puntatore al successivo nodo della lista
        next = listaGiocatore->nextGiocatore;

        //libero la memoria del nodo corrente
        free(listaGiocatore);

        //passo al nodo successivo
        listaGiocatore = next;
    }

    return NULL;
}



