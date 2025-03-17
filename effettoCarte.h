//
// Created by matte on 25/02/2024.
//

#ifndef PROGETTOESAME_EFFETTOCARTE_H
#define PROGETTOESAME_EFFETTOCARTE_H
void effettoCartaRUBA(Giocatore **listaGiocatore, Giocatore **giocatoreAttuale, int indexGiocatoreAttuale);
int effettoCartaSCARTAP(Giocatore **listaGiocatore, Carta **listaCartaScarto);
void effettoCartaSCAMBIADS(Giocatore **listaGiocatore,Carta **listaCartaInGioco, int indexGiocatoreAttuale, int *arrayPunteggi);
int effettoCartaSCARTAE(Giocatore **listaGiocatore,Carta **listaCartaScarto);
void effettoCartaSCARTAC(Giocatore **listaGiocatore, Carta **listaCartaScarto);
void effettoCartaSCAMBIAP(int *arrayPunteggi, int numGiocatori);
void effettoCartaSBIRCIA(Carta **listaCarta, Giocatore **listaGiocatore, Carta **listaCartaScarto);
void effettoCartaSCAMBIAC(Giocatore **listaGiocatore,Carta **listaCartaInGioco, int *arrayPunteggi);
void effettoCartaANNULLA( int numGiocatori, int *arrayEffetti, Giocatore *listaGiocatore);
void istantaneoCartaAUMENTA(int *arrayPunteggi, Giocatore **listaGiocatore);
void istantaneoCartaDIMINUISCI(int *arrayPuntiEffetto, Giocatore **listaGiocatore);
void istantaneoCartaINVERTI(int *arrayPuntiEffetto, int numGiocatori);
void istantaneoCartaDIROTTA(Ostacolo **ostacoloTerra, Giocatore **listaGiocatore, Carta **listaCartaScarto, Ostacolo **listaOstacolo);
void istantaneoCartaSALVA(Ostacolo **ostacoloTerra, Ostacolo **listaOstacolo);

bool giocaCartaEffetto(int effettoInGioco, Giocatore **listaGiocatore,Carta **listaCarta, Giocatore **giocatoreAttuale, Carta **listaCartaInGioco, Carta **listaCartaScarto, int indexGiocatoreAttuale, int *arrayPunteggi, int numGiocatori, int *arrayEffetti, int *arrayPuntiEffetto);
void giocaIstantaneaPunteggio(int effettoInGioco, Giocatore **listaGiocatore, int *arrayPunteggi, int numGiocatori, int *arrayPuntiEffetto);
void giocaIstantaneaOstacolo(int effettoInGioco, Giocatore **listaGiocatore, Ostacolo **listaOstacolo, Ostacolo **ostacoloTerra, Carta **listaCartaScarto);
void controllaIstantaneaOstacolo(Giocatore **listaGiocatore, Carta **listaCartaScarto, Ostacolo **listaOstacolo, Ostacolo **ostacoloTerra);
void giocaInstantanee(Giocatore **listaGiocatore, Carta **listaCartaScarto, int *arrayPunteggi, int numGiocatori, int *arrayPuntiEffetto);
bool controlloIstantanee(Giocatore **listaGiocatore, Carta **listaCartaScarto, int *arrayPunteggi, int numGiocatori, int *arrayPuntiEffetto);

#endif //PROGETTOESAME_EFFETTOCARTE_H
