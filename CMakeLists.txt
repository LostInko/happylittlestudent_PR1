cmake_minimum_required(VERSION 3.27)
project(progettoesame C)

set(CMAKE_C_STANDARD 11)

add_executable(progettoesame main.c
        "caricamento.c"
        "caricamento.h"
        "preparazionegioco.h"
        "preparazionegioco.c"
        "mescolamazzi.c"
        "mescolamazzi.h"
        "ANSI-color-codes.h"
        "giocate.c"
        "giocate.h"
        "utility.c"
        "utility.h"
        "stampeVarie.c"
        "stampeVarie.h"
        "calcoloPunteggio.c"
        "calcoloPunteggio.h"
        "vinciPerdi.c"
        "vinciPerdi.h"
        effettoCarte.c
        effettoCarte.h
        salvataggi.c
        salvataggi.h
        salvataggi.h
)
