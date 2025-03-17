//
// Created by matte on 16/02/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "caricamento.h"
#include "preparazionegioco.h"
#include "mescolamazzi.h"
#include "utility.h"
#include "ANSI-color-codes.h"

/**
 * pesca una carta scelta dal mazzo del giocatore e la sposta in una lista di tipo Carta,
 * in genere usata per pescare una carta e metterla nella listaCartaInGioco
 *
 * @param listaGiocatore il listaGiocatore che pesca la carta
 * @param cartaScelta l'indice della carta scelta nella mano del giocatore
 * @param listaCartaInGioco puntatore alla lista delle carte in gioco
 *
 * @exit "8" se il giocatore non esiste
 */
void pescaCartaScelta(Giocatore *listaGiocatore, int cartaScelta, Carta **listaCartaInGioco) {
    if (listaGiocatore == NULL) {
        exit(8);
    }

    Carta *manoCarta = listaGiocatore->manoCarta;
    Carta *precedente = NULL;
    bool cartaTrovata = false;
    int contatore = 0;

    //trovo la carta tramite l'indice passato come parametro nell'elenco delle carte in mano
    while (manoCarta != NULL && !cartaTrovata) {
        if (contatore == cartaScelta) {

            //rimuovo la carta dalla mano del giocatore e controlla se la carta è in testa
            if (precedente != NULL) {
                precedente->nextCarta = manoCarta->nextCarta;
            } else {
                listaGiocatore->manoCarta = manoCarta->nextCarta;
            }

            //trovo l'ultimo elemento nella lista delle carte in gioco
            Carta *ultimoCartaInGioco = *listaCartaInGioco;
            if (ultimoCartaInGioco != NULL) {

                while (ultimoCartaInGioco->nextCarta != NULL) {
                    ultimoCartaInGioco = ultimoCartaInGioco->nextCarta;
                }

                //aggiungo la carta alla fine della lista delle carte in gioco
                ultimoCartaInGioco->nextCarta = manoCarta;
                manoCarta->nextCarta = NULL;
            } else {
                //se la lista delle carte in gioco è vuota, la carta diventa la testa
                *listaCartaInGioco = manoCarta;
                manoCarta->nextCarta = NULL;
            }

//            printf("Hai pescato la carta \"%s\"\n", manoCarta->nomeCarta);

            //uno volta trovata la carta, imposta il booleano a vero per interrompere il ciclo
            cartaTrovata = true;
        }

        precedente = manoCarta;
        manoCarta = manoCarta->nextCarta;
        contatore++;
    }
}

/**
 * pesco un dato numero di carte dalla lista listaCartaMazzo e la aggiunge alla mano del giocatore
 *
 * @param listaCartaMazzo puntatore al puntatore alla testa della lista delle carte pescabili (listaCarta)
 * @param cartaMano puntatore al puntatore alla testa della lista delle carte in mano al giocatore
 * @param num numero di carte da pescare e aggiungere alla mano
 * @param listaCartaScarto puntatore al puntatore alla testa della lista delle carte scartate
 */
void pescaCarta(Carta **listaCartaMazzo, Carta **cartaMano, int num, Carta **listaCartaScarto) {
    //ciclo per il numero specificato di carte da pescare
    for (int i = 0; i < num; i++) {
        //controllo che la lista listaCartaMazzo non sia vuota
        if (*listaCartaMazzo == NULL) {

            //se la lista è vuota mescolo il mazzo delle carte scartate e lo utilizzo come il mazzo principale
            printf("Carte esaurite, si procede a mescolare il mazzo \"carte cfu\"\n");
            mescolaMazzoCarta(*listaCartaScarto);

            //imposto il mazzo delle carte pescate come il mazzo delle carte scartate
            listaCartaMazzo = listaCartaScarto;

            //elimino la lista degli scarti
            *listaCartaScarto = eliminaListaCarta(*listaCartaScarto);
        }
        //salvo il puntatore all'elemento da pescare
        Carta *tempCarta = *listaCartaMazzo;

        //sposto il puntatore dalla testa della lista delle carte pescate alla prossima carta
        *listaCartaMazzo = (*listaCartaMazzo)->nextCarta;

        //collego l'elemento corrente alla testa della lista cartaMano
        tempCarta->nextCarta = *cartaMano;

        //aggiorno il puntatore alla testa della lista cartaMano con l'elemento corrente
        *cartaMano = tempCarta;

    }
}

