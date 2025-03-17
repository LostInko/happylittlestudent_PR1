//
// Created by matte on 16/02/2024.
//

#include <stdio.h>
#include "caricamento.h"
#include "stampeVarie.h"
#include "ANSI-color-codes.h"

/**
 * stampa le informazioni complete dei giocatori e delle carte, utilizzata per il debug
 *
 * @param listaGiocatore puntatore alla testa della lista dei giocatori
 */
void stampaGiocatoreCompleta(Giocatore *listaGiocatore) {

    printf("\n############### STAMPA DEBUG GIOCATORE #################\n");

    //scorro i giocatori
    while (listaGiocatore != NULL) {

        printf("-->%s\n", listaGiocatore->nomeUtente);
        printf("-->%s\n", listaGiocatore->personaggio.nomePersonaggio);
        printf("-->%d\n", listaGiocatore->cfuAccumulati);

        printf("-->Carte in mano:\n");
        while (listaGiocatore->manoCarta != NULL) {
            printf("%-20s\t", listaGiocatore->manoCarta->nomeCarta);
            printf("%5d\n", listaGiocatore->manoCarta->valore);
            listaGiocatore->manoCarta = listaGiocatore->manoCarta->nextCarta;
        }
        printf("\n");

        //punto al prossimo giocatore della lista
        listaGiocatore = listaGiocatore->nextGiocatore;
    }
}

/**
 * stampa l'effetto della carta corrispondente all'indice, utilizzato per stampare le enumerazioni
 *
 * @param index rappresenta quale effetto della carta da stampare
 */
void stampaEffettoCarta(int index) {

    char arrayEffettoCarta[][DIM_STR] = {"NESSUNO",
                                         "SCARTAP",
                                         "RUBA",
                                         "SCAMBIADS",
                                         "SCARTAE",
                                         "SCARTAC",
                                         "SCAMBIAP",
                                         "DOPPIOE",
                                         "SBIRCIA",
                                         "SCAMBIAC",
                                         "ANNULLA",
                                         "AUMENTA",
                                         "DIMINUISCI",
                                         "INVERTI",
                                         "SALVA",
                                         "DIROTTA"};

    printf("-->%s\n", arrayEffettoCarta[index]);

}

/**
 * stampa il tipo di ostacolo corrispondente all'indice, utilizzato per la stampa delle enumerazioni
 *
 * @param index rappresenta quale tipo di ostacolo stampare
 */
void stampaTipoOstacolo(int index) {

    char arrayTipoOstacolo[][DIM_STR] = {"STUDIO",
                                         "SOPRAVVIVENZA",
                                         "SOCIALE",
                                         "ESAME"};

    printf("-->[%s]\n", arrayTipoOstacolo[index]);
}

/**
 * stampa le informazioni sull'ostacolo "a terra" cioè in gioco
 *
 * @param ostacoloTerra puntatore all'ostacolo a terra da stampare
 */
void stampaOstacoloTerra(Ostacolo *ostacoloTerra){

    //stampo le informazioni varie sull'ostacolo
    printf("\nL'ostacolo pescata e': \"%s\"\n\n", ostacoloTerra->nomeOstacolo);
    printf(BLU "---------------------Info Ostacolo---------------------\n" RESET);
    printf("-->%s\n", ostacoloTerra->nomeOstacolo);
    stampaTipoOstacolo(ostacoloTerra->tipoOstacolo);
    printf("%s\n", ostacoloTerra->descOstacolo);
}

/**
 * stampo le informazioni del giocatore senza impostare il puntatore al prossimo giocatore
 * (viene usato nella funzione per la giocata uno)
 *
 * @param auxGiocatore puntatore al giocatore di cui stampare le informazioni
 * @param dim dimensione array dei bonus malus
 */
