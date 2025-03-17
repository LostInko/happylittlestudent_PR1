//
// Created by matte on 16/02/2024.
//

#ifndef PROGETTOESAME_GIOCATE_H
#define PROGETTOESAME_GIOCATE_H

void giocataUno(Giocatore *listaGiocatore, Carta **listaCarta, Carta **listaCartaScarto, Carta **listaCartaInGioco);
void terminatorePartita(Giocatore **listaGiocatore, Carta **listaCarta, Ostacolo **listaOstacolo, Personaggio *personaggio, int *arrayIndexVincenti);
void controllaStats(Giocatore *auxStampaStats, Giocatore *auxGiocatore, int dim);
char sceltaTerminaPartita();
int sceltaTurno();
void freeVarie(Giocatore *listaGiocatore, Carta *listaCarta, Ostacolo *listaOstacolo, Personaggio *personaggio, int *arrayIndexVincenti);
#endif //PROGETTOESAME_GIOCATE_H
