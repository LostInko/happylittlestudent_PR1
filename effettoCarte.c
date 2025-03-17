//
// Created by matte on 25/02/2024.
//

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "caricamento.h"
#include "utility.h"
#include "stampeVarie.h"
#include "effettoCarte.h"
#include "ANSI-color-codes.h"

/**
 * questa funzione restituisce l'effetto di una carta giocata durante il turno
 *
 * @param effettoInGioco il tipo di effetto della carta giocata
 * @param listaGiocatore puntatore alla testa della lista dei giocatori
 * @param listaCarta puntatore al giocatore attuale
 * @param giocatoreAttuale puntatore al giocatore attuale
 * @param listaCartaInGioco puntatore alla testa della lista delle carte in gioco
 * @param listaCartaScarto puntatore alla testa della lista delle carte scartate
 * @param indexGiocatoreAttuale indice del giocatore attuale
 * @param arrayPunteggi array dei punteggi dei giocatori
 * @param numGiocatori il numero dei giocatori
 * @param arrayEffetti array degli effetti delle carte
 * @param arrayPuntiEffetto array dei punti degli effetti
 * @return true se l'effetto raddoppia gli effetti
 */
bool giocaCartaEffetto(int effettoInGioco, Giocatore **listaGiocatore,Carta **listaCarta, Giocatore **giocatoreAttuale, Carta **listaCartaInGioco, Carta **listaCartaScarto, int indexGiocatoreAttuale, int *arrayPunteggi, int numGiocatori, int *arrayEffetti, int *arrayPuntiEffetto){

    bool raddoppio = false;
    int valoreCarta = 0;
    Giocatore *auxGiocatore = *listaGiocatore;
    Giocatore *auxAttuale = *giocatoreAttuale;

    stampaEffettoCartaAttivazione(effettoInGioco);

    switch (effettoInGioco) {

        case NESSUNO:
            printf("Carta senza effetto\n");
            break;

        case SCARTAP:
            valoreCarta = effettoCartaSCARTAP(giocatoreAttuale, listaCartaScarto);
            break;

        case RUBA:
            effettoCartaRUBA(&auxGiocatore, &auxAttuale, indexGiocatoreAttuale);
            break;

        case SCAMBIADS:
            effettoCartaSCAMBIADS(&auxGiocatore, listaCartaInGioco, indexGiocatoreAttuale, arrayPunteggi);
            break;

        case SCARTAE:
            valoreCarta = effettoCartaSCARTAE(giocatoreAttuale,listaCartaScarto);
            break;

        case SCARTAC:
            effettoCartaSCARTAC(&auxAttuale, listaCartaScarto);
            break;

        case SCAMBIAP:
            effettoCartaSCAMBIAP(arrayPunteggi, numGiocatori);
            break;

        case DOPPIOE:
            raddoppio = true;
            printf("---> Raddoppia gli effetti delle carte che aumentano o diminuiscono il punteggio (per tutti).\n");
            break;

        case SBIRCIA:
            effettoCartaSBIRCIA(listaCarta, &auxAttuale, listaCartaScarto);
            break;

        case SCAMBIAC:
            effettoCartaSCAMBIAC(listaGiocatore, listaCartaInGioco, arrayPunteggi);
            break;

        case ANNULLA:
            effettoCartaANNULLA(numGiocatori , arrayEffetti, *listaGiocatore);
            break;

        default:
            printf("Errore!");
    }

    //aggiorno l'array dei punti dell'effetto del giocatore
    arrayPuntiEffetto[indexGiocatoreAttuale] += valoreCarta;
    return raddoppio;
}

/**
 * esegue l'effetto istantaneo relativo al punteggio dei giocatori
 *
 * @param effettoInGioco l'effetto istantaneo relativo al punteggio
 * @param listaGiocatore  puntatore alla testa della lista dei giocatori
 * @param arrayPunteggi array dei punteggi dei giocatori
 * @param numGiocatori numero totale dei giocatori
 * @param arrayPuntiEffetto array dei punti effetto
 */
void giocaIstantaneaPunteggio(int effettoInGioco, Giocatore **listaGiocatore, int *arrayPunteggi, int numGiocatori, int *arrayPuntiEffetto){
    stampaEffettoCartaAttivazione(effettoInGioco);

    switch (effettoInGioco) {


        case AUMENTA:
            istantaneoCartaAUMENTA( arrayPuntiEffetto, listaGiocatore);
            break;

        case DIMINUISCI:
            istantaneoCartaDIMINUISCI( arrayPuntiEffetto, listaGiocatore);
            break;

        case INVERTI:
            istantaneoCartaINVERTI(arrayPunteggi, numGiocatori);
            break;

        default:
            printf("Errore!");
    }

}

/**
 * esegue l'effetto istantaneo di un ostacolo in gioco
 *
 * @param effettoInGioco l'effetto istantaneo relativo al punteggio
 * @param listaGiocatore  puntatore alla testa della lista dei giocatori
 * @param listaOstacolo puntatore alla testa della lista degli ostacoli
 * @param ostacoloTerra puntatore all'ostacolo in "terra"
 * @param listaCartaScarto puntatore alla testa della lista delle carte scartate
 */
