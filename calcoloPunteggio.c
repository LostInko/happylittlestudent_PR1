//
// Created by matte on 21/02/2024.
//
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "caricamento.h"
#include "utility.h"
#include "stampeVarie.h"
#include "calcoloPunteggio.h"
#include "effettoCarte.h"
#include "ANSI-color-codes.h"

/**
 * Gestisce il turno supplementare, con eventuali pareggi e turni extra
 *
 * @param listaGiocatore puntatore alla testa della lista dei giocatori.
 * @param listaCartaScarto puntatore al puntatore alla testa della lista delle carte scartate.
 * @param dimNumGiocatori dimensione dell'array da cui vengono presi gli indici per lo scorrimento (maxIndex e contStartMin)
 * @param numGiocatori numero totale dei giocatori.
 * @param maxIndex indice del giocatore con il punteggio massimo su cui il turno supplementare non si applica
 * @param contStartMin il valore di partenza per lo scorrimento dei giocatori ( i precedenti sono impostati a PERDENTE)
 * @return l'indice del giocatore che ha perso
 */
int turnoSupplementare(Giocatore *listaGiocatore, Carta **listaCartaScarto, int dimNumGiocatori, int numGiocatori,int maxIndex, int contStartMin){

    //contatori
    int cont, contaCartaEffetto, contaCartaMano, contaNoCarteGiocabili;

    int indicePerdente = PERDENTE;
    int min;
    int sceltaCarta;
    int minIndex;
    bool turnoExtra;
    int *arrayPunti = allocaArrayInteri(numGiocatori);
    Giocatore *auxGiocatore = listaGiocatore;
    Giocatore *auxExtraGiocatore = listaGiocatore;
    Carta *copiaCarta = NULL;
    Carta *auxManoCarta = NULL;
    FILE *log = NULL;

    //inizializzazione dell'array dei punteggi supplementari
    for (int i = 0; i < numGiocatori; ++i) {
        arrayPunti[i] = PERDENTE;
    }

    //inizio il ciclo dei turni
    do {
        log = fopen("log.txt", "a");
        if(log == NULL) exit(-5);
        fprintf(log, "\nTURNO SUPPLEMENTARE\n");
        fclose(log);

        printf("--------Turno supplementare------\n");

        //inizializzazioni variabili turno
        turnoExtra = false;
        contaNoCarteGiocabili = 0;
        auxGiocatore = listaGiocatore;
        auxExtraGiocatore = listaGiocatore;

        //scorro i giocatori iniziando subito dopo l'inizializzazione a PERDENTE, scorro della dimensione dell'array da cui considero maxIndex
        for (int i = contStartMin; i < dimNumGiocatori; ++i) {
            cont = 0;

            //scorro tutti i giocatori
            while (auxExtraGiocatore != NULL ) {

                //inizializzo contatori giocatore
                contaCartaEffetto = contaCartaMano = 0;

                //se sta giocando un giocatore diverso dal vincitore entra
                if (maxIndex != cont) {

                    // Verifica quante carte con effetto ci sono nella mano del giocatore
                    auxManoCarta = auxExtraGiocatore->manoCarta;
                    stampaGiocatoreTurnoSupplementare(auxExtraGiocatore);
                    while (auxManoCarta != NULL) {
                        if (auxManoCarta->effettoCarta > ANNULLA) {
                            contaCartaEffetto++;
                        }
                        contaCartaMano++;
                        auxManoCarta = auxManoCarta->nextCarta;
                    }
                    //stampa di debug di carte effetto e carte totali
//                    printf("Carte effetto %d\n", contaCartaEffetto);
//                    printf("Carte tot %d\n", contaCartaMano);

                    //prima di pescare controllo se ha abbastanza carte o se puo giocarne
                    auxGiocatore = listaGiocatore;
                    if (contaCartaEffetto == contaCartaMano || contaCartaMano == 0) {
                        printf("Non puoi giocare nessuna carta!\n");

                        log = fopen("log.txt", "a");
                        if(log == NULL) exit(-5);
                        fprintf(log, "Il giocatore %s non puo' giocare alcuna carta\n", auxGiocatore->nomeUtente);
                        fclose(log);

                        //l'array dei punti ottiene valore PERDENTE
                        arrayPunti[cont] = PERDENTE;

                        //salvo l'indice del perdente e conto che non puoi giocare
                        indicePerdente = cont;
                        contaNoCarteGiocabili++;
                    }
                    else {
                        //altrimenti procedo con la pesca della carta
                        auxGiocatore = listaGiocatore;
                        do {

                            //scelgo la carta richiamando la funzione sceltaCartaInGioco
                            sceltaCarta = sceltaCartaInGioco(contaCartaMano);

                            //copio la carta richiamando la funzione e verifico se posso pescarla
                            copiaCartaScelta(auxExtraGiocatore, sceltaCarta, &copiaCarta);
                            if (copiaCarta->effettoCarta > ANNULLA) {
                                printf("Errore! La carta \"%s\" non e' di tipo punto!\n", (*copiaCarta).nomeCarta);
                            }else {

                                log = fopen("log.txt", "a");
                                if(log == NULL) exit(-5);
                                fprintf(log, "Il giocatore %s ha ha giocato %s\n", auxGiocatore->nomeUtente, copiaCarta->nomeCarta);
                                fclose(log);

                                //se posso pescarla mi salvo il valore della carta nell'array dei punti
                                arrayPunti[cont] = copiaCarta->valore;
                            }
                        } while (copiaCarta->effettoCarta > ANNULLA);

                        //pesco la carta scelta dal mazzo
                        auxGiocatore = listaGiocatore;
                        pescaCartaScelta(auxExtraGiocatore, sceltaCarta, listaCartaScarto);

                    }
                }
                cont++;

                //scorro al giocatore successivo
                auxExtraGiocatore = auxExtraGiocatore->nextGiocatore;
            }
        }

        //stampe di debug e inserimento manuale dei punteggi
        /*for (int i = 0; i < numGiocatori; ++i) {
            printf("%d\t", arrayPunti[i]) ;

        }
        printf("Ins array:");
        for (int i = 0; i < numGiocatori; ++i) {
            scanf("%d", &arrayPunti[i]) ;

        }
        printf("\n");
        for (int i = 0; i < numGiocatori; ++i) {
            printf("%d\t", arrayPunti[i]) ;

        }*/

        //trovo il nuovo punteggio minimo del turno supplementare
        min = trovaMin(numGiocatori, arrayPunti, &minIndex);

        //stampa di debug del minimo trovato
//        printf("min %d", min);

        //controllo se tutti i giocatori potevano giocare il turno
        if (contaNoCarteGiocabili == 0) {

            //nel caso tutti i giocatori abbiano giocato una carta
            indicePerdente = minIndex;

            //cerco un pareggio scorrendo il punteggio e considerando solo i punteggi maggiori di PERDENTE
            for (int i = 0; i < numGiocatori - 1; i++) {
                for (int j = i + 1; j < numGiocatori; j++) {
                    if (arrayPunti[i] == arrayPunti[j] && arrayPunti[i] >= 0 && arrayPunti[j] >= 0) {

                        //se trovo un pareggio imposto turnoExtra a true e il doWhile si ripete
                        if (arrayPunti[i] == min) {
                            printf(RED "========== Pareggio ==========\n" RESET);

                            log = fopen("log.txt", "a");
                            if(log == NULL) exit(-5);
                            fprintf(log, "Pareggio nel turno supplementare\n\n");
                            fclose(log);

                            turnoExtra = true;
                        }
                    }
                }
            }
        }

        //per default se solo un giocatore non può giocare niente viene considerato perdente del turno

        //stampa di debug dell'indice del perdente
//        printf("Indice perdente %d", indicePerdente);

    } while (turnoExtra);

    //libero la memoria allocata dinamicamente
    free(arrayPunti);

    //restituisco l'indice del perdente
    return indicePerdente;
}

