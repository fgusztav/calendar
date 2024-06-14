#include "rekord_kezeles.h"
#include "debugmalloc.h"

//a kapott Datumot es Idot atalakitja egy darab egesz szamma
static unsigned long long szam_idobol(Rekord r) {
    Datum d = r.dat;
    Ido t = r.ido;
    unsigned long long nagyszam = (unsigned long long) (d.ev*10000 + d.ho*100 + d.nap)*1000000 + 10000*t.h + 100*t.m + t.s;
    return nagyszam;
}

//osszehasonlit 2 struct Rekord tipusu elemet es visszaadja hogy egyenloek e
static bool r_ugyanaz(Rekord r1, Rekord r2) {
    bool cmp1 = (strcmp(r1.hely.tomb, r2.hely.tomb) == 0);
    bool cmp2 = (strcmp(r1.megj.tomb, r2.megj.tomb) == 0);
    bool cmp3 = (strcmp(r1.nev, r2.nev) == 0);
    if (szam_idobol(r1)==szam_idobol(r2) && cmp1 && cmp2 && cmp3) {
            return true;
    }
    return false;
}

//bubble sort algoritmussal rendezi a rekordok tombjet, csak datum es idopont alapjan
void rendez(DinRekord dr) {
    for (int i = dr.meret-1; i > 0; --i) {
        for (int k = 0; k < i; k++) {
            unsigned long long az1 = szam_idobol(dr.tomb[k]);
            unsigned long long az2 = szam_idobol(dr.tomb[k+1]);
            if (az1 > az2) {
                Rekord temp = dr.tomb[k];
                dr.tomb[k] = dr.tomb[k+1];
                dr.tomb[k+1] = temp;
            }
        }
    }
}

//hozzad egy uj rekordot a DinRekord tombhoz, majd rendezi azt !!nem lehet hozzaadni mar bennelevo Rekordot!!
void r_hozzaad(DinRekord *dr, Rekord r) {
    dinrekord_novel(dr);
    dr->tomb[dr->meret-1] = r;
    rendez(*dr);
}

//torol egy adott rekordot a DinRekord tombbol
void r_torol(DinRekord *dr, Rekord r) {
    for (int i = 0; i < dr->meret; i++) {
        if (r_ugyanaz(dr->tomb[i], r)) {
            dintomb_free(&(dr->tomb[i].megj));
            dintomb_free(&(dr->tomb[i].hely));
            //a torolt i indexu elem helyere eltolja az utana kovetkezo elemeket:
            for (int k = i; k < dr->meret-1; k++) {
                dr->tomb[k] = dr->tomb[k+1];

            }
            dinrekord_csokkent(dr);
            break;
        }
    }
}

//atir egy rekordot - torli azt, majd egy ujat hoz letre az uj adatokkal
void r_modosit(DinRekord *dr, Rekord regi, Rekord uj) {
    r_torol(dr, regi);
    r_hozzaad(dr, uj);
}

Rekord ures_rek() {
    Rekord R;
    R.dat.ev = 0; R.dat.ho = 0; R.dat.nap = 0;
    R.ido.h = 0; R.ido.m = 0; R.ido.s = 0;
    dintomb_init(&R.hely, 1); dintomb_init(&R.megj, 1);
    R.hely.tomb[0] = 0; R.megj.tomb[0] = 0;
    strcpy(R.nev, "");
    return R;
}

//egy sor szovegge atalakit egy Rekordot - kereseshez
static struct DinTomb rekord_sorra(Rekord r) {
    DinTomb sor;
    int meret = 32 + r.hely.meret + r.megj.meret + strlen(r.nev);
    dintomb_init(&sor, meret);
    sprintf(sor.tomb, "%4.4d.%2.2d.%2.2d%2.2d:%2.2d:%2.2d", r.dat.ev, r.dat.ho, r.dat.nap, r.ido.h, r.ido.m, r.ido.s);
    strncat(sor.tomb, r.hely.tomb ,meret-strlen(sor.tomb)-1);
    strncat(sor.tomb, r.megj.tomb ,meret-strlen(sor.tomb)-1);
    strncat(sor.tomb, r.nev ,meret-strlen(sor.tomb)-1);
    sor.tomb[sor.meret-1] = 0;
    return sor;
}

//visszaadja a keresesnek megfelelo Rekordok indexeit tartalmazo tombot (dinamikus tomb, -1 jelzi a veget)
int* r_kereses(DinRekord dr, char* sz) {
    int* talal = (int*) malloc(sizeof(int) * dr.meret);
    int b = 0;

    for (int a = 0; a < dr.meret; a++) {
        DinTomb sor = rekord_sorra(dr.tomb[a]);
        if (strstr(sor.tomb, sz) != NULL) {
            talal[b] = a;
            b++;
        }
        dintomb_free(&sor);
    }
    talal[b] = -1;
    return talal;
}