void giocaIstantaneaOstacolo(int effettoInGioco, Giocatore **listaGiocatore, Ostacolo **listaOstacolo, Ostacolo **ostacoloTerra, Carta **listaCartaScarto){

    stampaEffettoCartaAttivazione(effettoInGioco);

    switch (effettoInGioco) {

        case SALVA:
            istantaneoCartaSALVA(ostacoloTerra, listaOstacolo);
            break;

        case DIROTTA:
            istantaneoCartaDIROTTA(ostacoloTerra, listaGiocatore, listaCartaScarto, listaOstacolo);
            break;

        default:
            printf("Errore!");
    }

}

/**
 * scarta una carta punto e aggiungi il suo punteggio a quello del turno
 *
 * @param listaGiocatore  puntatore alla testa della lista dei giocatori
 * @param listaCartaScarto puntatore alla testa della lista delle carte scartate
 * @return valore della carta scartata
 */
int effettoCartaSCARTAP(Giocatore **listaGiocatore, Carta **listaCartaScarto){

    bool cartaPunto = false;
    bool cartaTrovata = false;
    int valoreCarta = 0, sceltaCarta;
    int contaCartaMano = 0;
    Giocatore *auxGiocatore = *listaGiocatore;
    Carta *manoCarta = auxGiocatore->manoCarta;
    int contatore = 0;

    printf("---> Scarta una carta CFU punto e aggiungi il suo punteggio a quello del turno.\n");
    stampaCarte(auxGiocatore);
    auxGiocatore = *listaGiocatore;

    //verifica se il giocatore ha almeno una carta punto nella mano
    manoCarta = auxGiocatore->manoCarta;
    while (manoCarta!= NULL){
        if(manoCarta->effettoCarta == NESSUNO){
            cartaPunto = true;
        }
        manoCarta = manoCarta->nextCarta;
    }

    //se ne ha almeno una
    if(cartaPunto) {
        //richiamo la funzione sceltaCartaInGioco per scegliere la carta
        manoCarta = auxGiocatore->manoCarta;
        contaCartaMano = contatoreListaCarta(manoCarta);
        sceltaCarta = sceltaCartaInGioco(contaCartaMano);

        //cerco la carta desiderata nella mano del giocatore
        contatore = 0;
        manoCarta = auxGiocatore->manoCarta;
        while (manoCarta != NULL && !cartaTrovata) {
            if (contatore == sceltaCarta) {
                //mi salvo il valore della carta
                valoreCarta = manoCarta->valore;

                //imposto il booleano per far terminare il while
                cartaTrovata = true;
            }

            //in caso non venga trovato continuo a far scorrere il while andando al giocatore successivo
            manoCarta = manoCarta->nextCarta;
            contatore++;
        }

        //una volta confermata la carta viene pescata quella effettiva e scartata
        auxGiocatore = *listaGiocatore;
        pescaCartaScelta(auxGiocatore, sceltaCarta, listaCartaScarto);
    } else {

        //se non ha carte punto viene mandato un messaggio d'avviso
        printf("------- Non possiedi carte punto! -------\n");
    }

    return valoreCarta;
}

/**
 * guarda la mano di un collega e ruba una carta a scelta
 *
 * @param listaGiocatore  puntatore alla testa della lista dei giocatori
 * @param giocatoreAttuale puntatore al giocatore attuale
 * @param indexGiocatoreAttuale indice del giocatore attuale
 */
void effettoCartaRUBA(Giocatore **listaGiocatore, Giocatore **giocatoreAttuale,int indexGiocatoreAttuale){

   int indexGiocatore;
   Giocatore *auxGiocatore = *listaGiocatore;
   Carta *cartaAttuale = auxGiocatore->manoCarta;
   int contScorrimento = 0, cont = 0;
   int sceltaCarta, contaCartaMano;

    printf("---> Guarda la mano di un collega e ruba una carta a scelta.\n");
    printf("\n----- ?? Quale giocatore si vuole spiare ?? -----\n");

    //stampo i nomi dei giocatori escludendo quello attuale
    auxGiocatore = *listaGiocatore;
    while (auxGiocatore != NULL){

        if(indexGiocatoreAttuale != cont) {
            printf("Giocatore %d --> [%s]\n", cont, auxGiocatore->nomeUtente);

            cont++;
        }
        auxGiocatore = auxGiocatore->nextGiocatore;
    }

    //richiamo la funzione e scelgo il giocatore escluso quello scelto
    auxGiocatore = *listaGiocatore;
    indexGiocatore = cercaNomeEscluso(auxGiocatore, *giocatoreAttuale);

    //mostro le carte nella mano del giocatore scelto
    cont = 0;
    auxGiocatore = *listaGiocatore;
    while (auxGiocatore != NULL){

        if(contScorrimento == indexGiocatore){
            cartaAttuale = auxGiocatore->manoCarta;
            printf("\n--------- Carte nella mano di %s ---------\n", auxGiocatore->nomeUtente);
            while (cartaAttuale != NULL) {

                printf("[%d] ", cont + 1);
                printf("%-20s", cartaAttuale->nomeCarta);
                printf("%5d\t", cartaAttuale->valore);
                stampaEffettoCarta(cartaAttuale->effettoCarta);
                cont++;

                cartaAttuale = cartaAttuale->nextCarta;
            }

            printf("\n----- ?? Quale carta si vuole rubare ?? -----\n");
            //scelgo la carta da rubare
            contaCartaMano = contatoreListaCarta(auxGiocatore->manoCarta);
            sceltaCarta = sceltaCartaGenerale(contaCartaMano);

            //ruba la carta scelta e la aggiungo alla mano del giocatore
            pescaCartaScelta(auxGiocatore, sceltaCarta, &(*giocatoreAttuale)->manoCarta);
        }

        contScorrimento++;
        auxGiocatore = auxGiocatore->nextGiocatore;
    }
}

