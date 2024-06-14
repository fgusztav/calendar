#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "debugmalloc.h"
#include "megjelenites.h"

const unsigned int tav = 2;//oszlopok tavolsaga
const unsigned int sz_hossz = 27;//szoveges oszlopok szelessege betukben

//INPUT:

//beker egy sort a konzolbol egy dinamikus tombbe
DinTomb sor_beker() {
    DinTomb sor;
    dintomb_init(&sor, 1);
    int i = 0;
    while (sor.tomb[i-1] != '\n') {
        sor.tomb[i] = getchar();
        if (i == sor.meret-1) dintomb_novel(&sor);
        i++;
    }
    sor.tomb[i-1] = 0;
    return sor;
}

//OUTPUT:

//kiir konzolba egy rekordot
void r_print(Rekord r) {
    //datum, ido kiirasa:
    printf("%4d.%2.2d.%2.2d.%*.2d:%2.2d:%2.2d", r.dat.ev, r.dat.ho, r.dat.nap, tav+2, r.ido.h, r.ido.m, r.ido.s);

    char* kiir[3] = {r.hely.tomb, r.nev, r.megj.tomb};//a kiirando szovegeket pointerei
    //a pointereken vegigmenve a szoveg hosszanak megfelelo formaban irja ki ("..."-al vagy nelkul):
    for (int i = 0; i < 3; i++) {
        if (strlen(kiir[i]) <= sz_hossz) printf("%*.*s", sz_hossz+tav, sz_hossz, kiir[i]);
        else printf("%*.*s...", sz_hossz-3+tav, sz_hossz-3, kiir[i]);
    }
}

//adatmezok fejlecet irja ki
static void fejlec() {
    printf("%*s%*s",4+2+2 +3, "Datum",tav+2+2+2 +2, "Ido");
    unsigned int x = tav + sz_hossz;
    printf("%*s%*s%*s",x, "Hely",x, "Nev",x, "Megjegyzes");

}

//kirajzol megfelelo hosszusagu elvalaszto vonalat, vegen uj sorral
static void vonal() {
    unsigned int hossz = 4+2+2 +3 + 2+2+2 +2 + 4*tav + 3*sz_hossz;
    for (int i = 0; i <= hossz/2; i++) printf("~-");
    printf("\n");
}

//fomenu
int fomenu() {
    system("cls");
    vonal();
    printf("[1] Esemenyek kilistaza\n[2] Uj rekord\n[3] Kereses\n[0] Kilepes\n");
    vonal();
    int valasz;

    DinTomb sor = sor_beker();
    sscanf(sor.tomb, "%d ", &valasz);
    dintomb_free(&sor);

    return valasz;
}

char idomenu() {
    system("cls");
    printf("Idotartam?\n[e] Ev\n[h] Honap\n[n] Nap\n");
    char valasz;

    DinTomb sor = sor_beker();
    sscanf(sor.tomb, "%c ", &valasz);
    dintomb_free(&sor);

    return tolower(valasz);
}

//kilistazza a rekordokat egy adott idotartomanyban
//visszaadja a DinRekord egy indexet+1, amit torolni kell ha negativ es modositani ha pozitiv, 0-at ad vissza ha a fomenube kell visszalepni
static int kilistaz(Datum d, char mode, DinRekord dr) {
        int azon = 1;
        system("cls");
        printf("\t");
        fejlec();
        printf("\n\t");
        vonal();
        int evszam, honap;
        bool talalat = false;
        //szures datum szerint:
        int idx;
        switch (mode) {
            case 'n':
                for (idx = 0; idx < dr.meret; idx++) {
                    Datum temp = dr.tomb[idx].dat;
                    if (temp.ev == d.ev && temp.ho == d.ho && temp.nap == d.nap) {
                        printf("[%5d] ", azon); r_print(dr.tomb[idx]); printf("\n");
                        azon++;
                        talalat = true;
                    } else if (talalat) break;
                }
                break;
            case 'e':
                evszam = d.ev;
                for (idx = 0; idx < dr.meret; idx++) {
                    Datum temp = dr.tomb[idx].dat;
                    if (temp.ev == evszam) {
                        printf("[%5d] ", azon); r_print(dr.tomb[idx]); printf("\n");
                        azon++;
                        talalat = true;
                    } else if (talalat) break;
                }
                break;
            case 'h':
                evszam = d.ev;
                honap = d.ho;
                for (idx = 0; idx < dr.meret; idx++) {
                    Datum temp = dr.tomb[idx].dat;
                    if (temp.ev == evszam && temp.ho == honap) {
                        printf("[%5d] ", azon); r_print(dr.tomb[idx]); printf("\n");
                        azon++;
                        talalat = true;
                    } else if (talalat) break;
                }
                break;
        }

        printf("\t");
        vonal();
        if (azon == 1) printf("\tEzen a napon nincsenek meg rekordok \n");
        else printf("Valasszon ki egy rekordot ha torolni/modositani szeretne\n");
        printf("[0] Vissza a fomenube\n");
        int valasztas;

        DinTomb sor = sor_beker();
        sscanf(sor.tomb, "%d ", &valasztas);
        dintomb_free(&sor);

        if (valasztas == 0 || valasztas >= azon) return 0;
        valasztas = valasztas + idx - azon;
        system("cls");
        fejlec(); printf("\n");
        r_print(dr.tomb[valasztas]);
        printf("\n[t] Torles\n[m] Modositas\n");
        char k;

        sor = sor_beker();
        sscanf(sor.tomb, "%c ", &k);
        dintomb_free(&sor);

        if (k == 't') return -valasztas;
        else if (k == 'm') return valasztas;
        return 0;
}