/**
 * Calcola il punteggio del turno per ciascun giocatore, considerando le carte in gioco e
 * i bonus/malus del personaggio, attivo gli effetti e gioco le istantanee punteggio
 * dopo il calcolo, gestisce il risultato del turno.
 *
 * @param numGiocatori numero di giocatori.
 * @param listaCartaInGioco puntatore alla testa della lista delle carte in gioco.
 * @param listaGiocatore puntatore al puntatore alla testa della lista dei giocatori.
 * @param listaCarta puntatore al puntatore alla testa della lista delle carte (mazzo)
 * @param tipoOstacolo tipo di ostacolo a terra
 * @param ostacoloTerra puntatore al puntatore alla testa della lista degli ostacoli a terra.
 * @param listaCartaScarto puntatore al puntatore alla testa della lista delle carte scartate.
 * @param listaOstacolo puntatore al puntatore alla testa della lista degli ostacoli.
 */
void calcoloPunteggioTurno(int numGiocatori,Carta *listaCartaInGioco, Giocatore **listaGiocatore, Carta **listaCarta,
                           int tipoOstacolo, Ostacolo **ostacoloTerra, Carta **listaCartaScarto, Ostacolo **listaOstacolo) {


    bool ripeti = true;
    bool raddoppio = false;
    int max;
    int cont = 0;
    int *arrayPunteggi = allocaArrayInteri(numGiocatori);
    Carta *auxCartaInGioco = NULL;
    Giocatore *auxGiocatore = *listaGiocatore;
    int *arrayEffetti = allocaArrayInteri(numGiocatori);
    int *arrayPuntiEffetto = allocaArrayInteri(numGiocatori);
    int maxIndex;

    //azzero l'array
    for (int i = 0; i < numGiocatori; ++i) {
        arrayPuntiEffetto[i] = 0;
    }

    //inserisco il punteggio
    auxCartaInGioco = listaCartaInGioco;
    while (auxCartaInGioco != NULL) {
        arrayPunteggi[cont] = auxCartaInGioco->valore;
        cont++;
        auxCartaInGioco = auxCartaInGioco->nextCarta;
    }

    //stampa di debug dei punteggi prima di essere alterati
    /*  printf("Punteggi pre bm:\t");
    for (int i = 0; i < numGiocatori; ++i) {
        printf("%d\t", arrayPunteggi[i]);
    }*/

    //aggiungo i bonus e malus in base al tipo di ostacolo
    cont = 0;
    while (auxGiocatore != NULL) {
        arrayPunteggi[cont] += auxGiocatore->personaggio.arrayBonMal[tipoOstacolo];
        cont++;

        auxGiocatore = auxGiocatore->nextGiocatore;
    }

    //stampa di debug dei punteggi
    /*    printf("\nPunteggi post bm:\t");
        for (int i = 0; i < numGiocatori; ++i) {
            printf("%d\t", arrayPunteggi[i]);
        }
        printf("\n");*/



    ///attivo le carte effetto
    printf( BOLD "--------- Premi qualsiasi tasto per passare all'attivazione delle carte! ---------\n" RESET );
    getchar();
    getchar();
    clearConsole();

    printf(RED "\n==================== Attivazione effetti delle carte ====================\n\n" RESET);
    //mi salvo gli effetti di tutte le carte in gioco
    auxCartaInGioco = listaCartaInGioco;
    cont = 0;
    while (auxCartaInGioco != NULL) {
        arrayEffetti[cont] = auxCartaInGioco->effettoCarta;
//        printf("%d", arrayEffetti[cont]);
        cont++;
        auxCartaInGioco = auxCartaInGioco->nextCarta;
    }

    //stampo tutte le carte in gioco
    auxCartaInGioco = listaCartaInGioco;
    cont = 0;
    printf("\n--------- Carte in gioco ---------\n");
    while (auxCartaInGioco != NULL) {

        printf("[%d] ", cont + 1);
        printf("%-20s", auxCartaInGioco->nomeCarta);
        printf("%5d\t", auxCartaInGioco->valore);
        stampaEffettoCarta(auxCartaInGioco->effettoCarta);
        cont++;

        auxCartaInGioco = auxCartaInGioco->nextCarta;
    }
    printf("\n");

    //inserimento manuale effetti e stampa di debug
    /*printf("Inserimento manuale effetti");
    auxCartaInGioco = listaCartaInGioco;
    for (int i = 0; i < numGiocatori; ++i) {

        printf("%d ", i);
        scanf("%d", &arrayEffetti[i]);
    }
    printf("effetti:\t");
            for (int j = 0; j < numGiocatori; ++j) {
                printf("%d\t", arrayEffetti[j]);
            }
            printf("\n");
*/
    //gioco gli effetti in ordine dal piu' grande
    auxGiocatore = *listaGiocatore;
    for (int i = 0; i < numGiocatori; ++i) {

        cont = 0;

        //cerco il max e il suo indice
        max = trovaMax(numGiocatori, arrayEffetti, &maxIndex);

        //stampa ddi debug
        /*printf("\nMax %d\t max index %d\n", max, maxIndex);
        printf("effetti:\n");
        for (int j = 0; j < numGiocatori; ++j) {
            printf("%d\t", arrayEffetti[j]);
        }
        printf("\n");
        printf("%d", arrayEffetti[cont]);*/

        auxGiocatore = *listaGiocatore;
        while (auxGiocatore != NULL ) {

            //se il giocatore che scorre corrisponde all'effetto massimo e questo è diverso dall'effetto zero
            if(cont == maxIndex && max != 0){

                printf(BLU "------------ Attivazione effetto di %s ------------\n\n" RESET, auxGiocatore->nomeUtente);


                //gioco l'effetto e mi salvo il booleano in caso di raddoppio dei punteggi
                raddoppio = giocaCartaEffetto(arrayEffetti[cont],  listaGiocatore, listaCarta,
                                              &auxGiocatore, &listaCartaInGioco, listaCartaScarto,
                                              cont, arrayPunteggi, numGiocatori, arrayEffetti,
                                              arrayPuntiEffetto);
                printf("\n");

                //imposto l'effetto appena giocato a -1 in modo da non rifarlo giocare
                arrayEffetti[maxIndex] = -1;
            }

            cont++;
            auxGiocatore = auxGiocatore->nextGiocatore;

        }

    }

    //stampa di debug array effetti
    /*
    printf("effetti:\t");
    for (int j = 0; j < numGiocatori; ++j) {
        printf("%d\t", arrayEffetti[j]);
    }
    printf("\n");
    */

    //se l'effetto vale zero
    cont = 0;
    auxGiocatore = *listaGiocatore;
    while (auxGiocatore != NULL ) {

        if(arrayEffetti[cont] == 0){
            printf(BLU "------------ La carta di %s non ha effetto... ------------\n\n" RESET, auxGiocatore->nomeUtente);
        }
        cont++;
        auxGiocatore = auxGiocatore->nextGiocatore;

    }

    //stampa debug di effetti e array punti effetto
    /*printf("effetti:\t");
    for (int i = 0; i < numGiocatori; ++i) {
        printf("%d\t", arrayEffetti[i]);
    }
    printf("\n");

    printf("punti effetti:\t");
    for (int i = 0; i < numGiocatori; ++i) {
        printf("%d\t", arrayPuntiEffetto[i]);
    }
    printf("\n");*/

    ///gioco istantanee
    printf( BOLD "--------- Premi qualsiasi tasto per passare alle carte istantanee! ---------\n"RESET );
    getchar();
    getchar();
    clearConsole();

    printf(RED "\n======================== Gioca carte Istantanee ========================\n\n" RESET);
    while (ripeti){

        //richiamo la funzione che restituisce ripeti se c'e' la possibilita' che il giocatore abbia ancora carte istantanee
        ripeti = controlloIstantanee(listaGiocatore, listaCartaScarto,arrayPunteggi, numGiocatori,arrayPuntiEffetto);
    }

    //raddoppio i punteggi effetto se il booleano e' settato a true e li sommo ai punteggi normali
    for (int i = 0; i < numGiocatori; ++i) {
        if(raddoppio){
            arrayPuntiEffetto[i] *= 2;
        }

        arrayPunteggi[i] += arrayPuntiEffetto[i];
    }

    clearConsole();
    printf( BOLD "--------- Premi qualsiasi tasto per visualizzare i risultati! ---------\n" RESET );
    getchar();
    getchar();
    printf(RED "\n------------------ Risultato Round -----------------\n" RESET);

    printf("Punteggi:\t");
    for (int i = 0; i < numGiocatori; ++i) {
        printf("%d\t", arrayPunteggi[i]);
    }
    printf("\n");

    //debug inserimento manuale dei risultati
    /* printf("\n");
    printf("\n");

    printf("Inserimento manuale ris:\n");
    for (int i = 0; i < numGiocatori; ++i) {
        scanf("%d", &arrayPunteggi[i]);
    }
    printf("\n array:");

    for (int i = 0; i < numGiocatori; ++i) {
        printf("%d\t", arrayPunteggi[i]);
    }
    printf("\n");
    printf("\n");*/

    //richiamo la funzione per la gestione dei risultati
    auxGiocatore = *listaGiocatore;
    risultatoRound(numGiocatori, arrayPunteggi, listaGiocatore, ostacoloTerra, listaCartaScarto, listaOstacolo);

    free(arrayEffetti);
    free(arrayPuntiEffetto);
    free(arrayPunteggi);
}

