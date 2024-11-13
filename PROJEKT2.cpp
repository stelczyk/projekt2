#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable:4996)


char* make_polish(const char* sentence)
{
    int rozmiar = 18;
    int* pl_txt = (int*)malloc(rozmiar * sizeof(int));
    int* pl_console = (int*)malloc(rozmiar * sizeof(int));

    if (pl_txt == NULL || pl_console == NULL) {
        printf("Błąd alokacji pamięci dla mapowania polskich znaków\n");
        free(pl_txt);
        free(pl_console);
        exit(2);
    }

    int temp_pl_txt[] = { -91, -58, -54, -93, -47, -45, -116, -113, -81, -71, -26, -22, -77, -15, -13, -100, -97, -65 };
    int temp_pl_console[] = { 164, 143, 168, 157, 227, 224, 151, 141, 189, 165, 134, 169, 136, 228, 162, 152, 171, 190 };

    for (int i = 0; i < rozmiar; i++) {
        pl_txt[i] = temp_pl_txt[i];
        pl_console[i] = temp_pl_console[i];
    }

    int length = strlen(sentence);
    char* sentence_pl = (char*)malloc((length + 1) * sizeof(char));

    if (sentence_pl == NULL)
    {
        printf("Błąd alokacji pamięci dla zmiennej sentence_pl\n");
        free(pl_txt);
        free(pl_console);
        exit(2);
    }

    for (int i = 0; i < length; i++)
    {
        sentence_pl[i] = sentence[i];
        for (int j = 0; j < rozmiar; j++)
        {
            if ((unsigned char)sentence[i] == (unsigned char)pl_txt[j])
            {
                sentence_pl[i] = (char)pl_console[j];
                break;
            }
        }
    }
    sentence_pl[length] = '\0';

    free(pl_txt);
    free(pl_console);
    return sentence_pl;
}



char* wczytaj_plik(const char* nazwa_pliku, int* dlugosc) {
    FILE* plik = fopen(nazwa_pliku, "r");
    if (plik == NULL) {
        printf("Nie można otworzyć pliku: %s\n", nazwa_pliku);
        return NULL;
    }

    int rozmiar = 1024;
    char* tekst = (char*)malloc(rozmiar * sizeof(char));
    if (tekst == NULL) {
        printf("Nie udało się zaalokować pamięci\n");
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
                printf("Nie udało się rozszerzyć pamięci\n");
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

    return strncmp(tekst1, tekst2, n);
}


void zapisz_wynik(const char* nazwa_pliku, const char* wynik) {
    FILE* plik = fopen(nazwa_pliku, "w");
    if (plik == NULL) {
        printf("Nie można otworzyć pliku do zapisu: %s\n", nazwa_pliku);
        return;
    }
    fprintf(plik, "%s", make_polish(wynik));
    fclose(plik);
}

int main() {
    const char* plik1 = "plik1.txt";
    const char* plik2 = "plik2.txt";
    const char* plik_wynikowy = "wynik.txt";
    int n = 10;

    int dlugosc1, dlugosc2;

    char* tekst1_raw = wczytaj_plik(plik1, &dlugosc1);
    char* tekst2_raw = wczytaj_plik(plik2, &dlugosc2);

    if (tekst1_raw == NULL || tekst2_raw == NULL) {
        printf("Błąd wczytywania plików.\n");
        free(tekst1_raw);
        free(tekst2_raw);
        return 1;
    }

    char* tekst1 = make_polish(tekst1_raw);
    if (tekst1 == NULL) {
        printf("Błąd alokacji pamięci dla tekstu 1.\n");
        free(tekst1_raw);
        free(tekst2_raw);
        free(tekst1);
        return 1;
    }
    char* tekst2 = make_polish(tekst2_raw);
    if (tekst2 == NULL) {
        printf("Błąd alokacji pamięci dla tekstu 2.\n");
        free(tekst1_raw);
        free(tekst2_raw);
        free(tekst2);
        return 1;
    }

    free(tekst1_raw);
    free(tekst2_raw);

    int wynik = porownaj_tekst(tekst1, tekst2, n);

    char wynik_tekst[256];
    if (wynik == 0) {
        snprintf(wynik_tekst, sizeof(wynik_tekst), "Pierwsze %d znaków tekstów jest identyczne\n", n);
    }
    else if (wynik > 0) {
        snprintf(wynik_tekst, sizeof(wynik_tekst), "Pierwsze %d znaków tekst1 jest większe od tekst2\n", n);
    }
    else {
        snprintf(wynik_tekst, sizeof(wynik_tekst), "Pierwsze %d znaków tekst1 jest mniejsze od tekst2\n", n);
    }


    printf("%s", make_polish(wynik_tekst));

    
    zapisz_wynik(plik_wynikowy, make_polish(wynik_tekst));

    free(tekst1);
    free(tekst2);
 

    return 0;
}