/**
 * scambia questa carta con quella di un altro giocatore purchè senza effetto
 *
 * @param listaGiocatore  puntatore alla testa della lista dei giocatori
 * @param listaCartaInGioco puntatore alla testa della lista delle carte in gioco
 * @param indexGiocatoreAttuale indice del giocatore attuale
 * @param arrayPunteggi array dei punteggi dei giocatori
 */
void effettoCartaSCAMBIADS(Giocatore **listaGiocatore,Carta **listaCartaInGioco, int indexGiocatoreAttuale, int *arrayPunteggi){
    Carta *auxCartaInGioco = *listaCartaInGioco;
    Giocatore *auxGiocatore = *listaGiocatore;
    int sceltaCarta;
    int contScorrimento, contaCarteInGioco, cont;
    int numGiocatori = contatoreListaGiocatore(*listaGiocatore);
    int aux;
    int contaSenzaEffetto = 0;

    auxGiocatore = *listaGiocatore;
    contScorrimento = 0;
    cont = 1;
    Carta *cartaInGioco = *listaCartaInGioco;

    printf("---> Scambia questa carta con quella di un altro giocatore, purche' senza effetto\n");

    //stampo le carte di tutti tranne che di quello che gioca
    printf("\n--------- Carte in gioco ---------\n");
    while (contScorrimento < numGiocatori) {

        if(indexGiocatoreAttuale != contScorrimento) {
            printf("[%d] %s ",cont, auxGiocatore->nomeUtente);
            printf("%-20s", cartaInGioco->nomeCarta);
            printf("%5d\t", cartaInGioco->valore);
            stampaEffettoCarta(cartaInGioco->effettoCarta);

            if(cartaInGioco->effettoCarta == NESSUNO){
                contaSenzaEffetto++;
            }

            cont++;
        }

        contScorrimento++;
        cartaInGioco = cartaInGioco->nextCarta;
        auxGiocatore = auxGiocatore->nextGiocatore;
    }

    //se ci sono carte senza effetto permetto lo scambio
    if( contaSenzaEffetto > 0){

        cartaInGioco = *listaCartaInGioco;
        contaCarteInGioco = contatoreListaCarta(cartaInGioco);
        printf("\n----- ?? Con quale carta si vuole scambiare ?? -----\n");

        sceltaCarta = sceltaCartaGenerale(contaCarteInGioco);

        aux = arrayPunteggi[indexGiocatoreAttuale];
        arrayPunteggi[indexGiocatoreAttuale] = arrayPunteggi[sceltaCarta];
        arrayPunteggi[sceltaCarta] = aux;
    } else{
        printf("Non puoi scambiare con nessuna carta perche' sono tutte effetto!\n");
    }

}

/**
 * scarta una carta punto con effetto e aggiunge il suo punteggio a quello del turno
 *
 * @param listaGiocatore  puntatore alla testa della lista dei giocatori
 * @param listaCartaScarto puntatore alla testa della lista delle carte scartate
 * @return il punteggio della carta scartata
 */
int effettoCartaSCARTAE(Giocatore **listaGiocatore,Carta **listaCartaScarto){

    bool cartaEffetto = false;
    int valoreCarta = 0, sceltaCarta;
    int contaCartaMano = 0;
    Carta *copiaCarta = NULL;
    Carta *manoCarta = NULL;
    Giocatore *auxGiocatore = NULL;

    printf("---> Scarta una carta CFU punto con effetto e aggiungi il suo punteggio a quello del turno\n");
    auxGiocatore = *listaGiocatore;
    stampaCarte(auxGiocatore);

    //controlla se il giocatore possiede carte effetto nella mano
    manoCarta = auxGiocatore->manoCarta;
    while (manoCarta!= NULL){
        if(manoCarta->effettoCarta > NESSUNO || manoCarta->effettoCarta < ANNULLA ){
            cartaEffetto = true;
        }
        manoCarta = manoCarta->nextCarta;
    }

    //se il giocatore ha almeno una carta effetto procede con lo scarto
    if(cartaEffetto) {

        auxGiocatore = *listaGiocatore;
        do {
            //contro le carte della mano
            contaCartaMano = contatoreListaCarta(auxGiocatore->manoCarta);
            auxGiocatore = *listaGiocatore;

            //richiamo la funzione sceltaCartaInGioco per scegliere la carta
            sceltaCarta = sceltaCartaEffettoInGioco(contaCartaMano);

            //richiamo la funzione che mi permette di copiare la carta scelta per verificare se e' una carta effetto
            copiaCartaScelta(auxGiocatore, sceltaCarta, &copiaCarta);
            if (copiaCarta->effettoCarta == NESSUNO || copiaCarta->effettoCarta > ANNULLA) {
                printf("Errore! La carta \"%s\" non e' una carta effetto! Pesca ancora!\n", (*copiaCarta).nomeCarta);
            } else {
                valoreCarta = copiaCarta->valore;
            }
        } while (copiaCarta->effettoCarta == NESSUNO || copiaCarta->effettoCarta > ANNULLA);

        //una volta confermata la carta viene pescata quella effettiva
        auxGiocatore = *listaGiocatore;
        pescaCartaScelta(auxGiocatore, sceltaCarta, listaCartaScarto);

    }else {
        printf("------- Non possiedi carte effetto! -------\n");
    }
    return valoreCarta;
}