/**
 * pesca un dato numero di ostacoli dalla listaOstacolo e li sposta in una lista in gioco
 *
 * @param listaOstacolo puntatore al puntatore alla testa della lista da cui verranno pescati gli ostacoli
 * @param ostacoloTerra puntatore al puntatore alla testa della lista in gioco a cui verranno spostati gli ostacoli
 * @param num numero di ostacoli da pescare e spostare
 *
 * @exit "8" controllo che listaOstacolo non sia vuota
 */
void pescaOstacolo(Ostacolo **listaOstacolo, Ostacolo **ostacoloTerra, int num) {

    //ciclo per il numero specificato di ostacoli da pescare
    for (int i = 0; i < num; i++) {

        //controllo la validità di listaOstacolo
        if (*listaOstacolo == NULL)   exit(8);

        //salvo il puntatore all'elemento da pescare
        Ostacolo *tempCarta = *listaOstacolo;

        //sposto il puntatore dalla testa della lista degli ostacoli pescati al prossimo ostacolo
        *listaOstacolo = (*listaOstacolo)->nextOstacolo;

        //collego l'elemento corrente alla testa della lista degli ostacoli in gioco
        tempCarta->nextOstacolo = *ostacoloTerra;

        //aggiorno il puntatore alla testa degli ostacoli in gioco con l'elemento corrente
        *ostacoloTerra = tempCarta;
    }
}

/**
 * faccio scegliere al giocatore quale carta giocare dalla sua mano, tenendo conto del numero di carte in suo possesso
 *
 * @param contaCarta numero di carte in mano al giocatore
 * @return l'indice della carta scelta dal giocatore
 */
int sceltaCartaInGioco(int contaCarta){
    int sceltaCarta;

    printf("\n\n----- ?? Quale carta si vuole giocare ?? -----\n[scegli una carta punto!!]\n");

    //chiedo al giocatore di un inserire un numero finché non rispetta il range prescritto (il numero di carte in mano)
    do {
        printf("Inserisci il numero -->");
        scanf("%d",&sceltaCarta);

        if(sceltaCarta <= MIN_CARTA || sceltaCarta > contaCarta)
            printf("Errore! Inserisci una carta tra quelle nella tua mano!\n");

    } while (sceltaCarta <= MIN_CARTA || sceltaCarta > contaCarta);

    //sottraggo 1 perche l'indice dell'array parte da 0 mentre nella stampa la prima carta ha il numero 1
    return sceltaCarta-1;
}

/**
 * copia la carta selezionata dal mazzo del giocatore senza eliminarla dalla mano
 *
 * @param giocatore il giocatore da cui si vuole copiare la carta
 * @param cartaScelta l'indice della carta da copiare dalla mano
 * @param copiaCarta puntatore al puntatore alla lista a cui aggiungere la carta copiata
 *
 * @exit "8" se il giocatore non è valido
 * @exit "10" se non è possibile allocare memoria per la copia della carta
 * @exit "13" se la carta da copiare non esiste
 */
