//
// Created by matte on 16/02/2024.
//

#ifndef PROGETTOESAME_UTILITY_H
#define PROGETTOESAME_UTILITY_H

#define FINE_PARTITA (0)
#define GIOCA_CARTA (1)
#define CONTROLLA_STATS (2)

void pescaCartaScelta(Giocatore *giocatore, int cartaScelta, Carta **listaCartaInGioco);
void pescaCarta(Carta **cartaPescabile, Carta **cartaMano, int num, Carta **listaCartaScarto);
void pescaOstacolo(Ostacolo **ostacoloPescato, Ostacolo **ostacoloTerra, int num);
int sceltaCartaInGioco(int contaCarta);
void copiaCartaScelta(Giocatore *giocatore, int cartaScelta, Carta **listaCartaInGioco);
int *allocaArrayInteri(int num);
int trovaMin(int dim, const int array[dim], int *minIndex);
int trovaMax(int dim, const int array[dim],  int *maxIndex);
void spostaOstacoloInCoda(Ostacolo **ostacoloAux, Ostacolo **listaOstacolo);
void arraySortingCrescente(int array[], int dim);
void eliminaDuplicati(int num, int array[num]);
int contatoreListaOstacolo(Ostacolo *listaOstacolo);
int contatoreListaCarta(Carta *listaCarta);
int contatoreListaGiocatore(Giocatore *listaGiocatore);
int sceltaCartaEffettoInGioco(int contaCarta);
int cercaNomeEscluso(Giocatore *listaGiocatore, Giocatore *giocatoreAttuale);
int cercaNome(Giocatore *listaGiocatore);
int sceltaCartaGenerale(int contaCarta);
char sceltaSN();
void clearConsole();
void sceltaNomeSave(char nomeFileSav[DIM_STR]);
void cercaNomeSave(char nomeFileSav[DIM_STR]);

#endif //PROGETTOESAME_UTILITY_H
