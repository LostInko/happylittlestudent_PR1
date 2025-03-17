//
// Created by matte on 10/02/2024.
//

#ifndef PROGETTOESAME_CARICAMENTO_H
#define PROGETTOESAME_CARICAMENTO_H

#define DIM_STR (31)
#define DIM_STR_PLUS (127)
#define DIM (4)
#define BUFFER (1000)

///caricamento personaggio
typedef struct personaggio{
    char nomePersonaggio[DIM_STR+1];
    int arrayBonMal[DIM];
}Personaggio;


///enum carte
typedef enum{NESSUNO, SCARTAP, RUBA, SCAMBIADS, SCARTAE,
             SCARTAC, SCAMBIAP, DOPPIOE, SBIRCIA, SCAMBIAC,
             ANNULLA, AUMENTA, DIMINUISCI, INVERTI, SALVA, DIROTTA} EffettoCarta;
///caricamento carte cfu
struct carta{
    char nomeCarta[DIM_STR+1];
    int valore;
    EffettoCarta effettoCarta;
    struct carta *nextCarta;
};
typedef struct carta Carta;


    ///enumerazione ostacolo
typedef enum {  STUDIO = 1,SOPRAVVIVENZA,SOCIALE,ESAME} TipoOstacolo;
    ///caricamento carta ostacolo
struct ostacolo{
    char nomeOstacolo[DIM_STR+1];
    char descOstacolo[DIM_STR_PLUS+1];
    TipoOstacolo tipoOstacolo;
    struct ostacolo *nextOstacolo;
};
typedef struct ostacolo Ostacolo;



///caricamento giocatore
struct giocatore{
    char nomeUtente[DIM_STR+1];
    Personaggio personaggio;
    int cfuAccumulati;
    Carta *manoCarta;
    Ostacolo *manoOstacolo;
    struct giocatore *nextGiocatore;
};
typedef struct giocatore Giocatore;


///firme subroutine caricamento
int contaRigheFile(char nomeFile[]);

Personaggio *caricaPersonaggio(int dim, int *num);
Personaggio *allocaPersonaggio(int num);

Ostacolo *caricaOstacolo();
Ostacolo *aggiungiOstacoloInTesta(Ostacolo *listaOstacolo, int index, FILE *fp);

Carta *aggiungiCartaInTesta(Carta *listaCarta, Carta cartaCopia);
Carta *caricaCarta();

Carta *eliminaListaCarta(Carta *listaCarta);
Ostacolo *eliminaListaOstacolo(Ostacolo *listaOstacolo);

#endif //PROGETTOESAME_CARICAMENTO_H
