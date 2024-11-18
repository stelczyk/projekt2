
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
    char linia[1024];
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

        
        strncpy(tekst + indeks, linia, dlugosc_linii);
        indeks += dlugosc_linii;
    }
    tekst[indeks] = '\0';  

    *dlugosc = indeks;
    fclose(plik);
    return tekst;
}
int compare_words(char* w1, char* w2) {
    // 1. Jeżeli mamy ascii od 32 do 64 i od 91 do 96 i od 123 do 255 oraz <0  ma być na końcu
    // 2. Sprawdzić czy mam do czynienia z 2 dużymi jednym malym i duzym czy 2 malymi
    //
    // Jeżeli maly - duzy == 32 to sa tej samej klasy duzy > maly
    // Jezeli maly - duzy > 32 to duzy > maly
    // jezeli maly - duzy < 32 to maly > duzy

    char c1 = w1[0]; // "points" to the characters in w1
    char c2 = w2[0]; // the same but to w2

    int w1_len = strlen(w1);
    int w2_len = strlen(w2);

    int i = 1;
    while (c1 != 0 && c2 != 0) {
        if ((c1 >= 65 && c1 <= 90) && (c2 >= 97 && c2 <= 122)) {
            // c1 upper and c2 lower
            if (c2 - c1 >= 32) return 1; // 120 - 80 = 40 => w1 > w2
            if (c2 - c1 != 0) return -1;
        }
        else if ((c2 >= 65 && c2 <= 90) && (c1 >= 97 && c1 <= 122)) {
            // c1 lower and c2 upper
            if (c1 - c2 >= 32) return -1;
            if (c1 - c2 != 0) return 1;
        }
        else if ((c1 >= 65 && c1 <= 90) && (c2 >= 65 && c2 <= 90)) {
            if (c1 != c2) return c1 - c2 < 0 ? 1 : -1; // both upper
        }
        else if ((c1 >= 97 && c1 <= 122) && (c2 >= 97 && c2 <= 122)) {
            if (c1 != c2) return c1 - c2 < 0 ? 1 : -1; // both lower
        }
        else {
            if ((c1 >= 65 && c1 <= 90) || (c1 >= 97 && c1 <= 122)) return 1;  // c1 normal c2 weid
            if ((c2 >= 65 && c2 <= 90) || (c2 >= 97 && c2 <= 122)) return -1; // c1 weird c2 normal

            // c1 i c2 sa weird
            if (c1 != c2) return c1 - c2 < 0 ? 1 : -1;
        }

        c1 = w1[i];
        c2 = w2[i];
        i++;
    }

    return (w1_len == w2_len) ? 0 : (w1_len > w2_len) ? 1 : -1;
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
    int n;

    printf("Podaj liczbe n: ");
    scanf("%d", &n);

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
    int wynik = 0;
    for (int i = 0; i < n; i++) {
        if (compare_words(tekst1, tekst2) > 0) wynik = 1;
        else if (compare_words(tekst1, tekst2) < 0) wynik = -1;
    }
    

    char wynik_tekst[256];
    if (wynik == 0) {
        snprintf(wynik_tekst, sizeof(wynik_tekst), "Pierwsze %d znakow tekstów jest identyczne\n", n);
    }
    else if (wynik > 0) {
        snprintf(wynik_tekst, sizeof(wynik_tekst), "Pierwsze %d znakow tekst1 jest większe od tekst2\n", n);
    }
    else {
        snprintf(wynik_tekst, sizeof(wynik_tekst), "Pierwsze %d znakow tekst1 jest mniejsze od tekst2\n", n);
    }


    printf("%s", make_polish(wynik_tekst));


    zapisz_wynik(plik_wynikowy, make_polish(wynik_tekst));

    free(tekst1);
    free(tekst2);


    return 0;
}

