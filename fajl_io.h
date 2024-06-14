#ifndef FAJL_IO_H
#define FAJL_IO_H

#include "din_tomb.h"

//STRUKTURAK:

typedef struct Datum {
    int ev;
    int ho;
    int nap;
} Datum;

typedef struct Ido {
    int h;
    int m;
    int s;
} Ido;

typedef struct Rekord {
    Datum dat;
    Ido ido;
    DinTomb hely;
    char nev[50+1];
    DinTomb megj;
} Rekord;

typedef struct DinRekord {
    Rekord* tomb;
    int meret;
} DinRekord;

//DINREKORD FUGGVENYEK:


//letrehoz egy adott meretu rekordokbol allo dinamikus tombot
void dinrekord_init(DinRekord *dr, int meret);

//a rekordokat tartalmazo dinamikus tomb meretet egyel megnoveli
void dinrekord_novel(DinRekord *dr);

//rekordokat tartalmazo dinamikus tomb altal foglalt memoriateruletet felszabaditja
void dinrekord_free(DinRekord dr);

//a rekordokat tartalmazo dinamikus tomb meretet 1-gyel csokkenti
void dinrekord_csokkent(DinRekord *dr);

//FAJLT BEOLVASO FUGGVENYEK:

//eltarolja a fajl tartalmat egy Rekord tipusu adatokbol allo dinamikus tombben
DinRekord fajl_beolvas();


//kiirja rekordokat az uj fajlba
void fajlba_ir(DinRekord dr);

#endif
