#include <stdio.h>
#include <stdlib.h>
#include "debugmalloc.h"
#include "din_tomb.h"


//letrehoz egy adott meretu dinamikus tombot
void dintomb_init(DinTomb *dt, int meret) {
    dt->meret = meret;
    dt->tomb = (char*) malloc(meret * sizeof(char));
    if (dt->tomb == NULL) printf("Hiba: Memoriafoglalas sikertelen\n");
}

//megduplazza egy dinamikus tomb meretet
void dintomb_novel(DinTomb *dt) {
    char* temp = (char*) malloc(dt->meret * 2 * sizeof(char));
    if (temp == NULL) printf("Hiba: Memoriafoglalas sikertelen\n");
    for (int i = 0; i < dt->meret; i++) temp[i] = dt->tomb[i];
    free(dt->tomb);
    dt->tomb = temp;
    dt->meret *= 2;
}

//felszabaditjuk a dinamikus tomb altal foglalt memoriat
void dintomb_free(DinTomb *dt) {
    free(dt->tomb);
}
