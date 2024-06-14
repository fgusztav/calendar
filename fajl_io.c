#include <stdio.h>
#include "fajl_io.h"
#include "debugmalloc.h"
#define FAJLNEV "adatbazis.txt"

//DINREKORD FUGGVENYEK:

//letrehoz egy adott meretu rekordokbol allo dinamikus tombot
void dinrekord_init(DinRekord *dr, int meret) {
    dr->meret = meret;
    dr->tomb = (Rekord*) malloc(meret * sizeof(Rekord));
    if (dr->tomb == NULL) printf("Hiba: Memoriafoglalas sikertelen\n");
}

//a rekordokat tartalmazo dinamikus tomb meretet 1-gyel megnoveli
void dinrekord_novel(DinRekord *dr) {
    Rekord* temp = (Rekord*) malloc((dr->meret + 1) * sizeof(Rekord));
    if (temp == NULL) printf("Hiba: Memoriafoglalas sikertelen\n");
    for (int i = 0; i < dr->meret; i++) temp[i] = dr->tomb[i];
    free(dr->tomb);
    dr->tomb = temp;
    dr->meret += 1;
}

//a rekordokat tartalmazo dinamikus tomb meretet 1-gyel csokkenti
void dinrekord_csokkent(DinRekord *dr) {
    Rekord* temp = (Rekord*) malloc((dr->meret - 1) * sizeof(Rekord));
    if (temp == NULL) printf("Hiba: Memoriafoglalas sikertelen\n");
    for (int i = 0; i < dr->meret-1; i++) temp[i] = dr->tomb[i];
    free(dr->tomb);
    dr->tomb = temp;
    dr->meret -= 1;
}

//rekordokat tartalmazo dinamikus tomb altal foglalt memoriateruletet felszabaditja
void dinrekord_free(DinRekord dr) {
    for (int i = 0; i < dr.meret; i++) {
        dintomb_free(&(dr.tomb[i].hely));
        dintomb_free(&(dr.tomb[i].megj));
    }
    free(dr.tomb);
}

//FAJLT BEOLVASO FUGGVENYEK:

//beolvas egy sort a fajlbol, es DinTomb adattipusban adja vissza
//ha a fajl vegere ertunk akkor a masodik parameter igaz lesz
static DinTomb sor_beolvas(FILE* fp, bool* fajlvege) {
    DinTomb sor;
    dintomb_init(&sor, 32);
    int i = 0;
    char temp;
    while (true) {
        if ((temp = fgetc(fp)) == EOF) {
            *fajlvege = true;
            break;
        }
        else if (temp == '\n') break;
        sor.tomb[i] = temp;
        i++;
        //ha kell akkor megduplazza a sor max hosszat:
        if (i == sor.meret) dintomb_novel(&sor);
    }
    sor.tomb[i] = '\0';
    return sor;
}

//kap egy sor szoveget dinamikus tombben es visszaadja Rekord adattipussa alakitva
static Rekord rekord_sorbol(DinTomb sor) {
    Rekord r;
    dintomb_init(&r.hely, 2); dintomb_init(&r.megj, 2);
    int hely_idx = 0; int nev_idx = 0; int megj_idx = 0;
    sscanf(sor.tomb, " %d.%d.%d.\t%d:%d:%d\t", &r.dat.ev, &r.dat.ho, &r.dat.nap, &r.ido.h, &r.ido.m, &r.ido.s);
    int adatmezo_idx = 0;
    for (int i = 0; i < sor.meret; i++) {
        if (sor.tomb[i] == 9) { //ha a karakter tabulator
            adatmezo_idx++;
            continue;
        }
        switch (adatmezo_idx) {
            case 2: //hely (dintomb)
                r.hely.tomb[hely_idx++] = sor.tomb[i];
                if (r.hely.meret == hely_idx) dintomb_novel(&r.hely);
                break;
            case 3: //nev (max 50 kar.)
                if (nev_idx <= 51) r.nev[nev_idx++] = sor.tomb[i];
                else printf("Hiba: Esemeny nev a fajlban tul hosszu\n");
                break;
            case 4: //megjegyzes (dintomb)
                r.megj.tomb[megj_idx++] = sor.tomb[i];
                if (r.megj.meret == megj_idx) dintomb_novel(&r.megj);
                break;
        }
    }
    r.hely.tomb[hely_idx] = '\0';
    r.nev[nev_idx] = '\0';
    r.megj.tomb[megj_idx] = '\0';
    dintomb_free(&sor);
    return r;
}

//eltarolja a fajl tartalmat a Rekord tipusu adatokbol allo dinamikus tombben
DinRekord fajl_beolvas() {
    DinRekord dr;
    dinrekord_init(&dr, 1);

    FILE* fp;
    fp = fopen(FAJLNEV, "r");
    bool fajlvege = false;

    DinTomb sor = sor_beolvas(fp, &fajlvege);
    dr.tomb[0] = rekord_sorbol(sor);
    for (int i = 1; !fajlvege; i++) {
        dinrekord_novel(&dr);
        dr.tomb[i] = rekord_sorbol(sor_beolvas(fp, &fajlvege));
    }

    fclose(fp);
    return dr;
}


//FAJLBA IRO FUGGVENYEK:

void fajlba_ir(DinRekord dr) {
    FILE* fp;
    fp = fopen(FAJLNEV, "w");
    if (fp != NULL) {
        for (int i = 0; i < (int) dr.meret; i++) {
            Datum d = dr.tomb[i].dat;
            Ido t = dr.tomb[i].ido;
            fprintf(fp, "%d.%d.%d\t%d:%d:%d\t", d.ev, d.ho, d.nap, t.h, t.m, t.s);
            fprintf(fp, "%s\t%s\t%s", dr.tomb[i].hely.tomb, dr.tomb[i].nev, dr.tomb[i].megj.tomb);
            if (i < dr.meret-1) fprintf(fp, "\n");
        }
    }
    else printf("Hiba: Fajlba iras sikertelen\n");
    fclose(fp);
}
