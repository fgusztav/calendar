#ifndef REKORD_KEZELES_H
#define REKORD_KEZELES_H

#include <string.h>
#include "fajl_io.h"

Rekord ures_rek();

//bubble sort algoritmussal rendezi a rekordok tombjet, csak datum es idopont alapjan
void rendez(DinRekord dr);

//hozzad egy uj rekordot a DinRekord tombhoz, majd rendezi azt
void r_hozzaad(DinRekord *dr, Rekord r);

//torol egy adott rekordot a DinRekord tombbol
void r_torol(DinRekord *dr, Rekord r);

//atir egy rekordot - torli azt, majd egy ujat hoz letre az uj adatokkal
void r_modosit(DinRekord *dr, Rekord regi, Rekord uj);

//visszaadja a keresesnek megfelelo Rekordok indexeit tartalmazo tombot (dinamikus tomb, -1 jelzi a veget)
int* r_kereses(DinRekord dr, char* sz);

#endif