void copiaCartaScelta(Giocatore *giocatore, int cartaScelta, Carta **copiaCarta) {

    if (giocatore == NULL) {
        exit(8);
    }

    //inizializzo i puntatori ausiliari
    Carta *auxCopiaCarta = NULL;
    Carta *manoCarta = giocatore->manoCarta;
    int contatore = 0;
    bool cartaTrovata = false;

    //cerco la carta desiderata nella mano del giocatore
    while (manoCarta != NULL && !cartaTrovata) {
        if (contatore == cartaScelta) {
            //se la carta viene trova si crea una copia della carta
            auxCopiaCarta = (Carta *)malloc(sizeof(Carta));
            if (auxCopiaCarta == NULL)  exit(10);

            //copia di tutti i dati della carta
            strcpy(auxCopiaCarta->nomeCarta, manoCarta->nomeCarta);
            auxCopiaCarta->valore = manoCarta->valore;
            auxCopiaCarta->effettoCarta = manoCarta->effettoCarta;

            //aggiungo la nuova carta alla lista copiaCarta
            auxCopiaCarta->nextCarta = *copiaCarta;
            *copiaCarta = auxCopiaCarta;

            //imposto il booleano per far terminare il while
            cartaTrovata = true;
        }

        //in caso non venga trovato continuo a far scorrere il while andando al giocatore successivo
        manoCarta = manoCarta->nextCarta;
        contatore++;
    }

    //se la carta non viene trovata si esce
    if(copiaCarta == NULL)  exit(13);
}

/**
 * alloca dinamicamente un array di interi di una lunghezza specificata come parametro
 *
 * @param num lunghezza dell'array
 * @return un puntatore all'array allocato
 *
 * @exit "12" se fallisce l'allocazione di memoria
 */
int *allocaArrayInteri(int num) {
    int *array = NULL;

    //alloco la memoria per l'array di interi
    array = (int *) calloc(num, sizeof(int));

    //verifico il successo dell'operazione
    if (array == NULL) exit(12);

    //restituisco il puntatore all'array allocato
    return array;
}

/**
 * trova il minimo valore all'interno di un array e restituisce anche il suo indice
 *
 * @param dim dimensione dell'array
 * @param array vettore su cui effettuare lo studio
 * @param minIndex puntatore all'indice del minimo nell'array
 * @return il minimo valore nell'array
 */
int trovaMin(int dim, const int array[dim], int *minIndex){

    //inizializzo il minimo al valore massimo possibile
    int min = INT_MAX;

    //cerco il min valore nell'array
    for (int i = 0; i < dim; ++i) {
        if(array[i] < min && array[i] != -2){

            //aggiorno le informazioni ogni volta che trovo un valore piu piccolo
            min = array[i];
            *minIndex = i;
        }
    }

    return min;
}

/**
 * trova il massimo valore all'interno di un array e restituisce anche il suo indice
 *
 * @param dim dimensione dell'array
 * @param array vettore su cui effettuare lo studio
 * @param maxIndex puntatore all'indice del massimo nell'array
 * @return il massimo valore nell'array
 */
int trovaMax(int dim, const int array[dim],  int *maxIndex){

    //inizializzo il massimo al valore minimo possibile
    int max = INT_MIN;

    //cerco il max valore nell'array
    for (int i = 0; i < dim; ++i) {
        if(array[i] > max){

            //aggiorno le informazioni ogni volta che trovo un valore piu grande
            max = array[i];
            *maxIndex = i;
        }
    }

    return max;
}

/**
 * sposta il primo ostacolo della lista ostacoli desiderata nella coda della lista listaOstacolo(mazzo)
 *
 * @param ostacoloAux puntatore al puntatore della testa della lista ostacoli da cui prendo l'ostacolo
 * @param listaOstacolo puntatore al puntatore della testa della lista ostacoli (mazzo)
 *
 * @exit "12" se la lista ostacoloAux è vuota
 */
void spostaOstacoloInCoda(Ostacolo **ostacoloAux, Ostacolo **listaOstacolo) {
    if (*ostacoloAux == NULL) {
        exit(12);
    }

    //trovo l'ultimo nodo di listaOstacolo
    Ostacolo *ultimoListaOstacolo = *listaOstacolo;
    if (ultimoListaOstacolo != NULL) {
        while (ultimoListaOstacolo->nextOstacolo != NULL) {
            ultimoListaOstacolo = ultimoListaOstacolo->nextOstacolo;
        }
    }

    //se listaOstacolo è vuota, il primo nodo della lista ostacoloAux diventa il nuovo nodo in coda
    if (ultimoListaOstacolo == NULL) {
        *listaOstacolo = *ostacoloAux;
        *ostacoloAux = (*ostacoloAux)->nextOstacolo;
        (*listaOstacolo)->nextOstacolo = NULL;
    } else {
        //altrimenti collega l'ultimo nodo di listaOstacolo al primo nodo della lista ostacoloAux
        ultimoListaOstacolo->nextOstacolo = *ostacoloAux;
        *ostacoloAux = (*ostacoloAux)->nextOstacolo;
        ultimoListaOstacolo->nextOstacolo->nextOstacolo = NULL;
    }
}

