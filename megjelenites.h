#ifndef MEGJELENITES_H_INCLUDED
#define MEGJELENITES_H_INCLUDED

#include "rekord_kezeles.h"

//kiir konzolba egy rekordot egy sorba
void r_print(Rekord r);

DinTomb sor_beker();

int fomenu();

char idomenu();

int esemenyek_menu(char be, DinRekord dr);

struct Rekord ujrekord(Rekord regi);

void keres(DinRekord dr);
#endif // MEGJELENITES_H_INCLUDED