//visszaadja a DinRekord egy indexet+1, amit torolni kell ha negativ es modositani ha pozitiv, 0-at ad vissza ha a fomenube kell visszalepni
int esemenyek_menu(char be, DinRekord dr) {
    system("cls");
    Datum d;
    DinTomb sor;
    switch(be) {
    case 'n':
        printf("Kerem adja meg a pontos datumot (YYYY-MM-DD alakban):\n");

        sor = sor_beker();
        sscanf(sor.tomb, "%d-%d-%d ", &d.ev, &d.ho, &d.nap);
        dintomb_free(&sor);

        return kilistaz(d, be, dr);
    case 'e':
        printf("Melyik ev?\n");

        sor = sor_beker();
        sscanf(sor.tomb, "%d ", &d.ev);
        dintomb_free(&sor);

        d.ho = -1; d.nap = -1;
        return kilistaz(d, be, dr);
    case 'h':
        printf("Melyik ev?\n");

        sor = sor_beker();
        sscanf(sor.tomb, "%d ", &d.ev);
        dintomb_free(&sor);

        printf("Hanyadik honap?\n");

        sor = sor_beker();
        sscanf(sor.tomb, "%d ", &d.ho);
        dintomb_free(&sor);

        d.nap = -1;
        return kilistaz(d, be, dr);
    }
    return 0;
}

struct Rekord ujrekord(Rekord regi) {
    Rekord uj;
    system("cls");

    printf("Adja meg az uj datumot: [YYYY-MM-DD]\n");
    DinTomb sor = sor_beker();
    if (sor.meret == 1) uj.dat = regi.dat;
    else sscanf(sor.tomb, "%d-%d-%d ", &uj.dat.ev, &uj.dat.ho, &uj.dat.nap);
    dintomb_free(&sor);

    printf("Adja meg az uj idopontot: [HH:MM:SS]\n");
    sor = sor_beker();
    if (sor.meret == 1) uj.ido = regi.ido;
    else sscanf(sor.tomb, "%d:%d:%d ", &uj.ido.h, &uj.ido.m, &uj.ido.s);
    dintomb_free(&sor);

    printf("Adja meg az uj helyszint:\n");
    DinTomb ujhely = sor_beker();
    if (ujhely.meret == 1) uj.hely = regi.hely;
    else uj.hely = ujhely;

    printf("Adja meg az esemeny uj nevet: [max. 50 karakter]\n");
    sor = sor_beker();
    if (strlen(sor.tomb) <= 50 && sor.meret != 0) strcpy(uj.nev, sor.tomb);
    else strcpy(uj.nev, regi.nev);
    dintomb_free(&sor);

    printf("Adja meg az esemeny uj megjegyzeset:\n");
    DinTomb ujmegj = sor_beker();
    if (ujmegj.meret == 1) uj.megj = regi.megj;
    else uj.megj = ujmegj;

    return uj;
}

void keres(DinRekord dr) {
    system("cls");
    printf("Adja meg a keresett szoveget:\n");

    DinTomb sor = sor_beker();


    int* talalatok = r_kereses(dr, sor.tomb);

    int i;
    for (i = 0; talalatok[i] != -1; i++) {
        r_print(dr.tomb[talalatok[i]]);
        printf("\n");
    }

    if (i == 0) printf("Nincs talalat\n");
    dintomb_free(&sor);
    printf("Nyomjon ENTERt hogy visszaterjen a fomenube");
    sor = sor_beker();
    dintomb_free(&sor);
    free(talalatok);
}