/**
 * ordino l'array in ordine crescente utilizzando il bubble sort
 *
 * @param array l'array da ordinare
 * @param dim dimensione dell'array in questione
 */
void arraySortingCrescente(int array[], int dim) {
    int auxGiocatoreTemp;

    for (int i = 0; i < dim - 1; i++) {
        for (int j = 0; j < dim - i - 1; j++) {
            //confronto gli elementi adiacenti e li scambio se sono fuori posto
            if (array[j] > array[j + 1]) {

                auxGiocatoreTemp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = auxGiocatoreTemp;
            }
        }
    }
}

/**
 * elimina i duplicati dall'array di interi fornito.
 *
 * @param num numero di elementi dell'array
 * @param array vettore da cui eliminare i duplicati
 */
void eliminaDuplicati(int num, int array[num]) {
    int j, duplicato;

    //scorro l'array
    for (int i = 0; i < num; i++) {

        //inizializzo l'indicatore di un duplicato al primo elemento
        duplicato = 0;
        j = 0;

        //verifico se l'elemento corrente è già stato incontrato prima
        while (j < i && !duplicato) {
            if (array[i] == array[j]) {
                //segnalo che l'elemento è un duplicato
                duplicato = 1;
            }

            //scorro il contatore j
            j++;
        }

        //se l'elemento corrente non è un duplicato, lo sposto all'inizio dell'array
        if (duplicato) {
            array[i] = -2;
        }
    }

    //stampa di debug dell'array senza duplicati
//    printf("Array senza duplicati:\n");
//    for (int i = 0; i < num; i++) {
//        printf("%d ", array[i]);
//    }
//    printf("\n");
}

/**
 * Conta il numero di carte nella listaCarta
 *
 * @param listaCarta puntatore alla testa della lista delle carte
 * @return numero totale di carte nella lista
 */
int contatoreListaCarta(Carta *listaCarta){
    int numCarta = 0;

    //scorro la lista e incremento il contatore per ogni carta
    while(listaCarta != NULL) {
        numCarta++;
        listaCarta = listaCarta->nextCarta;
    }

    return numCarta;
}

/**
 * conta il numero di carte nella listaOstacolo
 *
 * @param listaOstacolo puntatore alla testa della lista degli ostacoli
 * @return numero totale di ostacoli nella lista
 */
int contatoreListaOstacolo(Ostacolo *listaOstacolo){
    int numOstacoli = 0;

    //scorro la lista e incremento il contatore per ogni ostacolo
    while(listaOstacolo != NULL) {
        numOstacoli++;
        listaOstacolo = listaOstacolo->nextOstacolo;
    }

    return numOstacoli;
}

/**
 * conta il numero di carte nella listaGiocatore
 *
 * @param listaGiocatore puntatore alla testa della lista dei giocatori
 * @return numero totale di giocatori nella lista
 */
int contatoreListaGiocatore(Giocatore *listaGiocatore){
    int numGiocatori = 0;

    //scorro la lista e incremento il contatore per ogni giocatore
    while(listaGiocatore != NULL) {
        numGiocatori++;
        listaGiocatore = listaGiocatore->nextGiocatore;
    }

    return numGiocatori;
}

/**
 * permette al giocatore di scegliere una carta effetto dalla propria mano
 *
 * @param contaCarta il numero di carte come limite massimo
 * @return l'indice della carta effetto
 */
