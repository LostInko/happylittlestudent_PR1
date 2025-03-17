//Nome: Matteo Manai (m.manai22@studenti.unica.it)
//Matricola: 60/61/66438
//Tipologia progetto: Medio

///dichiarazione librerie
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "caricamento.h"
#include "preparazionegioco.h"
#include "mescolamazzi.h"
#include "giocate.h"
#include "utility.h"
#include "stampeVarie.h"
#include "calcoloPunteggio.h"
#include "vinciPerdi.h"
#include "ANSI-color-codes.h"
#include "salvataggi.h"
///fine dichiarazione librerie


///entry point
int main() {

    ///FILE
    FILE *log = NULL;

    ///dichiarazione variabili
    Personaggio *personaggio = NULL;
    int numPersonaggi;

    Ostacolo *listaOstacolo = NULL;

    Carta *listaCarta = NULL;
    Carta *listaCartaInGioco = NULL;
    Carta *auxManoCarta = NULL;
    Carta *listaCartaScarto = NULL;

    Giocatore *listaGiocatore = NULL;
    int numGiocatori = 0;

    char sceltaSav;
    char nomeFileSav[DIM_STR];

    int numCarte;
    int turno = 1;
    int scelta;
    Ostacolo *ostacoloTerra = NULL;
    Giocatore *auxGiocatore = NULL;
    Giocatore *auxStampaStats = NULL;

    int contaCarte;
    int contScorrimento;
    int contVincenti;
    int indexPerdente = 0;
    int *arrayIndexVincenti = NULL;

    Ostacolo *auxManoOstacolo = NULL;

    ///inizializzazione
    srand(time(NULL));

    //inizializzo il file log cancellando il contenuto delle scorse partite
    log = fopen("log.txt", "w");
    if(log == NULL) exit(-1);
    fclose(log);

    /*printf(HGRN " __                                 __ __ __   __   __        \n"
           "|  |--.---.-.-----.-----.--.--.    |  |__|  |_|  |_|  |.-----.\n"
           "|     |  _  |  _  |  _  |  |  |    |  |  |   _|   _|  ||  -__|\n"
           "|__|__|___._|   __|   __|___  |    |__|__|____|____|__||_____|\n"
           "            |__|  |__|  |_____|                               \n"
           "        __            __               __                     \n"
           ".-----.|  |_.--.--.--|  |.-----.-----.|  |_.-----.            \n"
           "|__ --||   _|  |  |  _  ||  -__|     ||   _|__ --|            \n"
           "|_____||____|_____|_____||_____|__|__||____|_____|            \n"
           "                                                              \n"
           ""RESET);
*/
    printf(HGRN" _   _                           _     _ _   _   _      \n"
           "| | | |                         | |   (_) | | | | |     \n"
           "| |_| | __ _ _ __  _ __  _   _  | |    _| |_| |_| | ___ \n"
           "|  _  |/ _` | '_ \\| '_ \\| | | | | |   | | __| __| |/ _ \\\n"
           "| | | | (_| | |_) | |_) | |_| | | |___| | |_| |_| |  __/\n"
           "\\_| |_/\\__,_| .__/| .__/ \\__, | \\_____/_|\\__|\\__|_|\\___|\n"
           "            | |   | |     __/ |                         \n"
           "            |_|   |_|    |___/                          \n"
           "   _____ _             _            _                   \n"
           "  /  ___| |           | |          | |                  \n"
           "  \\ `--.| |_ _   _  __| | ___ _ __ | |_ ___             \n"
           "   `--. \\ __| | | |/ _` |/ _ \\ '_ \\| __/ __|            \n"
           "  /\\__/ / |_| |_| | (_| |  __/ | | | |_\\__ \\            \n"
           "  \\____/ \\__|\\__,_|\\__,_|\\___|_| |_|\\__|___/            \n"
           "                                                        \n"
           "                                                        \n"
           ""RESET);


    ///salvataggio partita
    printf(RED "---------------- Salvataggi ----------------\n" RESET);
    printf("Vuoi caricare un salvataggio?\n");

    //richiedo all'utente se vuole caricare un save o no
    sceltaSav = sceltaSN();

    //se non vuole creo una nuova partita e creo un nuovo file di salvataggio
    if(sceltaSav == 'n') {
        printf("\n-------- ?? Come vuoi chiamare il tuo salvataggio ?? --------\n[ricorda di non scrivere .sav!!]\n");

        //richiedo il nome del file
        sceltaNomeSave(nomeFileSav);

        ///acquisizione personaggi
        personaggio = caricaPersonaggio(DIM, &numPersonaggi);

        ///acquisizione ostacolo
        listaOstacolo = caricaOstacolo();
        listaOstacolo = mescolaMazzoOstacolo(listaOstacolo);

        ///acquisizione carta
        listaCarta = caricaCarta();
        listaCarta = mescolaMazzoCarta(listaCarta);

        /*auxManoCarta = listaCarta;
        stampaCarteGiocatore(auxManoCarta);*/

        ///acquisizione giocatori
        listaGiocatore = inputGiocatori(numPersonaggi, personaggio, &numGiocatori);

        ///assegnazione prime carte
        auxGiocatore = listaGiocatore;
        while (auxGiocatore != NULL) {

            auxGiocatore->cfuAccumulati = START_CFU;
            auxGiocatore->manoOstacolo = START_OST;

            pescaCarta(&listaCarta, &auxGiocatore->manoCarta, START_CARTA, &listaCartaScarto);

            auxGiocatore = auxGiocatore->nextGiocatore;

        }

    } else if(sceltaSav == 's'){

        //se l'utente vuole caricare un file sav chiedo il nome
        printf("\n-------- ?? Qual'e' il nome del salvataggio ?? --------\n[ricorda di non scrivere .sav!!]\n");

        cercaNomeSave(nomeFileSav);

        //carico il salvataggio inserito
        numGiocatori = caricaSalvataggio(&listaGiocatore, &listaOstacolo, &listaCarta, &listaCartaScarto, nomeFileSav, &turno);

    }

    //stampe di debug delle informazioni su mazzi e giocatori
    /*auxGiocatore = listaGiocatore;
    stampaGiocatoreCompleta(auxGiocatore);

    printf("scarto\n");
    auxManoCarta = listaCartaScarto;
    stampaCarteGiocatore(listaCartaScarto);

    printf("carte\n");
    auxManoCarta = listaCarta;
    stampaCarteGiocatore(listaCarta);

    printf("ostacoli\n");
    auxManoOstacolo = listaOstacolo;
    while (auxManoOstacolo != NULL) {
        printf("- %s\n", auxManoOstacolo->nomeOstacolo);
        auxManoOstacolo = auxManoOstacolo->nextOstacolo;
    }*/

    ///inizio turni
    getchar();
    printf("\nCaricamento partita in corso...\n");
    printf( RED "----------------- Inizio Partita -----------------\n" RESET);

    //do while che gestisce tutti i turni di gioco
    do {
        ///salvataggio turno
        auxGiocatore = listaGiocatore;
        creaSalvataggio(auxGiocatore, listaOstacolo, listaCarta, listaCartaScarto, nomeFileSav, turno);

        ///inizializzazione preTurno
        contVincenti = 0;
        arrayIndexVincenti = allocaArrayInteri(numGiocatori);
        for (int i = 0; i < numGiocatori; ++i) {
            arrayIndexVincenti[i] = -1;
        }


        printf(BOLD "--------- Premi qualsiasi tasto per iniziare il turno! ---------\n" RESET);
        getchar();
        clearConsole();

        ///salvo sul log i turni
        log = fopen("log.txt", "a");
        if(log == NULL) exit(-5);
        fprintf(log, "\nTURNO %d\n", turno);
        fclose(log);

        //inserimento manuale ostacoli
        /*int aux;
        printf("\n a chi vuoi dare 10 ost");
        scanf("%d", &aux);
        contScorrimento = 0;
        auxGiocatore = listaGiocatore;
        while (auxGiocatore!= NULL) {
            if(contScorrimento == aux) {
                pescaOstacolo(&listaOstacolo, &auxGiocatore->manoOstacolo, 10);
            }
            contScorrimento++;
            auxGiocatore = auxGiocatore->nextGiocatore;
        }*/

        //pesco dal mazzo l'ostacolo in gioco
        pescaOstacolo(&listaOstacolo, &ostacoloTerra, 1);

        //scrivo sul log
        log = fopen("log.txt", "a");
        if(log == NULL) exit(-5);
        fprintf(log, "Ostacolo del turno: %s\n\n", ostacoloTerra->nomeOstacolo);
        fclose(log);

        //scorro il turno per ogni giocatore
        auxGiocatore = listaGiocatore;
        while (auxGiocatore != NULL) {

            printf(RED "\n=========================== TURNO %d ===========================\n" RESET, turno);

            //stampo le informazioni sull'ostacolo a terra
            stampaOstacoloTerra(ostacoloTerra);

            //stampo le informazioni del giocatore
            stampaGiocatoreTurnoNoNext(auxGiocatore, DIM);

            //ottengo la scelta del turno
            scelta = sceltaTurno();

            //nello switch la scelta puo' essere di 3 tipi
            switch (scelta) {

                case CONTROLLA_STATS:
                    auxStampaStats = listaGiocatore;
                    controllaStats(auxStampaStats, auxGiocatore, DIM);
                    stampaCarte(auxGiocatore);
                    //non inserisco un break in modo da obbligare il giocatore a scegliere una carta

                case GIOCA_CARTA:
                    giocataUno(auxGiocatore, &listaCarta, &listaCartaScarto, &listaCartaInGioco);
                    break;

                case FINE_PARTITA:
                    terminatorePartita(&listaGiocatore, &listaCarta, &listaOstacolo, personaggio, arrayIndexVincenti);
                    break;

                default:
                    printf("Errore!\n");
            }

            clearConsole();

            auxGiocatore = auxGiocatore->nextGiocatore;
        }

        //stampa di debug della lista in gioco
        /*printf("Stato aggiornato della listaCartaInGioco:\n");
        stampaCarteGiocatore(listaCartaInGioco);*/

        ///calcolo punteggio
        auxGiocatore = listaGiocatore;
        calcoloPunteggioTurno(numGiocatori, listaCartaInGioco,  &auxGiocatore, &listaCarta,  ostacoloTerra->tipoOstacolo, &ostacoloTerra, &listaCartaScarto, &listaOstacolo);

        //faccio in modo che tutti i giocatori abbiano 5 carte prima dell'inizio del nuovo turno
        auxGiocatore = listaGiocatore;
        while (auxGiocatore != NULL){
            numCarte = contatoreListaCarta(auxGiocatore->manoCarta);
            if(numCarte <START_CARTA){
                pescaCarta(&listaCarta, &auxGiocatore->manoCarta, START_CARTA-numCarte, &listaCartaScarto);
            }
            auxGiocatore = auxGiocatore->nextGiocatore;
        }

        //aumento i punteggi cfu in base al numero di carte ostacolo
        auxGiocatore = listaGiocatore;
        calcoloPuntiOstacolo(&listaGiocatore);

        //inserimento manuale cfu
        /*printf("inserimento manuale cfu\n");
        auxGiocatore = listaGiocatore;
        while (auxGiocatore != NULL){
            scanf("%d", &auxGiocatore->cfuAccumulati);

            auxGiocatore = auxGiocatore->nextGiocatore;
        }*/

        ///gestisco l'eliminazione dei perdenti
        contScorrimento = 0;
        auxGiocatore = listaGiocatore;
        indexPerdente = controllaEliminati(auxGiocatore);

        //elimino il giocatore
        auxGiocatore = listaGiocatore;
        Giocatore *giocatoreDaEliminare = listaGiocatore;
        Giocatore *giocatorePrecedente = NULL;

        //cerco il giocatore corrispondente all'indice
        contScorrimento = 0;

        //scorro finché non trovo il giocatore corrispondente
        while (contScorrimento < indexPerdente && giocatoreDaEliminare != NULL) {

            //mi salvo il giocatore precedente a quello da eliminare
            giocatorePrecedente = giocatoreDaEliminare;
            giocatoreDaEliminare = giocatoreDaEliminare->nextGiocatore;
            contScorrimento++;
        }

        //se c'è un indice perdente valido
        if (indexPerdente >= 0) {

            // Stampo la mano di ostacoli che l'ha fatto perdere
            printf(GRN "------ Ostacoli nella sua mano ------\n" RESET);
            auxManoOstacolo = giocatoreDaEliminare->manoOstacolo;
            while (auxManoOstacolo != NULL) {
                printf("--->%-20s\t", auxManoOstacolo->nomeOstacolo);
                stampaTipoOstacolo(auxManoOstacolo->tipoOstacolo);
                auxManoOstacolo = auxManoOstacolo->nextOstacolo;
            }
            printf("\n");

            //scarto le sue carte
            auxManoCarta = giocatoreDaEliminare->manoCarta;
            numCarte = contatoreListaCarta(auxManoCarta);
            pescaCarta(&auxManoCarta, &listaCartaScarto, numCarte, &listaCartaScarto);

            //metto in fondo al mazzo la sua mano di ostacoli
            auxManoOstacolo = giocatoreDaEliminare->manoOstacolo;
            while (auxManoOstacolo != NULL) {
                spostaOstacoloInCoda(&auxManoOstacolo, &listaOstacolo);
                auxManoOstacolo = auxManoOstacolo->nextOstacolo;
            }

            //scrivo un messaggio di avviso
            printf(RED "\n=============================================================\n"
                   "\tIl giocatore %s e' stato eliminato dalla partita.\n"
                   "=============================================================\n\n" RESET, giocatoreDaEliminare->nomeUtente);
                    getchar();

            //libero il giocatore dalla lista e ricollego gli altri giocatori
            if (giocatoreDaEliminare == listaGiocatore) {
                //in caso questo si trovi in testa
                listaGiocatore = (listaGiocatore)->nextGiocatore;
                free(giocatoreDaEliminare);
            } else {
                //in caso non sia in testa uso il precedente che mi sono salvato prima
                giocatorePrecedente->nextGiocatore = giocatoreDaEliminare->nextGiocatore;
                free(giocatoreDaEliminare);
            }
        }

        ///conto i giocatori in partita
        auxGiocatore = listaGiocatore;
        numGiocatori = contatoreListaGiocatore(auxGiocatore);

        ///vincenti
        auxGiocatore = listaGiocatore;

        //conto i vincenti
        contVincenti = cercaVincenti(auxGiocatore, arrayIndexVincenti);

        //gestisco i vincitori della partita
        giocatoriVincenti (listaGiocatore,numGiocatori, contVincenti, arrayIndexVincenti);

        ///scarto le carte in gioco
        contaCarte = contatoreListaCarta(listaCartaInGioco);
        pescaCarta(&listaCartaInGioco, &listaCartaScarto, contaCarte, &listaCartaScarto);

        //stampa di debug della lista scarto
        /*printf("Stato aggiornato della lista scarto:\n");
        stampaCarteGiocatore(listaCartaScarto);*/


        turno++;

        ///punteggi cfu
        contScorrimento = 0;
        printf(BOLD "--------- Premi qualsiasi tasto passare al punteggio della partita! ---------\n" RESET);
        getchar();
        printf(YEL "======================== PUNTEGGI PARTITA ======================== \n" RESET);
        auxGiocatore = listaGiocatore;
        while (auxGiocatore != NULL){
            printf("Giocatore %d:\t%s\t-->%d\n",contScorrimento+1, auxGiocatore->nomeUtente,auxGiocatore->cfuAccumulati);

            auxGiocatore = auxGiocatore->nextGiocatore;
            contScorrimento++;
        }

    //il while continua finche non viene trovato un vincente o finche il numero di giocatori è maggiore di 1
    } while (contVincenti == 0 && numGiocatori > 1);

    ///free
    freeVarie(listaGiocatore, listaCarta, listaOstacolo, personaggio, arrayIndexVincenti);

    return 0;
}