/**
 * scarta da uno a tre carte dalla mano del giocatore
 *
 * @param listaGiocatore  puntatore alla testa della lista dei giocatori
 * @param listaCartaScarto puntatore alla testa della lista delle carte scartate
 */
void effettoCartaSCARTAC(Giocatore **listaGiocatore, Carta **listaCartaScarto){

    Giocatore *auxGiocatore = *listaGiocatore;
    Carta *auxManoCarta = auxGiocatore->manoCarta;
    int sceltaCarta, contaCartaMano;
    int numScarto;

    printf("---> Scarta da uno a tre carte dalla tua mano\n");

    auxGiocatore = *listaGiocatore;
    stampaCarte(auxGiocatore);

    //controlla quante carte vuole scartare il giocatore
    do {
        printf("Quante carte vuoi scartare?\n");
        scanf("%d", &numScarto);

        if(numScarto <= 0 || numScarto > 3)
            printf("Errore! Inserisci una numero tra 0 e 3!\n");

    } while (numScarto <= 0 || numScarto > 3);

    //scarta il numero di carte richieste
    for (int i = 0; i < numScarto; ++i) {
        auxManoCarta = auxGiocatore->manoCarta;
        contaCartaMano = contatoreListaCarta(auxManoCarta);

        auxGiocatore = *listaGiocatore;
        stampaCarte(auxGiocatore);

        printf("\n\n----- ?? Quale carta si vuole scartare ?? -----\n");
        sceltaCarta = sceltaCartaGenerale(contaCartaMano);

        //pesco e scarto la carta selezionata
        auxGiocatore = *listaGiocatore;
        pescaCartaScelta(auxGiocatore, sceltaCarta, listaCartaScarto);
    }
}

/**
 * scambia il punteggio del giocatore con il punteggio massimo con quello del giocatore con punteggio minimo
 *
 * @param arrayPunteggi array dei punteggi dei giocatori
 * @param numGiocatori numero totale di giocatori
 */
void effettoCartaSCAMBIAP(int *arrayPunteggi, int numGiocatori) {
    int aux;
    int maxIndex, minIndex;

    //cerco gli indici max e min dei punteggi
    trovaMax(numGiocatori, arrayPunteggi, &maxIndex);
    trovaMin(numGiocatori, arrayPunteggi, &minIndex);

    printf("---> Scambiato il punteggio del giocatore con punteggio massimo e minimo.\n");

    //scambia i punteggi max e min dell'array
    aux = arrayPunteggi[maxIndex];
    arrayPunteggi[maxIndex] = arrayPunteggi[minIndex];
    arrayPunteggi[minIndex] = aux;

}

/**
 * guarda due carte in cima al mazzo, ne prende una scarta l'altra
 *
 * @param listaCarta puntatore alla testa della lista delle carte
 * @param listaGiocatore  puntatore alla testa della lista dei giocatori
 * @param listaCartaScarto puntatore alla testa della lista delle carte scartate
 */
void effettoCartaSBIRCIA(Carta **listaCarta, Giocatore **listaGiocatore, Carta **listaCartaScarto){

    Carta *auxListaCarta = *listaCarta;
    Giocatore *auxGiocatore = *listaGiocatore;
    int sceltaCarta;
    int cont = 0;

    printf("---> Guarda due carte in cima al mazzo, prendine una e scarta l'altra.\n");

    //stampo le prime due carte del mazzo
    printf("\n--------- Prime due carte del mazzo ---------\n");
    while (auxListaCarta != NULL && cont < 2) {

        printf("[%d] ", cont + 1);
        printf("%-20s", auxListaCarta->nomeCarta);
        printf("%5d\t", auxListaCarta->valore);
        stampaEffettoCarta(auxListaCarta->effettoCarta);
        cont++;

        auxListaCarta = auxListaCarta->nextCarta;
    }

    /*auxListaCarta = *listaCarta;*//*
    stampaCarteGiocatore(auxListaCarta);*/

    //richiedo quale carte pescare
    printf("\n----- ?? Quale carta si vuole pescare ?? -----\n");
    printf("\n------  [L'altra carta verra' scartata]  ------\n");

    sceltaCarta = sceltaCartaGenerale(2);

    //pesco e scarta la carta in base all'ordine richiesto
    if(sceltaCarta == 0){
        pescaCarta(listaCarta, &auxGiocatore->manoCarta, 1, listaCartaScarto);
        pescaCarta(listaCarta, listaCartaScarto, 1, listaCartaScarto);
    }else{
        pescaCarta(listaCarta, listaCartaScarto, 1, listaCartaScarto);
        pescaCarta(listaCarta, &auxGiocatore->manoCarta, 1, listaCartaScarto);
    }

    /*auxListaCarta = *listaCarta;
    stampaCarteGiocatore(auxListaCarta);*/

}

