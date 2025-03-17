//
// Created by matte on 22/02/2024.
//

///dichiarazione librerie
#include <stdio.h>
#include <stdlib.h>
#include "caricamento.h"
#include "utility.h"
#include "vinciPerdi.h"
#include "ANSI-color-codes.h"

/**
 * trova i giocatori che hanno accumulato CFU sufficienti per vincere il gioco.
 *
 * @param listaGiocatore puntatore alla testa della lista dei giocatori
 * @param arrayIndexVincenti array in cui memorizzare gli indici dei giocatori vincenti
 * @return il numero totale di giocatori vincenti trovati
 */
int cercaVincenti(Giocatore *listaGiocatore, int *arrayIndexVincenti){

    Giocatore *auxGiocatore;
    int cont = 0;
    int contVincenti = 0;

    //scorro i giocatori e conto quanti hanno raggiungo il numero di cfu per vincere
    auxGiocatore = listaGiocatore;
    while (auxGiocatore != NULL){

        if(auxGiocatore->cfuAccumulati >= VITTORIA){

            arrayIndexVincenti[cont] = cont;
            contVincenti++;
        }

        cont++;
        auxGiocatore = auxGiocatore->nextGiocatore;
    }

    return contVincenti;
}

/**
 * controlla quanti giocatori hanno raggiunto o superato la soglia di ostacoli eliminati e restituisce il conteggio.
 *
 * @param listaGiocatore puntatore alla testa della lista dei giocatori..
 * @return il numero di giocatori eliminati.
 */
int controllaEliminati(Giocatore *listaGiocatore) {
    Giocatore *auxGiocatore = NULL;
    Ostacolo *auxOstacolo = NULL;
    int *arrayContCarte = allocaArrayInteri(ESAME);
    int contGiocatore = 0;
    int contaTipoOstacolo = 0;
    int perdente = -1;

    auxGiocatore = listaGiocatore;
    while (auxGiocatore != NULL && perdente == -1) {

        //inizializzo l'array di conteggio delle carte a 0
        for (int i = 0; i < ESAME; ++i) {
            arrayContCarte[i] = 0;
        }
        contaTipoOstacolo = 0;


        auxOstacolo = auxGiocatore->manoOstacolo;

        //conto le carte ostacolo per ciascun tipo
        while (auxOstacolo != NULL) {

            for (int i = 0; i < ESAME; ++i) {
                if (i == (auxOstacolo->tipoOstacolo)) {
                arrayContCarte[i]++;
                }

            }
            auxOstacolo = auxOstacolo->nextOstacolo;
        }

        //controllo se ha almeno una carta di ogni tipo
        for (int i = 0; i < ESAME; ++i) {
            if (arrayContCarte[i] > 0) {
                contaTipoOstacolo++;
            }
        }
        //controllo se ci sono almeno 3 carte ognuna diversa dall'altra
        if (contaTipoOstacolo >= OSTACOLI_PERDENTI) {
            perdente = contGiocatore;
        }

        //sommo i jolly agli altri tipi di ostacoli
        for (int i = 0; i < ESAME - 1; ++i) {
            arrayContCarte[i] += arrayContCarte[ESAME - 1];
        }

        //stampa debug arr
        /*printf("\narr cont carte\n");
        for (int i = 0; i < ESAME; ++i) {

            printf("%d\t", arrayContCarte[i]);
        }*/

        //conto quante carte ci sono dello stesso tipo ed elimino chi ne ha 3 (ostacoli perdenti)
        for (int i = 0; i < ESAME; ++i) {
            if (arrayContCarte[i] >= OSTACOLI_PERDENTI) {
                perdente = contGiocatore;
            }
        }

        contGiocatore++;
        auxGiocatore = auxGiocatore->nextGiocatore;
    }

    free(arrayContCarte);

    return perdente;
}

/**
 * Identifica e visualizza i giocatori vincenti sulla base dei punteggi accumulati.
 *
 * @param listaGiocatore puntatore alla testa della lista dei giocatori
 * @param numGiocatori numero totale di giocatori
 * @param contVincenti numero di giocatori vincenti trovati
 * @param arrayIndexVincenti array contenente gli indici dei giocatori vincenti
 */
void giocatoriVincenti(Giocatore *listaGiocatore, int numGiocatori, int contVincenti, const int *arrayIndexVincenti){
    Giocatore *auxGiocatore = listaGiocatore;
    int contScorrimento = 0;
    FILE *log = NULL;

    //verifico il numero di giocatori vincenti e visualizza i messaggi di vittoria o pareggio
    auxGiocatore = listaGiocatore;

    //se il vincitore è solo uno
    if(contVincenti == 1){
        while (auxGiocatore != NULL){

            //cerco il vincitore e stampo il suo messaggio di vittoria
            if(arrayIndexVincenti[contScorrimento] == contScorrimento){

                log = fopen("log.txt", "a");
                if(log == NULL) exit(-5);
                fprintf(log, "\n\nIl giocatore %s ha vinto la partita\n\n", auxGiocatore->nomeUtente);
                fclose(log);

                printf(YEL "=====================!! IL GIOCATORE %s HA VINTO  !!=====================\n" RESET, auxGiocatore->nomeUtente);
            }
            contScorrimento++;
            auxGiocatore = auxGiocatore->nextGiocatore;
        }
        getchar();
        getchar();
    }else
    //se ci sono piu' vincitori
    if(contVincenti > 1){

        printf("Ha vinto piu' di un giocatore!!\n");

        log = fopen("log.txt", "a");
        if(log == NULL) exit(-5);
        fprintf(log, "Piu' di un giocatore ha vinto\n");
        fclose(log);

        //scorro e stampo tutti i vincitori
        while (auxGiocatore != NULL){

            if(arrayIndexVincenti[contScorrimento] == contScorrimento){

                log = fopen("log.txt", "a");
                if(log == NULL) exit(-5);
                fprintf(log, "\n\nIl giocatore %s ha vinto la partita\n\n", auxGiocatore->nomeUtente);
                fclose(log);

                printf(YEL "\n=====================!! IL GIOCATORE %s HA VINTO  !!=====================\n"RESET , auxGiocatore->nomeUtente);
                getchar();
                getchar();
            }

            contScorrimento++;
            auxGiocatore = auxGiocatore->nextGiocatore;
        }
    } else
    //oppure il vincitore è l'unico giocatore rimasto in partita
    if( numGiocatori == 1){

        log = fopen("log.txt", "a");
        if(log == NULL) exit(-5);
        fprintf(log, "\n\nIl giocatore %s ha vinto la partita\n\n", auxGiocatore->nomeUtente);
        fclose(log);

        printf(YEL "\n=====================!! IL GIOCATORE %s HA VINTO  !!=====================\n"RESET , auxGiocatore->nomeUtente);
        getchar();
        getchar();
    }
}
