#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)
#include <locale.h>

char* wczytaj_plik(const char* nazwa_pliku, int* dlugosc) {
    FILE* plik = fopen(nazwa_pliku, "r");
    if (plik == NULL) {
        printf("Nie mozna otworzyc pliku: %s\n", nazwa_pliku);
        return NULL;
    }

    int rozmiar = 1024;
    char* tekst = (char*)malloc(rozmiar * sizeof(char));
    if (tekst == NULL) {
        printf("nie udalo sie zaalokowaæ pamiêci\n");
        fclose(plik);
        return NULL;
    }

    int indeks = 0;
    char linia[256];
    while (fgets(linia, sizeof(linia), plik)) {
        int dlugosc_linii = strlen(linia);

        if (indeks + dlugosc_linii >= rozmiar) {
            rozmiar *= 2;
            char* nowy_tekst = realloc(tekst, rozmiar * sizeof(char));
            if (nowy_tekst == NULL) {
                printf("Nie uda³o sie rozszerzyc pamieci\n");
                free(tekst);
                fclose(plik);
                return NULL;
            }
            tekst = nowy_tekst;
        }
        strcpy(tekst + indeks, linia);
        indeks += dlugosc_linii;
    }
    tekst[indeks] = '\0';
    *dlugosc = indeks;

    fclose(plik);
    return tekst;
}

int porownaj_tekst(const char* tekst1, const char* tekst2, int n) {
    printf("%s\t%s\n", tekst1, tekst2);
    return strncmp(tekst1, tekst2, n);
    
}

void zapisz_wynik(const char* nazwa_pliku, const char* wynik) {
    FILE* plik = fopen(nazwa_pliku, "w");
    if (plik == NULL) {
        printf("Nie mozna otworzyc pliku do zapisu: %s\n", nazwa_pliku);
        return;
    }
    fprintf(plik, "%s", wynik); 
    fclose(plik);
}

int main() {
    setlocale(LC_ALL, "Polish_Poland.1250");
    const char* plik1 = "plik1.txt";
    const char* plik2 = "plik2.txt";
    const char* plik_wynikowy = "wynik.txt"; 
    int n = 10;

    int dlugosc1, dlugosc2;

    char* tekst1 = wczytaj_plik(plik1, &dlugosc1);
    char* tekst2 = wczytaj_plik(plik2, &dlugosc2);

    if (tekst1 == NULL || tekst2 == NULL) {
        printf("B³¹d wczytywania plików.\n");
        free(tekst1);
        free(tekst2);
        return 1;
    }

    int wynik = porownaj_tekst(tekst1, tekst2, n);


    const char* wynik_tekst = NULL;

    if (wynik == 0) {
        printf("Pierwsze %d znaków tekstów jest identyczne\n", n);
        wynik_tekst = "Pierwsze %d znaków tekstów jest identyczne\n";
    }
    else if (wynik > 0) {
        printf("Pierwsze %d znaków tekst1 jest mniejsze od tekst2\n", n);
        wynik_tekst = "Pierwsze %d znaków tekst1 jest mniejsze od tekst2\n";
    }
    else {
        printf("Pierwsze %d znaków tekst1 jest wiêksze od tekst2\n", n);
        wynik_tekst = "Pierwsze %d znaków tekst1 jest wiêksze od tekst2\n";
    }

    zapisz_wynik(plik_wynikowy, wynik_tekst);

    free(tekst1);
    free(tekst2);

    return 0;
}