/**
 * scambia la carta punto giocata nel turno da un giocatore g1 con quella di un giocatore g2
 *
 * @param listaGiocatore  puntatore alla testa della lista dei giocatori
 * @param listaCartaInGioco puntatore alla testa della lista delle carte da gioco
 * @param arrayPunteggi array contenente i punteggi dei giocatori
 */
void effettoCartaSCAMBIAC(Giocatore **listaGiocatore,Carta **listaCartaInGioco, int *arrayPunteggi){

    Carta *auxCartaInGioco = *listaCartaInGioco;
    Giocatore *auxGiocatore = *listaGiocatore;
    int sceltaCartaG1, sceltaCartaG2;
    int contScorrimento, contaCarteInGioco, cont;
    int numGiocatori = contatoreListaGiocatore(*listaGiocatore);
    int aux;
    auxGiocatore = *listaGiocatore;
    contScorrimento = 0;
    Carta *cartaInGioco = *listaCartaInGioco;

    printf("---> Scambia la carta punto giocata nel turno da un giocatore G1 con quella di un giocatore G2\n");

    //stampa le carte da gioco
    printf("\n--------- Carte in gioco ---------\n");
    while (contScorrimento < numGiocatori) {

        printf("[%d]%s ",contScorrimento+1, auxGiocatore->nomeUtente);
        printf("%-20s", cartaInGioco->nomeCarta);
        printf("%5d\t", cartaInGioco->valore);
        stampaEffettoCarta(cartaInGioco->effettoCarta);

        contScorrimento++;
        cartaInGioco = cartaInGioco->nextCarta;
        auxGiocatore = auxGiocatore->nextGiocatore;
    }

    //conto le carte in gioco e scelgo il G1
    contaCarteInGioco = contatoreListaCarta(*listaCartaInGioco);
    printf("\n----- ?? Scegli la prima carta da scambiare ?? -----\n");
    sceltaCartaG1 = sceltaCartaGenerale(contaCarteInGioco);

    //conto le carte in gioco e scelgo il G2
    contaCarteInGioco = contatoreListaCarta(*listaCartaInGioco);
    printf("----- ?? Scegli la seconda carta da scambiare ?? -----\n");
    sceltaCartaG2 = sceltaCartaGenerale(contaCarteInGioco);

    //scambia i punteggi delle carte scelte
    aux = arrayPunteggi[sceltaCartaG1];
    arrayPunteggi[sceltaCartaG1] = arrayPunteggi[sceltaCartaG2];
    arrayPunteggi[sceltaCartaG2] = aux;
}

/**
 * annulla gli effetti di tutte le carte punto durante il turno
 *
 * @param listaGiocatore  puntatore alla testa della lista dei giocatori
 * @param numGiocatori numero totale dei giocatori
 * @param arrayEffetti array contenente gli effetti delle carte punto per ciascun giocatore
 */
void effettoCartaANNULLA( int numGiocatori, int *arrayEffetti, Giocatore *listaGiocatore){

    int cont = 0;
    printf("---> Annulla gli effetti di tutte le carte punto durante il turno\n");

    //imposto tutti gli effetti in modo che non vengano attivati
    for (int i = 0; i < numGiocatori; ++i) {
        arrayEffetti[i] = -1;
    }

    //stampo l'avviso di annullamento
    Giocatore *auxGiocatore = listaGiocatore;
    while (auxGiocatore != NULL ) {

        if(arrayEffetti[cont] == -1){
            printf(BLU "------------ L'effetto della carta di %s e' stato annullato ------------\n\n" RESET, auxGiocatore->nomeUtente);
        }
        cont++;
        auxGiocatore = auxGiocatore->nextGiocatore;

    }
}

/**
 * aumenta di 2 cfu il punteggio del turno di un giocatore a scelta
 *
 * @param arrayPuntiEffetto array contenente i punteggi effetto dei giocatori
 * @param listaGiocatore  puntatore alla testa della lista dei giocatori
 */
void istantaneoCartaAUMENTA(int *arrayPuntiEffetto, Giocatore **listaGiocatore){
    int indexGiocatore;
    int cont = 1;
    Giocatore *auxGiocatore = *listaGiocatore;

    printf("  ---> Aumenta di 2 CFU il punteggio del turno di un giocatore a scelta\n");

    printf("\n----- ?? A quale giocatore del turno si vuole aumentare il punteggio ?? -----\n");

    //stampa i nomi dei giocatori e assegna un indice a ciascuno
    while (auxGiocatore != NULL){
        printf("Giocatore %d --> [%s]\n", cont, auxGiocatore->nomeUtente);

        cont++;
        auxGiocatore = auxGiocatore->nextGiocatore;
    }

    //richiedo all'utente di scegliere quale carta a cui aumentare il punteggio
    auxGiocatore = *listaGiocatore;
    indexGiocatore = cercaNome(auxGiocatore);

    //aumenta il punteggio del giocatore scelto
    arrayPuntiEffetto[indexGiocatore] += 2;
}

