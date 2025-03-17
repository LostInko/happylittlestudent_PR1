//
// Created by matte on 28/02/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "caricamento.h"
#include "preparazionegioco.h"
#include "utility.h"
#include "ANSI-color-codes.h"
#include "salvataggi.h"

/**
 * carica i dati di salvataggio da un file binario e li inserisce nelle liste corrispondenti.
 *
 * @param listaGiocatore puntatore al puntatore alla testa della lista dei giocatori
 * @param listaOstacolo puntatore al puntatore alla testa della lista degli ostacoli
 * @param listaCarta puntatore al puntatore alla testa della lista delle carte
 * @param listaCartaScarto puntatore al puntatore alla testa della lista delle carte scartate
 * @param nomeFileSav nome del file di salvataggio da cui caricare i dati
 * @return il numero di giocatori
 */
int caricaSalvataggio(Giocatore **listaGiocatore, Ostacolo **listaOstacolo, Carta **listaCarta, Carta **listaCartaScarto, char nomeFileSav[DIM_STR], int *turno){
    FILE *fp = NULL;
    int numGiocatori, numOstacoli, numCarte;
    Giocatore auxGiocatore;
    Giocatore *testaGiocatore = NULL;
    Carta auxCarta;
    Ostacolo auxOstacolo;

    //apro il file binario in modalità lettura
    fp = fopen(nomeFileSav, "rb");
    if(fp == NULL)  exit(-6);

    //carico il numero di giocatori
    fread( &numGiocatori, sizeof(int), 1, fp );
//    printf("numg %d\n", numGiocatori);
    //eseguo il for per ogni giocatore
    for (int i = 0; i < numGiocatori; ++i) {

        //carico i dati del giocatore in un ausiliare
        fread( &auxGiocatore, sizeof(Giocatore), 1, fp );
        //richiamo la funzione per aggiungere il giocatore passando l'ausiliare
        aggiungiSaveGiocatoreCoda(listaGiocatore, auxGiocatore);

        //salvo la testa della lista
        if(i == 0){
            testaGiocatore = *listaGiocatore;
        }

        //sposto il puntatore dalla lista dei giocatori all'ultimo giocatore aggiunto nel for
        for (int j = 0; j < i; ++j) {
            (*listaGiocatore) = (*listaGiocatore)->nextGiocatore;
        }

        //carico le carte nella mano del giocatore
        for (int j = 0; j < START_CARTA; ++j) {

            fread( &auxCarta, sizeof(Carta), 1, fp );
            aggiungiSaveCartaCoda(&(*listaGiocatore)->manoCarta, auxCarta);
        }

        //leggo il numero di ostacoli
        fread( &numOstacoli, sizeof(int), 1, fp );

        //carico gli ostacoli nella mano del giocatore
        for (int j = 0; j < numOstacoli; ++j) {

            fread( &auxOstacolo, sizeof(Ostacolo), 1, fp );
            aggiungiSaveOstacoloCoda(&(*listaGiocatore)->manoOstacolo, auxOstacolo);
        }

        //ripristino il puntatore alla testa della lista
        *listaGiocatore = testaGiocatore;
//        printf("Caricamento Giocatore %d\n", i);
    }

    //leggo il numero di carte nel mazzo
    fread( &numCarte, sizeof(int), 1, fp );
    for (int i = 0; i < numCarte; ++i) {

        //carico i dati delle carte in un ausiliare
        fread( &auxCarta, sizeof(Carta), 1, fp );
        //richiamo la funzione per aggiungere la carta passando l'ausiliare
        aggiungiSaveCartaCoda(listaCarta,auxCarta);
//        printf("Caricamento Carta %d\n", i);
    }

    //leggo il numero di carte nel mazzo scarti
    fread( &numCarte, sizeof(int), 1, fp );
    for (int i = 0; i < numCarte; ++i) {

        //carico i dati delle carte in un ausiliare
        fread( &auxCarta, sizeof(Carta), 1, fp );
        //richiamo la funzione per aggiungere la carta passando l'ausiliare
        aggiungiSaveCartaCoda(listaCartaScarto,auxCarta);
//        printf("Caricamento Scarto %d\n", i);
    }

    //leggo il numero di ostacoli nel mazzo
    fread( &numOstacoli, sizeof(int), 1, fp );
    for (int i = 0; i < numOstacoli; ++i) {

        //carico i dati dell'ostacolo in un ausiliare
        fread( &auxOstacolo, sizeof(Ostacolo), 1, fp );
        //richiamo la funzione per aggiungere la carta passando l'ausiliare
        aggiungiSaveOstacoloCoda(listaOstacolo,auxOstacolo);
//        printf("Caricamento Ostacolo %d\n", i);
    }

    //inizializzo il turno a 1 in caso non sia presente nel file
    *turno = 1;
    fread(turno, sizeof(int), 1, fp);

    fclose(fp);

    return numGiocatori;
}

