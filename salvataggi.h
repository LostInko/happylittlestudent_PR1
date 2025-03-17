//
// Created by matte on 28/02/2024.
//

#ifndef PROGETTOESAME_SALVATAGGI_H
#define PROGETTOESAME_SALVATAGGI_H

int caricaSalvataggio(Giocatore **listaGiocatore, Ostacolo **listaOstacolo, Carta **listaCarta, Carta **listaCartaScarto, char nomeFileSav[DIM_STR], int *turno);
void aggiungiSaveGiocatoreCoda(Giocatore **listaGiocatore, Giocatore auxGiocatore);
void aggiungiSaveOstacoloCoda(Ostacolo **listaOstacolo, Ostacolo auxOstacolo) ;
void aggiungiSaveCartaCoda(Carta **listaCarta, Carta auxCarta);
void creaSalvataggio(Giocatore *listaGiocatore, Ostacolo *listaOstacolo, Carta *listaCarta, Carta *listaCartaScarto, char nomeFileSav[DIM_STR], int turno);

#endif //PROGETTOESAME_SALVATAGGI_H