/**
 * diminuisce di 2 cfu il punteggio del turno di un giocatore a scelta
 *
 * @param arrayPuntiEffetto array contenente i punteggi effetto dei giocatori
 * @param listaGiocatore  puntatore alla testa della lista dei giocatori
 */
void istantaneoCartaDIMINUISCI(int *arrayPuntiEffetto, Giocatore **listaGiocatore){
    int indexGiocatore;
    int cont = 1;
    Giocatore *auxGiocatore = NULL;

    printf("  ---> Diminuisci di 2 CFU il punteggio del turno di un giocatore a scelta\n");

    printf("\n----- ?? A quale giocatore del turno si vuole diminuire il punteggio ?? -----\n");

    //stampa i nomi dei giocatori e assegna un indice a ciascuno
    auxGiocatore = *listaGiocatore;
    while (auxGiocatore != NULL){
        printf("Giocatore %d --> [%s]\n", cont, auxGiocatore->nomeUtente);

        cont++;
        auxGiocatore = auxGiocatore->nextGiocatore;
    }

    //richiede all'utente di scegliere il giocatore a cui diminuire il punteggio
    auxGiocatore = *listaGiocatore;
    indexGiocatore = cercaNome(auxGiocatore);

    //diminuisce il punteggio del giocatore scelto
    arrayPuntiEffetto[indexGiocatore] -= 2;
}

/**
 * inverte i punteggi del giocatore con il punteggio massimo e del giocatore con il punteggio minimo
 *
 * @param arrayPunteggi
 * @param numGiocatori
 */
void istantaneoCartaINVERTI(int *arrayPunteggi, int numGiocatori) {
    int aux;
    int maxIndex, minIndex;

    //cerco gli indici dei massimi e dei minimi
    trovaMax(numGiocatori, arrayPunteggi, &maxIndex);
    trovaMin(numGiocatori, arrayPunteggi, &minIndex);

    printf("  ---> Scambiato il punteggio del giocatore con punteggio massimo e minimo.\n");

    //scambia i punteggi massimo e minimo nell'array
    aux = arrayPunteggi[maxIndex];
    arrayPunteggi[maxIndex] = arrayPunteggi[minIndex];
    arrayPunteggi[minIndex] = aux;

}

/**
 * consente al giocatore di mettere la carta ostacolo che sta per prendere
 * in fondo al mazzo
 *
 * @param ostacoloTerra puntatore al puntatore all'ostacolo in gioco
 * @param listaOstacolo puntatore al puntatore alla testa della lista degli ostacoli
 */
void istantaneoCartaSALVA(Ostacolo **ostacoloTerra, Ostacolo **listaOstacolo) {

    Ostacolo *auxOstacoloTerra = *ostacoloTerra;

    printf("  ---> Metti la carta Ostacolo che stai per prendere in fondo al mazzo.\n");

    //sposta l'ostacolo in fondo al mazzo
    spostaOstacoloInCoda(&auxOstacoloTerra, listaOstacolo);
}

/**
 * consente al giocatore di dirottare un ostacolo ad un altro giocatore, diventa ricorsiva perche'
 * viene data la possibilita al giocatore che la riceve di giocare una carta salva o dirotta
 *
 * @param ostacoloTerra puntatore al puntatore all'ostacolo in gioco
 * @param listaGiocatore  puntatore alla testa della lista dei giocatori
 * @param listaCartaScarto puntatore alla testa della lista delle carte scartate
 * @param listaOstacolo puntatore alla testa della lista degli ostacoli
 */
void istantaneoCartaDIROTTA(Ostacolo **ostacoloTerra, Giocatore **listaGiocatore, Carta **listaCartaScarto, Ostacolo **listaOstacolo) {

    Giocatore *auxGiocatore = *listaGiocatore;
    FILE *log = NULL;
    int indexGiocatore;
    char sceltaGioco;
    int contScorrimento = 0;
    bool istantaneaTrovata = false;
    Carta *manoCarta = NULL;

    printf("  ---> Dai la carta che stai per prendere ad un altro giocatore a tua scelta\n");

    printf("\n\n----- ?? A quale giocatore si vuole dirottare la carta ostacolo ?? -----\n");
    indexGiocatore = cercaNome(auxGiocatore);

    //scorre i giocatori fino a quello scelto a cui dirottare la carta
    while (contScorrimento < indexGiocatore && auxGiocatore != NULL) {

        auxGiocatore = auxGiocatore->nextGiocatore;
        contScorrimento++;
    }

    //cerco nella sua mano se puo giocare una carta salva o dirotta
    manoCarta = auxGiocatore->manoCarta;
    while (manoCarta != NULL && !istantaneaTrovata) {

        if (manoCarta->effettoCarta == SALVA || manoCarta->effettoCarta == DIROTTA) {

            istantaneaTrovata = true;
        }
        manoCarta = manoCarta->nextCarta;
    }

    //se la trova viene chiesto se la si vuole giocare
    if (istantaneaTrovata){

        stampaCarte(auxGiocatore);
        printf("-------- ?? Vuoi giocare una carta salva o dirotta ?? --------\n");
        sceltaGioco = sceltaSN();

        //se la vuole giocare richiamo la funzione che le fa giocare
        if (sceltaGioco == 's') {
            controllaIstantaneaOstacolo(listaGiocatore, listaCartaScarto, listaOstacolo, ostacoloTerra);
        }
    }

    //se non possiede la carta o non vuole giocarla prende la carta ostacolo
    if(!istantaneaTrovata || sceltaGioco == 'n') {

        log = fopen("log.txt", "a");
        if(log == NULL) exit(-5);
        fprintf(log, "Il giocatore %s ha perso il round e ha pescato %s\n", auxGiocatore->nomeUtente, (*ostacoloTerra)->nomeOstacolo);
        fclose(log);

        //viene assegnata l'ostacolo al perdente
        pescaOstacolo(ostacoloTerra, &auxGiocatore->manoOstacolo, 1);

    }

}