/**
 * aggiunge un nuovo giocatore in coda alla lista dei giocatori.
 *
 * @param listaGiocatore puntatore al puntatore alla testa della lista dei giocatori
 * @param auxGiocatore giocatore da aggiungere alla lista
 */
void aggiungiSaveGiocatoreCoda(Giocatore **listaGiocatore, Giocatore auxGiocatore) {

    Giocatore *auxTemporaneo = NULL;

    //alloco la memoria per il nuovo giocatore
    Giocatore *nuovaTesta = (Giocatore *) malloc(sizeof(Giocatore));
    if(nuovaTesta == NULL) {
        exit(17);
    }

    //copio i dati del giocatore nella nuova testa
    strcpy(nuovaTesta->nomeUtente, auxGiocatore.nomeUtente);
    nuovaTesta->manoCarta = NULL;
    nuovaTesta->manoOstacolo = NULL;
    nuovaTesta->cfuAccumulati = auxGiocatore.cfuAccumulati;
    nuovaTesta->personaggio = auxGiocatore.personaggio;

    //imposto il puntatore al giocatore successivo
    nuovaTesta->nextGiocatore = NULL;

    //se la lista è vuota, il nuovo giocatore diventa la testa della lista
    if (*listaGiocatore == NULL) {
        *listaGiocatore = nuovaTesta;
    } else {
        //altrimenti, scorri la lista fino all'ultimo elemento e aggiungi il nuovo giocatore
        auxTemporaneo = *listaGiocatore;
        while (auxTemporaneo->nextGiocatore != NULL) {
            auxTemporaneo = auxTemporaneo->nextGiocatore;
        }
        auxTemporaneo->nextGiocatore = nuovaTesta;
    }
}

/**
 * aggiunge un nuovo ostacolo in coda alla lista degli ostacoli.
 *
 * @param listaOstacolo puntatore al puntatore alla testa della lista degli ostacoli
 * @param auxOstacolo ostacolo da aggiungere alla lista
 */
void aggiungiSaveOstacoloCoda(Ostacolo **listaOstacolo, Ostacolo auxOstacolo) {

    Ostacolo *auxTemporaneo = NULL;

    //alloco la memoria per la nuova carta
    Ostacolo *nuovaTesta = (Ostacolo *) malloc(sizeof(Ostacolo));
    if(nuovaTesta == NULL) {
        exit(18);
    }

    //copio i dati della carta nella nuova testa
    strcpy(nuovaTesta->nomeOstacolo, auxOstacolo.nomeOstacolo);
    strcpy(nuovaTesta->descOstacolo, auxOstacolo.descOstacolo);
    nuovaTesta->tipoOstacolo = auxOstacolo.tipoOstacolo;

    //imposto il puntatore alla carta successiva
    nuovaTesta->nextOstacolo = NULL;

    //se la lista è vuota, il nuovo giocatore diventa la testa della lista
    if (*listaOstacolo == NULL) {
        *listaOstacolo = nuovaTesta;
    } else {
        //altrimenti, scorri la lista fino all'ultimo elemento e aggiungi il nuovo giocatore
        auxTemporaneo = *listaOstacolo;
        while (auxTemporaneo->nextOstacolo != NULL) {
            auxTemporaneo = auxTemporaneo->nextOstacolo;
        }
        auxTemporaneo->nextOstacolo = nuovaTesta;
    }

}

/**
 * aggiungo una nuova carta in coda alla lista delle carte.
 *
 * @param listaCarta puntatore al puntatore alla testa della lista delle carte
 * @param auxCarta carta da aggiungere alla lista
 */
void aggiungiSaveCartaCoda(Carta **listaCarta, Carta auxCarta) {

    Carta *auxTemporaneo = NULL;

    //alloco la memoria per la nuova carta
    Carta *nuovaTesta = (Carta *) malloc(sizeof(Carta));
    if(nuovaTesta == NULL) {
        exit(19);
    }

    //copio i dati della carta nella nuova testa
    strcpy(nuovaTesta->nomeCarta, auxCarta.nomeCarta);
    nuovaTesta->valore = auxCarta.valore;
    nuovaTesta->effettoCarta = auxCarta.effettoCarta;

    //imposto il puntatore alla carta successiva
    nuovaTesta->nextCarta = NULL;

    //se la lista è vuota, il nuovo giocatore diventa la testa della lista
    if (*listaCarta == NULL) {
        *listaCarta = nuovaTesta;
    } else {
        //altrimenti, scorri la lista fino all'ultimo elemento e aggiungi il nuovo giocatore
        auxTemporaneo = *listaCarta;
        while (auxTemporaneo->nextCarta != NULL) {
            auxTemporaneo = auxTemporaneo->nextCarta;
        }
        auxTemporaneo->nextCarta = nuovaTesta;
    }


}