/**
 * Gestisce il risultato del round, determinando i giocatori vincitori e perdenti,
 * in caso di pareggi, gestisce i turni supplementari per determinare il vincitore.
 *
 * @param numGiocatori numero di giocatori.
 * @param arrayPunteggi Array dei punteggi dei giocatori.
 * @param listaGiocatore Puntatore al puntatore alla testa della lista dei giocatori.
 * @param ostacoloTerra Puntatore al puntatore alla testa della lista degli ostacoli a terra.
 * @param listaCartaScarto Puntatore al puntatore alla testa della lista delle carte scartate.
 * @param listaOstacolo Puntatore al puntatore alla testa della lista degli ostacoli.
 */
void risultatoRound(int numGiocatori, int *arrayPunteggi, Giocatore **listaGiocatore, Ostacolo **ostacoloTerra, Carta **listaCartaScarto, Ostacolo **listaOstacolo) {
    int maxIndex, minIndex;
    bool checkPareggioMin = false;
    bool checkPareggioMax = false;
    bool istantaneaTrovata = false;
    char sceltaGioco;
    int indexPerdente;
    int dimNumGiocatori = numGiocatori*2;
    int *arrayIndexMax = allocaArrayInteri(dimNumGiocatori);
    int *arrayIndexMin = allocaArrayInteri(dimNumGiocatori);
    Carta *manoCarta = NULL;
    FILE *log = NULL;

    //contatori
    int contStartMin = 0;
    int contStartMax = 0;
    int contMin = 0;
    int contMax = 0;
    int cont;

    //cerco max e min
    int max = trovaMax(numGiocatori, arrayPunteggi, &maxIndex);
    int min = trovaMin(numGiocatori, arrayPunteggi, &minIndex);

    Giocatore *auxGiocatore = *listaGiocatore;

    //inizializzo a PERDENTE gli array di indici
    for (int i = 0; i < dimNumGiocatori; ++i) {
        arrayIndexMin[i] = PERDENTE;
        arrayIndexMax[i] = PERDENTE;
    }

    //cerco un pareggio
    for (int i = 0; i < numGiocatori - 1; i++) {
        for (int j = i + 1; j < numGiocatori; j++) {

            //se trovo due valori uguali cerco se corrispondono a un max o a un min
            if (arrayPunteggi[i] == arrayPunteggi[j]) {

                if(arrayPunteggi[i] == max){

                    //salvo gli indici dei massimi
                    arrayIndexMax[contMax++] = i;
                    arrayIndexMax[contMax++] = j;

                    //salvo in un booleano il pareggio per il vincitore
                    checkPareggioMax = true;
                } else if(arrayPunteggi[i] == min){

                    //salvo gli indici dei minimi
                    arrayIndexMin[contMin++] = i;
                    arrayIndexMin[contMin++] = j;

                    //salvo in un booleano il pareggio per il perdente
                    checkPareggioMin = true;
                }
            }
        }
    }

    //stampa di debug degli array prima eliminare i duplicati
    /*
    printf("Array min prima di essere eliminati:\n");
    for (int i = 0; i < dimNumGiocatori; i++) {
        printf("%d ", arrayIndexMin[i]);
    }
    printf("Array max prima di essere eliminati:\n");
    for (int i = 0; i < dimNumGiocatori; i++) {
        printf("%d ", arrayIndexMax[i]);
    }
*/

    //elimino i duplicati degli array di indici
    eliminaDuplicati(dimNumGiocatori, arrayIndexMin);
    eliminaDuplicati(dimNumGiocatori, arrayIndexMax);

    //riordino gli array di indici in ordine crescente
    arraySortingCrescente(arrayIndexMax,dimNumGiocatori);
    arraySortingCrescente(arrayIndexMin,dimNumGiocatori);

    //stampa di debug degli array riordinati
    /*
    printf("Array min sortato:\n");
    for (int i = 0; i < dimNumGiocatori; i++) {
        printf("%d ", arrayIndexMin[i]);
    }
    printf("Array max sortato:\n");
    for (int i = 0; i < dimNumGiocatori; i++) {
        printf("%d ", arrayIndexMax[i]);
    }
    */

    //conto quanti sono i max e i min trovati senza includere i PERDENTE
    contMax = contMin = 0;
    for (int i = 0; i < dimNumGiocatori; ++i) {
        if(arrayIndexMax[i] != PERDENTE){
            contMax++;
        }
        if(arrayIndexMin[i] != PERDENTE){
            contMin++;
        }
    }

    //conto quindi da dove partire per gli scorrimenti per non valutare l'espressione quando c'è PERDENTE
    contStartMax = dimNumGiocatori-contMax;
    contStartMin = dimNumGiocatori-contMin;

    //stampa di debug dei dati appena ottenuti
    /*
    printf("cont max %d\n", contMax);
    printf("cont min %d\n", contMin);
    printf("contStartMax %d\n", contStartMax);
    printf("contStartMin %d\n", contStartMin);
    */

    auxGiocatore = *listaGiocatore;
    //verifico se tutti i giocatori hanno vinto il turno
    if (contMax == numGiocatori) {
        printf("Nessun giocatore ha vinto il round!\n");

        //scrivo il log
        log = fopen("log.txt", "a");
        if(log == NULL) exit(-5);
        fprintf(log, "Nessun giocatore ha vinto il round\n");
        fclose(log);

        //se tutti hanno vinto l'ostacolo a terra viene reinserito in coda nella lista ostacoli
        spostaOstacoloInCoda(ostacoloTerra, listaOstacolo);
    } else {

        //gestisco il caso in cui ci sia un pareggio per il punteggio max
        if (checkPareggioMax) {

            printf("Ci sono piu' giocatori vincenti, %d giocatori sono vincitori\n", contMax);
            printf("I vincenti del round sono:\n");

            //scrivo il log
            log = fopen("log.txt", "a");
            if(log == NULL) exit(-5);
            fprintf(log, "Il round e' stato vinto da piu' di un giocatore\n");
            fclose(log);

            //scorro partendo oltre gli indici inizializzati a PERDENTE
            for (int i = contStartMax; i < dimNumGiocatori; ++i) {

                auxGiocatore = *listaGiocatore;
                cont = 0;
                while (auxGiocatore != NULL ) {

                    //controllo se i giocatori controllati corrispondono agli indici max salvati
                    if (arrayIndexMax[i] == cont) {
                        printf("%s\t", auxGiocatore->nomeUtente);
                        auxGiocatore->cfuAccumulati += max;

                        //scrivo il log
                        log = fopen("log.txt", "a");
                        if(log == NULL) exit(-5);
                        fprintf(log, "Il giocatore %s ha vinto il round\n", auxGiocatore->nomeUtente);
                        fclose(log);
                    }
                    cont++;
                    auxGiocatore = auxGiocatore->nextGiocatore;
                }
            }

        } else {
            //se non c'è un pareggio per il vincitore prendo l'indice massimo e gli assegno i punti cfu direttamente
            cont = 0;
            printf("Il vincitore del round e'");

            //scorro i giocatori
            while (auxGiocatore != NULL) {

                //cerco la corrispondenza tra l'indice del max e il contatore
                if (maxIndex == cont) {
                    printf("%s\t", auxGiocatore->nomeUtente);

                    //scrivo il log
                    log = fopen("log.txt", "a");
                    if(log == NULL) exit(-5);
                    fprintf(log, "Il giocatore %s ha vinto il round\n", auxGiocatore->nomeUtente);
                    fclose(log);

                    auxGiocatore->cfuAccumulati += max;
                }
                cont++;
                auxGiocatore = auxGiocatore->nextGiocatore;
            }
        }
        printf("\n");


        //in caso di pareggio tra i perdenti
        cont = 0;
        auxGiocatore = *listaGiocatore;
        if (checkPareggioMin) {

            //richiamo la funzione che fa giocare i turni supplementari finché non viene trovato un vincitore
            indexPerdente = turnoSupplementare(auxGiocatore, listaCartaScarto,dimNumGiocatori, numGiocatori, maxIndex, contStartMin);

            //escludo il caso PERDENTE
            if (indexPerdente >= 0) {
                auxGiocatore = *listaGiocatore;
                printf("Il perdente del round e'");

                //si cerca l'indice del perdente
                while (auxGiocatore != NULL) {
                    if (indexPerdente == cont) {
                        printf("%s\t", auxGiocatore->nomeUtente);

                        //richiama le istantanee
                        manoCarta = auxGiocatore->manoCarta;
                        while (manoCarta != NULL && !istantaneaTrovata) {

                            //se trova una carta salva o dirotta lo segnalo
                            if (manoCarta->effettoCarta == SALVA || manoCarta->effettoCarta == DIROTTA) {

                                istantaneaTrovata = true;
                            }
                            manoCarta = manoCarta->nextCarta;
                        }

                        //in caso ci sia chiedo se vuole giocarla
                        if (istantaneaTrovata){

                            //gli faccio scegliere la carta
                            stampaCarte(auxGiocatore);
                            printf("-------- ?? Vuoi giocare una carta salva o dirotta ?? --------\n");
                            sceltaGioco = sceltaSN();

                            if (sceltaGioco == 's') {

                                //scrivo il log
                                log = fopen("log.txt", "a");
                                if(log == NULL) exit(-5);
                                fprintf(log, "Il giocatore %s ha perso il round e ha pescato %s\n", auxGiocatore->nomeUtente, (*ostacoloTerra)->nomeOstacolo);
                                fclose(log);

                                //chiamo la funzione per giocare le istantanee
                                controllaIstantaneaOstacolo(listaGiocatore, listaCartaScarto, listaOstacolo, ostacoloTerra);
                            }
                        }

                        //se la carta non c'e' o non la si vuole giocare
                        if(!istantaneaTrovata || sceltaGioco == 'n') {

                            //viene assegnata l'ostacolo al perdente
                            pescaOstacolo(ostacoloTerra, &auxGiocatore->manoOstacolo, 1);
                        }
                    }
                    cont++;
                    auxGiocatore = auxGiocatore->nextGiocatore;
                }
            } else{
                //in caso l'indice sia PERDENTE (quindi tutti hanno finito le carte giocabili)
                printf("Nessun giocatore ha perso il turno, la carta ostacolo verrà messa in fondo al mazzo!\n");

                //scrivo il log
                log = fopen("log.txt", "a");
                if(log == NULL) exit(-5);
                fprintf(log, "Nessun giocatore ha perso il round quindi la carta %s e' stata messa in fondo al mazzo\n",(*ostacoloTerra)->nomeOstacolo);
                fclose(log);

                //sposto l'ostacolo in gioco nella coda della lista
                spostaOstacoloInCoda(ostacoloTerra, listaOstacolo);
            }

        } else {

            //in caso ci sia solo un perdente
            printf("Il perdente del round e'");
            while (auxGiocatore != NULL) {

                //cerco il suo indice
                if (minIndex == cont) {

                    printf("%s\t", auxGiocatore->nomeUtente);

                    //richiama le istantanee
                    manoCarta = auxGiocatore->manoCarta;
                    while (manoCarta != NULL && !istantaneaTrovata) {

                        //se trova una carta salva o dirotta lo segnalo
                        if (manoCarta->effettoCarta == SALVA || manoCarta->effettoCarta == DIROTTA) {

                            istantaneaTrovata = true;
                        }
                        manoCarta = manoCarta->nextCarta;
                    }

                    //in caso ci sia chiedo se vuole giocarla
                    if (istantaneaTrovata){

                        //gli faccio scegliere la carta
                        stampaCarte(auxGiocatore);
                        printf("-------- ?? Vuoi giocare una carta salva o dirotta ?? --------\n");
                        sceltaGioco = sceltaSN();

                        if (sceltaGioco == 's') {

                            //scrivo il log
                            log = fopen("log.txt", "a");
                            if(log == NULL) exit(-5);
                            fprintf(log, "Il giocatore %s ha perso il round e ha pescato %s\n", auxGiocatore->nomeUtente, (*ostacoloTerra)->nomeOstacolo);
                            fclose(log);

                            //chiamo la funzione per giocare le istantanee
                            controllaIstantaneaOstacolo(listaGiocatore, listaCartaScarto, listaOstacolo, ostacoloTerra);
                        }
                    }

                    //se la carta non c'e' o non la si vuole giocare
                    if(!istantaneaTrovata || sceltaGioco == 'n') {

                        //viene assegnata l'ostacolo al perdente
                        pescaOstacolo(ostacoloTerra, &auxGiocatore->manoOstacolo, 1);
                    }

                }
                cont++;
                auxGiocatore = auxGiocatore->nextGiocatore;
            }
        }
    }
    printf("\n");

    //libero la memoria allocata dinamicamente dai due array
    free(arrayIndexMin);
    free(arrayIndexMax);
}

/**
 * calcola i punti considerando che per ogni carta ostacolo si aggiunge un punto ai cfu del giocatore
 *
 * @param listaGiocatore    puntatore al puntatore della testa della lista dei giocatori
 */
void calcoloPuntiOstacolo(Giocatore **listaGiocatore){

    Giocatore *auxGiocatore = NULL;
    Ostacolo *auxManoOstacolo = NULL;
    int contaOstacolo = 0;

    //scorro la lista e aggiungo un punto per ogni carta ostacolo
    auxGiocatore = *listaGiocatore;
    while (auxGiocatore != NULL){

        //conto quanti ostacoli possiede il giocatore
        auxManoOstacolo = auxGiocatore->manoOstacolo;
        contaOstacolo = contatoreListaOstacolo(auxManoOstacolo);

        //sommo i punti
        auxGiocatore->cfuAccumulati += contaOstacolo;

        auxGiocatore = auxGiocatore->nextGiocatore;
    }

}