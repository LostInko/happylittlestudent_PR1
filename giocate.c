//
// Created by matte on 16/02/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include "caricamento.h"
#include "preparazionegioco.h"
#include "giocate.h"
#include "utility.h"
#include "stampeVarie.h"
#include "ANSI-color-codes.h"

/**
 * esegue la giocata uno per un solo giocatore, dunque scarta le carte dalla mano se ha
 * solo carte effetto, procede a pescare una carta facendo in modo che siano solo carte
 * punto e infine mette la carta in una lista in gioco
 *
 * @param listaGiocatore puntatore alla testa della lista giocatori
 * @param listaCarta puntatore alla testa della lista delle carte (mazzo carte)
 * @param listaCartaScarto puntatore alla testa della lista carte scartate
 * @param listaCartaInGioco puntatore alla testa della lista delle carte in gioco in quel turno
 */
void giocataUno(Giocatore *listaGiocatore, Carta **listaCarta, Carta **listaCartaScarto, Carta **listaCartaInGioco) {

    FILE *log = NULL;

    int sceltaCarta;
    int contaCartaEffetto = 0;
    int contaCartaMano = 0;
    Giocatore *auxGiocatore = listaGiocatore;
    Carta *auxManoCarta = auxGiocatore->manoCarta;
    Carta *copiaCarta = NULL;

    //stampa di debug delle carte in mano al giocatore prima di iniziare
//    printf("Stato attuale della mano del giocatore:\n");
//    stampaCarteGiocatore(auxManoCarta);

    //controllo quante sono le carte effetto e quante sono le carte nella mano
    while (auxManoCarta != NULL ) {
        if (auxManoCarta->effettoCarta > ANNULLA) {
            contaCartaEffetto++;
        }
        contaCartaMano++;
        auxManoCarta = auxManoCarta->nextCarta;
    }

    //stampa di debug del numero di carte effetto trovate
//    printf("Numero di carte con effetto trovate: %d\n", contaCartaEffetto);

    //se le carte in mano sono lo stesso numero di quelle effetto vengono tutte scartate e ripescate
    auxGiocatore = listaGiocatore;
    if (contaCartaEffetto == contaCartaMano) {
        pescaCarta(&(auxGiocatore->manoCarta), listaCartaScarto, START_CARTA, listaCartaScarto);
        pescaCarta(listaCarta, &(auxGiocatore->manoCarta), START_CARTA, listaCartaScarto);

        printf("##### Nella tua mano avevi solo carte effetto percio' sono state scartate automaticamente e sostituite #####\n");
        stampaCarte(auxGiocatore);
    }

    //stampa di debug della mano del giocatore
//    printf("Stato aggiornato della mano del giocatore:\n");
//    stampaCarteGiocatore(auxGiocatore->manoCarta);

    //il giocatore sceglie una carta da giocare finché non sceglie una carta non effetto
    auxGiocatore = listaGiocatore;
    do {

        //richiamo la funzione sceltaCartaInGioco per scegliere la carta
        sceltaCarta = sceltaCartaInGioco(contaCartaMano);

        //stampa di debug della scelta
//        printf("scelta %d\n", sceltaCarta);

        //richiamo la funzione che mi permette di copiare la carta scelta per verificare se è una carta effetto
        copiaCartaScelta(auxGiocatore, sceltaCarta, &copiaCarta);
        if (copiaCarta->effettoCarta > ANNULLA) {
            printf("Errore! La carta \"%s\" non e' di tipo punto! Pesca ancora!\n", (*copiaCarta).nomeCarta);
        } else{

            //salvo sul log chi pesca e quale carta
            log = fopen("log.txt", "a");
            if(log == NULL) exit(-5);
            fprintf(log, "Il giocatore %s gioca %s\n", auxGiocatore->nomeUtente, copiaCarta->nomeCarta);
            fclose(log);
        }
    } while (copiaCarta->effettoCarta > ANNULLA);

    //una volta confermata la carta viene pescata quella effettiva
    auxGiocatore = listaGiocatore;
    pescaCartaScelta(auxGiocatore, sceltaCarta, listaCartaInGioco);

    //stampa debug della mano aggiornata
//    printf("Stato aggiornato della mano del giocatore:\n");
//    stampaCarteGiocatore(auxGiocatore->manoCarta);

    //stampa debug della lista carte in gioco aggiornata
//    printf("Stato aggiornato della lista in gioco:\n");
//    stampaCarteGiocatore(*listaCartaInGioco);

}

/**
 * permette di terminare la partita, è la scelta zero con la free generale richiamata
 * e il codice di uscita per terminare la partita normalmente
 *
* @param listaGiocatore Puntatore al puntatore alla testa della lista dei giocatori.
 * @param listaCarta Puntatore al puntatore alla testa della lista delle carte (mazzo carta)
 * @param listaOstacolo Puntatore al puntatore alla testa della lista degli ostacoli (mazzo ostacoli)
 * @param personaggio Puntatore al personaggio principale.
 *
 * @exit "0" terminatore regolare della partita
 */