/**
 * controlla se ci sono giocatori che possiedono carte istantanee e gestisce l'opzione di giocarle
 *
 * @param listaGiocatore  puntatore alla testa della lista dei giocatori
 * @param listaCartaScarto puntatore alla testa della lista delle carte scartate
 * @param arrayPunteggi array dei punteggi dei giocatori
 * @param numGiocatori numero dei giocatori
 * @param arrayPuntiEffetto array dei punti effetto
 * @return true se almeno un giocatore ha carte istantanee giocabili
 */
bool controlloIstantanee(Giocatore **listaGiocatore, Carta **listaCartaScarto, int *arrayPunteggi, int numGiocatori, int *arrayPuntiEffetto) {
    Giocatore *auxGiocatore = NULL;
    Carta *manoCarta = NULL;
    bool istantaneaTrovata = false;
    char sceltaGioco;

    //scorro i giocatori per verificare se qualcuno possiede carte istantanee
    auxGiocatore = *listaGiocatore;
    while (auxGiocatore != NULL  && !istantaneaTrovata) {

        manoCarta = auxGiocatore->manoCarta;
        while (manoCarta != NULL && !istantaneaTrovata) {

            if (manoCarta->effettoCarta >= AUMENTA && manoCarta->effettoCarta <= INVERTI) {

                istantaneaTrovata = true;
            }
            manoCarta = manoCarta->nextCarta;
        }
        auxGiocatore = auxGiocatore->nextGiocatore;
    }

    //se nessun giocatore ha carte istantanee stampa un messaggio di avviso
    if (!istantaneaTrovata) {
        printf("\n--------- !! Nessun giocatore ha carte istantanee !! --------- \n");
    } else {

        //se qualcuno ha carte istantanee chiedo se qualcuno le vuole giocare
        printf("-------- ?? Si vogliono giocare carte Istantanee ?? --------\n");
        sceltaGioco = sceltaSN();

        //se risponde di si chiamo la funzione adeguata
        if (sceltaGioco == 's') {
            giocaInstantanee(listaGiocatore, listaCartaScarto, arrayPunteggi, numGiocatori, arrayPuntiEffetto);
        }else if (sceltaGioco == 'n') {

            //se no imposto il booleano a falso
            istantaneaTrovata = false;
        }
    }

    return istantaneaTrovata;
}

/**
 * gestisce il turno in cui i giocatori possono giocare carte istantanee
 *
 * @param listaGiocatore  puntatore alla testa della lista dei giocatori
 * @param listaCartaScarto puntatore alla testa della lista delle carte scartate
 * @param arrayPunteggi array dei punteggi dei giocatori
 * @param numGiocatori numero di giocatori
 * @param arrayPuntiEffetto array dei punti effetto
 */