/**
 * salva i dati di gioco in un file binario.
 *
 * @param listaGiocatore puntatore alla testa della lista dei giocatori
 * @param listaOstacolo puntatore alla testa della lista degli ostacoli
 * @param listaCarta puntatore alla testa della lista delle carte
 * @param listaCartaScarto puntatore alla testa della lista delle carte scartate
 * @param nomeFileSav nome del file di salvataggio
 *
 * @exit "-4" se ci sono problemi nell'apertura del file
 */
void creaSalvataggio(Giocatore *listaGiocatore, Ostacolo *listaOstacolo, Carta *listaCarta, Carta *listaCartaScarto, char nomeFileSav[DIM_STR], int turno) {
    FILE *fp = NULL;
    int numGiocatori, numOstacoli, numCarte;

    Giocatore *auxGiocatore = NULL;
    Carta *auxManoCarta = NULL;
    Ostacolo* auxOstacolo = NULL;

    Carta *auxCarta = NULL;
    Ostacolo *auxManoOstacolo = NULL;

    //conto il numero di giocatori
    auxGiocatore = listaGiocatore;
    numGiocatori = contatoreListaGiocatore(auxGiocatore);

    //apro il file per la scrittura in modalità binaria
    fp = fopen(nomeFileSav, "wb");
    if (fp == NULL) {
        exit(-4);
    }

    //salvo il numero di giocatori nel file
    fwrite(&numGiocatori, sizeof(int), 1, fp);

    //salvo i dati di ciascun giocatore
    auxGiocatore = listaGiocatore;
    for (int i = 0; i < numGiocatori; ++i) {
        //salvo il blocco del giocatore
        fwrite(auxGiocatore, sizeof(Giocatore), 1, fp);

        //salvo la testa della mano
        auxManoCarta = auxGiocatore->manoCarta;
        //salvo le carte in mano al giocatore
        for (int j = 0; j < START_CARTA; ++j) {
            fwrite(auxGiocatore->manoCarta, sizeof(Carta), 1, fp);
            auxGiocatore->manoCarta = auxGiocatore->manoCarta->nextCarta;
        }
        //reimposto la testa
        auxGiocatore->manoCarta = auxManoCarta;

        //salvo il numero di carte ostacolo accumulate dal giocatore
        numOstacoli = contatoreListaOstacolo(auxGiocatore->manoOstacolo);
        fwrite(&numOstacoli, sizeof(int), 1, fp);

        //salvo la testa della mano
        auxManoOstacolo = auxGiocatore->manoOstacolo;
        //salvo le carte ostacolo del giocatore
        for (int j = 0; j < numOstacoli; ++j) {
            fwrite(auxGiocatore->manoOstacolo, sizeof(Ostacolo), 1, fp);
            auxGiocatore->manoOstacolo = auxGiocatore->manoOstacolo->nextOstacolo;
        }
        //reimposto la testa
        auxGiocatore->manoOstacolo = auxManoOstacolo;

        auxGiocatore = auxGiocatore->nextGiocatore;
    }

    //salvo il numero di carte nel mazzo di carte
    auxCarta = listaCarta;
    numCarte = contatoreListaCarta(auxCarta);
    fwrite(&numCarte, sizeof(int), 1, fp);

    //salvo le carte nel mazzo di carte
    auxCarta = listaCarta;
    for (int i = 0; i < numCarte; ++i) {
        fwrite(auxCarta, sizeof(Carta), 1, fp);
        auxCarta = auxCarta->nextCarta;
    }

    //salvo il numero di carte nel mazzo di carte scartate
    auxCarta = listaCartaScarto;
    numCarte = contatoreListaCarta(auxCarta);
    fwrite(&numCarte, sizeof(int), 1, fp);

    //salvo le carte nel mazzo di carte scartate
    auxCarta = listaCartaScarto;
    for (int i = 0; i < numCarte; ++i) {
        fwrite(auxCarta, sizeof(Carta), 1, fp);
        auxCarta = auxCarta->nextCarta;
    }

    //salvo il numero di carte nel mazzo di ostacoli
    auxOstacolo = listaOstacolo;
    numOstacoli = contatoreListaOstacolo(auxOstacolo);
    fwrite(&numOstacoli, sizeof(int), 1, fp);

    //salvo le carte nel mazzo di ostacoli
    auxOstacolo = listaOstacolo;
    for (int i = 0; i < numOstacoli; ++i) {
        fwrite(auxOstacolo, sizeof(Ostacolo), 1, fp);
        auxOstacolo = auxOstacolo->nextOstacolo;
    }

    //salvo il numero del turno
    fwrite(&turno, sizeof(int), 1, fp);

    //chiudo il file
    fclose(fp);
}