void stampaGiocatoreTurnoNoNext(Giocatore *auxGiocatore, int dim){

    //stampa informazioni di base del giocatore
    printf(BLU "\n---------------------Info Giocatore----------------------\n" RESET);
    printf("-->%s\n", auxGiocatore->nomeUtente);
    printf("-->%s\n", auxGiocatore->personaggio.nomePersonaggio);

    //stampo l'array dei bonus malus
    for (int i = 0; i < dim; ++i) {
        printf("%d\t", auxGiocatore->personaggio.arrayBonMal[i]);
    }
    printf("\n");
    printf("-->%d\n", auxGiocatore->cfuAccumulati);

    //stampo gli ostacoli del giocatore
    stampaOstacoli(auxGiocatore);

    //stampo le carte del giocatore
    stampaCarte(auxGiocatore);

}

/**
 * stampo le informazioni del giocatore e della sua mano senza impostare il puntatore al prossimo giocatore
 * (viene usato nella funzione per il turno supplementare)
 *
 * @param auxGiocatore puntatore al giocatore di cui stampare le informazioni
 */
void stampaGiocatoreTurnoSupplementare(Giocatore *auxGiocatore){

    //stampa informazioni di base del giocatore
    printf(BLU "\n--------Info Giocatore---------\n" RESET);
    printf("-->%s\n", auxGiocatore->nomeUtente);
    //stampo le carte del giocatore
    stampaCarte(auxGiocatore);
}

/**
 * stampo le carte nella mano di un giocatore, usato per il debug
 *
 * @param manoCarta puntatore alla testa della lista delle carte in mano al giocatore
 */
void stampaCarteGiocatore(Carta *manoCarta){
    Carta *cartaAttuale = manoCarta;
    while (cartaAttuale != NULL) {
        printf("- %s\n", cartaAttuale->nomeCarta);
        cartaAttuale = cartaAttuale->nextCarta;
    }
}

/**
 * stampo le carte dalla mano del giocatore mostrando tutte le informazioni della carta
 *
 * @param auxGiocatore puntatore al giocatore di cui si stampano le carte
 */
void stampaCarte(Giocatore *auxGiocatore){
    int cont = 0;
    Carta *cartaAttuale = auxGiocatore->manoCarta;

    printf(GRN "\n--------- Carte nella tua mano ---------\n" RESET);
    while (cartaAttuale != NULL) {

        printf("[%d] ", cont + 1);
        printf("%-20s", cartaAttuale->nomeCarta);
        printf("%5d\t", cartaAttuale->valore);
        stampaEffettoCarta(cartaAttuale->effettoCarta);
        cont++;

        cartaAttuale = cartaAttuale->nextCarta;
    }
}

/**
 * stampa gli ostacoli nella mano del giocatore e le relative informazioni
 * senza stampare la descrizione
 *
 * @param auxGiocatore puntatore al giocatore di cui si vogliono stampare le informazioni
 */
void stampaOstacoli(Giocatore *auxGiocatore){
    Ostacolo *ostacoloAttuale = auxGiocatore->manoOstacolo;

    printf(GRN "\n---- Ostacoli nella tua mano ----\n" RESET);
    if (ostacoloAttuale == NULL) {
        printf("Non hai carte ostacolo nella tua mano!\n");
    }else{
        while (ostacoloAttuale != NULL) {

            printf("%-20s\t", ostacoloAttuale->nomeOstacolo);
            stampaTipoOstacolo(ostacoloAttuale->tipoOstacolo);

            ostacoloAttuale = ostacoloAttuale->nextOstacolo;
        }
    }

}

/**
 * stampa l'effetto della carta corrispondente all'indice, utilizzato per stampare le enumerazioni
 * da utilizzare durante l'attivazione degli effetti
 *
 * @param index rappresenta quale effetto della carta da stampare
 */
void stampaEffettoCartaAttivazione(int index) {

    char arrayEffettoCarta[][DIM_STR] = {"NESSUNO",
                                         "SCARTAP",
                                         "RUBA",
                                         "SCAMBIADS",
                                         "SCARTAE",
                                         "SCARTAC",
                                         "SCAMBIAP",
                                         "DOPPIOE",
                                         "SBIRCIA",
                                         "SCAMBIAC",
                                         "ANNULLA",
                                         "AUMENTA",
                                         "DIMINUISCI",
                                         "INVERTI",
                                         "SALVA",
                                         "DIROTTA"};

    printf("[%s]", arrayEffettoCarta[index]);

}