void giocaInstantanee(Giocatore **listaGiocatore, Carta **listaCartaScarto, int *arrayPunteggi, int numGiocatori, int *arrayPuntiEffetto){

    Carta *copiaCarta = NULL;
    Giocatore *auxGiocatore = NULL;
    Carta *manoCarta = NULL;
    bool giocatoreTrovato = false;
    bool istantaneaTrovata = false;
    int contScorrimento = 1;
    int cont = 1;
    int indexScelto, cartaScelta;
    int effettoCarta;

    printf("\n-------- ?? Chi vuole giocare carte Istantanee ?? --------\n");
    auxGiocatore = *listaGiocatore;
    contScorrimento = 1;

    //stampo solo i giocatori che possono giocare carte istantanee
    while (auxGiocatore != NULL) {

        manoCarta = auxGiocatore->manoCarta;
        while (manoCarta != NULL) {
            if (manoCarta->effettoCarta >= AUMENTA && manoCarta->effettoCarta <= INVERTI) {
                istantaneaTrovata = true; // Imposta istantaneaTrovata su true quando viene trovata una carta istantanea
            }
            manoCarta = manoCarta->nextCarta;
        }

        if(istantaneaTrovata){

            printf("[TASTO %d] Giocatore %d: %s\n", cont, contScorrimento, auxGiocatore->nomeUtente);
            cont++;
        }
        istantaneaTrovata = false;
        auxGiocatore = auxGiocatore->nextGiocatore;
        contScorrimento++; // Incrementa il contatore per il prossimo giocatore
    }

    //richiede al giocatore di selezionare un giocatore che puo' giocare carte istantanee
    contScorrimento = 0;
    auxGiocatore = *listaGiocatore;
    istantaneaTrovata = false;
    do {
        printf("Inserisci il numero -->");
        scanf("%d", &indexScelto);

        indexScelto -= 1;

        while (auxGiocatore != NULL  && !istantaneaTrovata) {

            if(contScorrimento == indexScelto) {

                manoCarta = auxGiocatore->manoCarta;
                while (manoCarta != NULL && !istantaneaTrovata) {

                    if (manoCarta->effettoCarta >= AUMENTA && manoCarta->effettoCarta <= INVERTI) {

                        istantaneaTrovata = true;
                    }
                    manoCarta = manoCarta->nextCarta;
                }
            }
            contScorrimento++;
            auxGiocatore = auxGiocatore->nextGiocatore;
        }


        if(!istantaneaTrovata){

            printf("Errore! Inserire un numero valido\n");
        }
    } while (!istantaneaTrovata);

    //eseguo il turno del giocatore selezionato
    cont = 0;
    contScorrimento = 0;
    auxGiocatore = *listaGiocatore;
    while (auxGiocatore != NULL && !giocatoreTrovato) {

        if(indexScelto == contScorrimento) {

            manoCarta = auxGiocatore->manoCarta;
            cont = contatoreListaCarta(manoCarta);
            stampaCarte(auxGiocatore);

            printf("\n----- ?? Quale carta si vuole giocare ?? -----\n"
                   "[ricordati di scegliere una carta istantanea!!]\n");

            //richiedo al giocatore di selezionare una carta istantanea
            do {
                //richiamo la funzione sceltaCartaGenerale per scegliere la carta
                cartaScelta = sceltaCartaGenerale(cont);

                //richiamo la funzione che mi permette di copiare la carta scelta per verificare se è una carta effetto
                copiaCartaScelta(auxGiocatore, cartaScelta, &copiaCarta);

                if (copiaCarta->effettoCarta < AUMENTA || copiaCarta->effettoCarta > INVERTI) {

                    printf("Errore! La carta \"%s\" non e' istantanea! Scegli ancora!\n", (*copiaCarta).nomeCarta);
                }else{

                    //mi salvo l'effetto
                    effettoCarta = copiaCarta->effettoCarta;
                }

            } while (copiaCarta->effettoCarta < AUMENTA || copiaCarta->effettoCarta > INVERTI);

            //pesco la carta scelta e la scarto
            pescaCartaScelta(auxGiocatore, cartaScelta, listaCartaScarto);

            //gioco l'istantanea tramite l'effetto salvato
            giocaIstantaneaPunteggio(effettoCarta, listaGiocatore, arrayPunteggi, numGiocatori, arrayPuntiEffetto);

            giocatoreTrovato = true;
        }
        contScorrimento++;
        auxGiocatore = auxGiocatore->nextGiocatore;

    }

}

/**
 * controlla e gestisce l'effetto istantaneo di una carta ostacolo
 *
 * @param listaGiocatore  puntatore alla testa della lista dei giocatori
 * @param listaCartaScarto puntatore alla testa della lista delle carte scartate
 * @param listaOstacolo puntatore alla testa della lista degli ostacoli
 * @param ostacoloTerra puntatore all'ostacolo in "terra"
 */
void controllaIstantaneaOstacolo(Giocatore **listaGiocatore, Carta **listaCartaScarto, Ostacolo **listaOstacolo, Ostacolo **ostacoloTerra){

    Carta *copiaCarta = NULL;
    Giocatore *auxGiocatore = NULL;
    Carta *manoCarta = NULL;
    int cont, cartaScelta;
    int effettoCarta;

    cont = 0;
    auxGiocatore = *listaGiocatore;

    //conto il numero di carte e le stampo
    manoCarta = auxGiocatore->manoCarta;
    cont = contatoreListaCarta(manoCarta);
    stampaCarte(auxGiocatore);

    printf("\n----- ?? Quale carta si vuole giocare ?? -----\n"
           "[ricordati di scegliere una carta di tipo SALVA O DIROTTA!!]\n");

    do {
        //richiamo la funzione sceltaCartaInGioco per scegliere la carta
        cartaScelta = sceltaCartaGenerale(cont);

        //richiamo la funzione che mi permette di copiare la carta scelta per verificare se è una carta effetto
        copiaCartaScelta(auxGiocatore, cartaScelta, &copiaCarta);

        //verifica se la carta è tra quelle chieste
        if (copiaCarta->effettoCarta != SALVA && copiaCarta->effettoCarta != DIROTTA) {

            printf("Errore! La carta \"%s\" non e' tra quelle specificate! Scegli ancora!\n", (*copiaCarta).nomeCarta);
        }else{

            effettoCarta = copiaCarta->effettoCarta;
        }

    } while (copiaCarta->effettoCarta != SALVA && copiaCarta->effettoCarta != DIROTTA);

    //pesca la carta scelta dalla mano e la scarta
    pescaCartaScelta(auxGiocatore, cartaScelta, listaCartaScarto);

    //gestisce l'effetto istantaneo della carta ostacolo
    giocaIstantaneaOstacolo(effettoCarta, listaGiocatore, listaOstacolo, ostacoloTerra, listaCartaScarto);
}


