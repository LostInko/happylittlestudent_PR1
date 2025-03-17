#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "caricamento.h"
#include "preparazionegioco.h"
#include "mescolamazzi.h"
#include "utility.h"


/**
 * Mescolo il mazzo di carte secondo l'algoritmo di fisher
 *
 * @param listaCarta puntatore alla testa della lista di carte da mescolare
 * @return  la testa della lista dopo aver mescolato le carte
 *
 * @exit "7" se non è possibile allocare la memoria per arrayLista
 */
/*"The Fisher–Yates shuffle is an algorithm for shuffling a finite sequence.
 * The algorithm takes a list of all the elements of the sequence,
 * and continually determines the next element in the shuffled sequence by randomly
 * drawing an element from the list until no elements remain."
 * */
Carta* mescolaMazzoCarta(Carta *listaCarta){

    int randNum;
    Carta scambioAux;
    Carta *aux = listaCarta;
    Carta *arrayLista= NULL;
    int numCarta;

    //conto il numero di carte nella lista
    numCarta = contatoreListaCarta(listaCarta);

    //alloco la memoria per l'array temporaneo delle carte
    arrayLista = (Carta *) calloc(numCarta, sizeof(Carta));
    if (arrayLista == NULL)  exit(7);

    //copio le carte dalla lista originale all'array temporaneo
    aux = listaCarta;
    for (int i= 0; i < numCarta; ++i) {
        arrayLista[i] = *aux;
        aux = aux->nextCarta;
    }

    //mescolo l'array temporaneo delle carte utilizzando l'algoritmo di fisher
    for (int i= numCarta-1; i > 0; i--) {
        randNum = randRangeIntero(0, i+1);
        scambioAux= arrayLista[randNum];
        arrayLista[randNum] = arrayLista[i];
        arrayLista[i] = scambioAux;
    }

    //copio le carte mescolate dall'array temporaneo alla listaCarta
    aux = listaCarta;
    for (int i= 0; i < numCarta; ++i){
        strcpy(aux->nomeCarta, arrayLista[i].nomeCarta);
        aux->valore = arrayLista[i].valore;
        aux->effettoCarta = arrayLista[i].effettoCarta;
        aux = aux->nextCarta;
    }

    //libero la memoria occupata dall"array temporaneo
    free( arrayLista);
    
    //restituisco la testa della lista
    return listaCarta;

}

/**
 * Mescolo il mazzo di ostacoli secondo l'algoritmo di fisher
 *
 * @param listaOstacolo puntatore alla testa della lista di ostacoli da mescolare
 * @return  la testa della lista dopo aver mescolato gli ostacoli
 *
 * @exit "7" se non è possibile allocare la memoria per arrayLista
 */
Ostacolo* mescolaMazzoOstacolo(Ostacolo *listaOstacolo){

    int randNum;
    Ostacolo scambioAux;
    Ostacolo *aux = listaOstacolo;
    Ostacolo *arrayLista = NULL;
    int numOstacolo;

    //conto il numero di carte nella lista
    numOstacolo = contatoreListaOstacolo(listaOstacolo);

    //alloco la memoria per l'array temporaneo degli ostacoli
    arrayLista = (Ostacolo *) calloc(numOstacolo, sizeof(Ostacolo));
    if (arrayLista == NULL)  exit(7); // Gestisce il fallimento dell'allocazione di memoria

    //copio gli ostacoli dalla lista originale all'array temporaneo
    aux = listaOstacolo;
    for (int i = 0; i < numOstacolo; ++i) {
        arrayLista[i] = *aux;
        aux = aux->nextOstacolo;
    }

    //mescolo l'array temporaneo degli ostacoli utilizzando l'algoritmo di fisher
    for (int i = numOstacolo - 1; i > 0; i--) {
        randNum = randRangeIntero(0, i + 1);  // Genera un numero casuale tra 0 e i inclusi
        // Scambia gli elementi nella nuova lista
        scambioAux = arrayLista[randNum];
        arrayLista[randNum] = arrayLista[i];
        arrayLista[i] = scambioAux;
    }

    //copio le carte mescolate dall'array temporaneo alla listaOstacolo
    aux = listaOstacolo;
    for (int i = 0; i < numOstacolo; ++i) {
        strcpy(aux->nomeOstacolo, arrayLista[i].nomeOstacolo);
        strcpy(aux->descOstacolo, arrayLista[i].descOstacolo);
        aux->tipoOstacolo = arrayLista[i].tipoOstacolo;
        aux = aux->nextOstacolo;
    }

    //libero la memoria occupata dall"array temporaneo
     free(arrayLista);

    //restituisco la testa della lista
    return listaOstacolo;
}