int sceltaCartaEffettoInGioco(int contaCarta){
    int sceltaCarta;

    printf("\n----- ?? Quale carta effetto si vuole scartare ?? -----\n");

    //chiedo al giocatore di un inserire un numero finché non rispetta il range prescritto (il numero di carte in mano)
    do {
        printf("Inserisci il numero -->");
        scanf("%d",&sceltaCarta);

        if(sceltaCarta <= MIN_CARTA || sceltaCarta > contaCarta)
            printf("Errore! Inserisci una carta tra quelle nella tua mano!\n");

    } while (sceltaCarta <= MIN_CARTA || sceltaCarta > contaCarta);

    //sottraggo 1 perche l'indice dell'array parte da 0 mentre nella stampa la prima carta ha il numero 1
    return sceltaCarta-1;
}

/**
 * cerca il nome di un giocatore nella lista, escludendo il giocatore attuale
 *
 * @param listaGiocatore puntatore alla testa della lista dei giocatori
 * @param giocatoreAttuale puntatore al giocatore attuale
 * @return indice dei giocatori trovato e -1 se non esiste
 */
int cercaNomeEscluso(Giocatore *listaGiocatore, Giocatore *giocatoreAttuale) {
    Giocatore *auxGiocatore = listaGiocatore;
    int indexGiocatore = -1;
    char nomeUtente[DIM_STR];

    bool nomeTrovato = false;
    bool continuaRicercaNome = true;

    while (continuaRicercaNome && auxGiocatore != NULL) {
        printf("Inserisci il nome di un altro giocatore --> ");
        scanf("%s", nomeUtente);

        //controllo se il nome inserito esiste tra i giocatori ma non corrisponde al nome del giocatore attuale
        while (auxGiocatore != NULL && !nomeTrovato) {
            if (strcmp(nomeUtente, auxGiocatore->nomeUtente) == 0 && strcmp(nomeUtente, giocatoreAttuale->nomeUtente) != 0) {
                nomeTrovato = true;
                indexGiocatore++;
            }
            auxGiocatore = auxGiocatore->nextGiocatore;
        }

        if (!nomeTrovato && continuaRicercaNome) {
            printf("Errore! Inserisci un altro nome\n");

            //ripristina auxGiocatore per la ricerca successiva
            auxGiocatore = listaGiocatore;
            //reimposta l'indice
            indexGiocatore = -1;
        } else {
            //imposto la condizione se trovo la condizione di uscita dal loop esterno
            continuaRicercaNome = false;
        }
    }

    return indexGiocatore;
}

/**
 * cerca il nome di un giocatore nella lista dei giocatori e restituisce il suo indice
 *
 * @param listaGiocatore puntatore alla testa della lista dei giocatori
 * @return indice del giocatore nella lista dei giocatori, -1 se non viene trovato
 */
int cercaNome(Giocatore *listaGiocatore) {
    Giocatore *auxGiocatore = listaGiocatore;
    int indexGiocatore = -1;
    char nomeUtente[DIM_STR];
    Giocatore *currentGiocatore = NULL;
    bool nomeTrovato = false;
    bool continuaRicercaNome = true;

    while (continuaRicercaNome && auxGiocatore != NULL) {
        printf("Inserisci il nome di un giocatore --> ");
        scanf("%s", nomeUtente);

        //controllo se il nome inserito esiste tra i giocatori
        currentGiocatore = listaGiocatore;
        while (currentGiocatore != NULL && !nomeTrovato) {
            if (strcmp(nomeUtente, currentGiocatore->nomeUtente) == 0) {
                nomeTrovato = true;
            }
            currentGiocatore = currentGiocatore->nextGiocatore;
        }

        if (!nomeTrovato) {
            printf("Nome inserito non esistente. Inserisci un altro nome.\n");
        } else {
            //imposto la condizione di uscita del loop esterno
            continuaRicercaNome = false;
        }
    }

    //trova l'indice del giocatore
    if (nomeTrovato) {
        indexGiocatore = 0;

        //salvo il currentGiocatore prima di iniziare
        currentGiocatore = listaGiocatore;
        while (currentGiocatore != NULL && strcmp(nomeUtente, currentGiocatore->nomeUtente) != 0) {
            indexGiocatore++;
            currentGiocatore = currentGiocatore->nextGiocatore;
        }
    }

    return indexGiocatore;
}

