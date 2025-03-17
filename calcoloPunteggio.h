//
// Created by matte on 21/02/2024.
//

#ifndef PROGETTOESAME_CALCOLOPUNTEGGIO_H
#define PROGETTOESAME_CALCOLOPUNTEGGIO_H



#define PERDENTE (-2)

int turnoSupplementare(Giocatore *listaGiocatore, Carta **listaCartaScarto, int dimNumGiocatori ,int numGiocatori,int maxIndex, int contStartMin);
void calcoloPunteggioTurno(int numGiocatori,Carta *listaCartaInGioco, Giocatore **listaGiocatore, Carta **listaCarta, int tipoOstacolo, Ostacolo **ostacoloTerra, Carta **listaCartaScarto, Ostacolo **listaOstacolo);
void risultatoRound(int numGiocatori, int *arrayPunteggi, Giocatore **listaGiocatore, Ostacolo **ostacoloTerra, Carta **listaCartaScarto, Ostacolo **listaOstacolo);
void calcoloPuntiOstacolo(Giocatore **listaGiocatore);

#endif //PROGETTOESAME_CALCOLOPUNTEGGIO_H
