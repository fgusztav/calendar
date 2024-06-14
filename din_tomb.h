#ifndef DIN_TOMB_H
#define DIN_TOMB_H

#include <stdbool.h>
#include "debugmalloc.h"

typedef struct DinTomb {
    char *tomb;
    int meret;
} DinTomb;

//letrehoz egy adott meretu dinamikus tombot
void dintomb_init(DinTomb *dt, int meret);

//megduplazza egy dinamikus tomb meretet
void dintomb_novel(DinTomb *dt);

//felszabaditjuk a dinamikus tomb altal foglalt memoriat
void dintomb_free(DinTomb *dt);

#endif