/**
 * richiede all'utente di inserire il numero corrispondente alla carta che desidera selezionare
 * assicurandosi che il numero inserito sia compreso nell'0'intervallo valido delle carte in mano
 *
 * @param contaCarta
 * @return
 */
int sceltaCartaGenerale(int contaCarta){
    int sceltaCarta;

    //chiedo al giocatore di un inserire un numero finché non rispetta il range prescritto (il numero di carte in mano)
    do {
        printf("Inserisci il numero -->");
        scanf("%d",&sceltaCarta);

        if(sceltaCarta <= MIN_CARTA || sceltaCarta > contaCarta)
            printf("Errore! Inserisci una carta tra quelle nella tua mano!\n");

    } while (sceltaCarta <= MIN_CARTA || sceltaCarta > contaCarta);

    //sottraggo 1 perche l'indice dell'array parte da 0 mentre nella stampa la prima carta ha il numero 1
    return sceltaCarta-1;
}

/**
 * richiede all'utente di inserire una risposta tra s e n
 *
 * @return la scelta dell'utente
 */
char sceltaSN() {
    char scelta;

    do {
        printf("Inserisci la risposta\t[s][n]\n");
        scanf("%c", &scelta);

        //se il carattere è diverso continuo il ciclo
        if(scelta != 's' && scelta != 'n'){

            printf("Errore! Inserire una risposta tra quelle indicate\n");
        }
    } while (scelta != 's' && scelta != 'n');

    return scelta;
}

/**
 * funzione per pulire la console del terminale
 *
 */
void clearConsole() {
    #ifdef _WIN64 // eseguire codice per Windows
        system("cls");
    #elif __APPLE__ // eseguire codice per piattaforme Apple
        system("clear");
    #elif __linux__ // eseguire codice per Linux
            system("clear");
    #endif
}

/**
 * richiede all'utente di inserire il nome di un file esistente
 *
 * @param nomeFileSav stringa dove viene salvato il nome del file
 */
void cercaNomeSave(char nomeFileSav[DIM_STR]){

    bool fileNonEsistente = false;
    FILE *fp;

    do {
        //resetto il booleano ogni ciclo
        fileNonEsistente = false;

        getchar();
        printf("Inserisci il nome-->");
        scanf("%[^\n]s", nomeFileSav);

        //aggiungo il tipo .sav al file
        strcat(nomeFileSav, ".sav");

        //apro il file
        fp = fopen(nomeFileSav, "r");
        if(fp == NULL) {
            //se restituisce NULL vuol dire che non esiste
            fileNonEsistente = true;
            printf("Errore! Il file \"%s\" non esiste. Inserisci un nome diverso.\n", nomeFileSav);

        } else{

            //se invece viene aperto vuol dire che esiste
            printf(BLINK "\nCaricamento %s ...\n" RESET, nomeFileSav);
            fclose(fp);
        }

    } while (fileNonEsistente );

}

/**
 * richiede all'utente di inserire un nome nuovo per il file di salvataggio
 *
 * @param nomeFileSav stringa in cui viene memorizzato il nome scelto
 */
void sceltaNomeSave(char nomeFileSav[DIM_STR]){

    bool fileGiaEsistente = false;
    FILE *fp;

    do {
        //resetto il booleano ogni ciclo
        fileGiaEsistente = false;

        getchar();
        printf("Inserisci il nome-->");
        scanf("%[^\n]s", nomeFileSav);

        //aggiungo il tipo .sav al file
        strcat(nomeFileSav, ".sav");

        //apro il file
        fp = fopen(nomeFileSav, "r");
        if (fp != NULL) {
            //se non restituisce NULL vuol dire che esiste e quindi l'operazione di inserimento va ripetuta
            fclose(fp);
            fileGiaEsistente = true;
            printf("Errore! Il file \"%s\" esiste gia'. Inserisci un nome diverso.\n", nomeFileSav);
        } else{
            //vuol dire che il nome non esiste tra i file percio' il nome puo essere utilizzato
            printf(BLINK "\nCreazione %s ...\n" RESET, nomeFileSav);
        }

    } while (fileGiaEsistente );

}