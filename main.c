#include <stdio.h>
#include <stdlib.h>
#include "debugmalloc.h"
#include "megjelenites.h"

int main() {

    DinRekord Adat = fajl_beolvas();
    int valasz;
    do {
        valasz = fomenu();
        switch (valasz) {
        case 1: ;//ures sor compiler error elkerulesehez
            int idx = esemenyek_menu(idomenu(), Adat);
            if (idx < 0 && -idx < Adat.meret) r_torol(&Adat, Adat.tomb[-idx]); //torles
            else if (idx > 0 && idx < Adat.meret) { //modositas
                Rekord r_uj = ujrekord(Adat.tomb[idx]);
                printf("ENTER lenyomasaval az ertekek nem valtoznak\n");
                r_modosit(&Adat, Adat.tomb[idx], r_uj);
            }
            break;
        case 2: ;
            Rekord r_regi = ures_rek(); //egyszerubb mint kulon megjelenites fv. rekord modositashoz es letrehozashoz
            Rekord r_uj = ujrekord(r_regi);
            r_hozzaad(&Adat, r_uj);

            dintomb_free(&r_regi.hely);
            dintomb_free(&r_regi.megj);
            break;
        case 3:
            keres(Adat);
            break;
        }
    } while (valasz != 0);

    fajlba_ir(Adat);
    dinrekord_free(Adat);
    return 0;
}