void terminatorePartita(Giocatore **listaGiocatore, Carta **listaCarta, Ostacolo **listaOstacolo, Personaggio *personaggio, int *arrayIndexVincenti) {
    printf("Partita Terminata\n");

    freeVarie(*listaGiocatore, *listaCarta, *listaOstacolo, personaggio, arrayIndexVincenti);

    exit(0);
}

/**
 * stampa le statistiche di un giocatore, escludendo il giocatore corrente
 *
 * @param auxStampaStats puntatore al giocatore di cui stampare le statistiche
 * @param auxGiocatore puntatore al giocatore corrente
 * @param dim dimensione dell'array delle statistiche del personaggio
 */
void controllaStats(Giocatore *auxStampaStats, Giocatore *auxGiocatore, int dim) {
    int cont = 0;
    Ostacolo *ostacoloAttuale = NULL;

    //scorro tutti i giocatori
    while (auxStampaStats != NULL) {

        //controllo se il giocatore non è quello corrente e in tal caso entro nell'if
        if (auxStampaStats->personaggio.nomePersonaggio != auxGiocatore->personaggio.nomePersonaggio) {

            //stampo le stats
            printf(BLU "\n-------------- Stats Giocatori -------------\n" RESET);
            printf("-->%s\n", auxStampaStats->nomeUtente);
            printf("-->%s\n", auxStampaStats->personaggio.nomePersonaggio);
            for (int i = 0; i < dim; ++i) {
                printf("%d\t", auxStampaStats->personaggio.arrayBonMal[i]);
            }
            printf("\n");
            printf("-->%d\n", auxStampaStats->cfuAccumulati);

            printf("-->Ostacoli:\n");
            ostacoloAttuale = auxStampaStats->manoOstacolo;
            while (ostacoloAttuale != NULL) {

                printf("%-20s", ostacoloAttuale->nomeOstacolo);
                stampaTipoOstacolo(ostacoloAttuale->tipoOstacolo);
                cont++;
                ostacoloAttuale = ostacoloAttuale->nextOstacolo;
            }

            //se il giocatore non ha carte ostacolo
            if (auxStampaStats->manoOstacolo == NULL) {
                printf("Non ha carte ostacolo!!\n");
            }
        }
        //punto al prossimo giocatore
        auxStampaStats = auxStampaStats->nextGiocatore;
    }
}

/**
 * gestisce la richiesta di conferma per terminare la partita
 *
 * @return la scelta finale dell'utente
 */
char sceltaTerminaPartita() {
    char sceltaFinale;

    do {
        //consumo il carattere residuo
        getchar();
        printf("Sei sicuro di voler terminare la partita?\t[s][n]\n");
        scanf("%c", &sceltaFinale);

        //verifico se la scelta è valida
        if (sceltaFinale != 'n' && sceltaFinale != 's') {
            printf("Errore! Fai una scelta tra 's' e 'n'!\n");
        }
    } while (sceltaFinale != 'n' && sceltaFinale != 's');

    //restituisco la scelta dell'utente
    return sceltaFinale;
}

/**
 * gestisce la scelta del tipo di azione da compiere durante il turno
 *
 * @return l'azione scelta dall'utente
 */
int sceltaTurno() {
    int scelta;
    char sceltaFinale;

    do {
        //mostro le opzioni disponibili
        printf("\n[TASTO 1] Giocare una carta CFU dalla propria mano\n"
               "[TASTO 2] Controllare lo stato degli altri giocatori (CFU e carte ostacolo)\n"
               "[TASTO 0] Uscire dalla partita\n");
        scanf("%d", &scelta);

        //controllo che la scelta sia valida
        if (scelta != GIOCA_CARTA && scelta != CONTROLLA_STATS && scelta != FINE_PARTITA) {
            printf("Errore! Fai una scelta valida compresa tra 0 e 2!\n");
        }

        //se viene scelto di terminare la partita chiedo conferma
        if (scelta == FINE_PARTITA) {
            sceltaFinale = sceltaTerminaPartita();
            if (sceltaFinale == 'n') {

                //in caso imposto la scelta a un valore non valido per continuare il ciclo
                scelta = -1;
            }
        }
    } while (scelta != GIOCA_CARTA && scelta != CONTROLLA_STATS && scelta != FINE_PARTITA);

    //restituisco l'intero contente la scelta
    return scelta;
}

/**
 * raggruppa le funzioni per liberare la memoria allocata per le strutture collegate al gioco
 *
 * @param listaGiocatore Un puntatore alla testa della lista dei giocatori da liberare.
 * @param listaCarta Un puntatore alla testa della lista delle carte da liberare.
 * @param listaOstacolo Un puntatore alla testa della lista degli ostacoli da liberare.
 * @param personaggio Un puntatore al personaggio da liberare.
 */
void freeVarie(Giocatore *listaGiocatore, Carta *listaCarta, Ostacolo *listaOstacolo, Personaggio *personaggio, int *arrayIndexVincenti) {
    eliminaListaGiocatore(listaGiocatore);
    eliminaListaCarta(listaCarta);
    eliminaListaOstacolo(listaOstacolo);
    free(arrayIndexVincenti);
    free(personaggio);
}

