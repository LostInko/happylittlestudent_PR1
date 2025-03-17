//
// Created by matte on 13/02/2024.
//

#ifndef PROGETTOESAME_PREPARAZIONEGIOCO_H
#define PROGETTOESAME_PREPARAZIONEGIOCO_H

#define MIN_CARTA (0)
#define MIN_GIOCATORI (2)
#define START_OST (NULL)
#define START_CFU (0)
#define START_CARTA (5)

Giocatore *eliminaListaGiocatore(Giocatore *listaGiocatore);
Giocatore *inputGiocatori( int numPersonaggi, Personaggio *personaggio, int *numGiocatori);
Giocatore *aggiungiGiocatoreInTesta(Giocatore *listaGiocatore, int ruolo, Personaggio *personaggio,  int i);
void arrayRandNoRipetizione(int numPersonaggi, int arrayRand[numPersonaggi]);
int randRangeIntero(int min, int max);
Giocatore *aggiungiGiocatoreInCoda(Giocatore *listaGiocatore, int ruolo, Personaggio *personaggio, int i);


#endif //PROGETTOESAME_PREPARAZIONEGIOCO_H
