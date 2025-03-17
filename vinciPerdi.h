//
// Created by matte on 22/02/2024.
//

#ifndef PROGETTOESAME_VINCIPERDI_H
#define PROGETTOESAME_VINCIPERDI_H

#define VITTORIA (60)
#define OSTACOLI_PERDENTI (3)

int cercaVincenti(Giocatore *listaGiocatore, int *arrayIndexVincenti);
int controllaEliminati(Giocatore *listaGiocatore);
void giocatoriVincenti(Giocatore *listaGiocatore, int numGiocatori, int contVincenti,  const int *arrayIndexVincenti);

#endif //PROGETTOESAME_VINCIPERDI_H
