//
// Created by matte on 16/02/2024.
//

#ifndef PROGETTOESAME_STAMPEVARIE_H
#define PROGETTOESAME_STAMPEVARIE_H

void stampaCarte(Giocatore *auxGiocatore);
void stampaGiocatoreCompleta(Giocatore *listaGiocatore);
void stampaEffettoCarta(int index);
void stampaTipoOstacolo(int index);
void stampaOstacoloTerra(Ostacolo *ostacoloTerra);
void stampaGiocatoreTurnoNoNext(Giocatore *auxGiocatore, int dim);
void stampaGiocatoreTurnoSupplementare(Giocatore *auxGiocatore);
void stampaCarteGiocatore(Carta *manoCarta);
void stampaOstacoli(Giocatore *auxGiocatore);
void stampaEffettoCartaAttivazione(int index);

#endif //PROGETTOESAME_STAMPEVARIE_H
