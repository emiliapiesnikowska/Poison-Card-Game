#include <time.h> 
#include<stdio.h>
#include <Windows.h>
#include <iostream>
#pragma warning(disable : 4996)
using namespace std;

#define ILOSC_GRACZY 20
#define KARTY 200
#define ILOSC_KOCIOLKOW 6

struct Karta {
    int wartosc = 0;
    int kolor = 0;
    int gracz = 0;
    bool ifhand = 0;
    int kociolek = 0;
    bool ifkociolek = 0;
};
struct Gracz {
    Karta* taliagracza = nullptr;
    int numergracza = 0;
    int ilosckartreka = 0;
    int ilosckartprzed = 0;
    int wynik = 0;
    int odpornoscna = 0;
};
struct Kociolek {
    Karta* kartywkociolku = nullptr;
    int wartosckociolka = 0;
    int numerkociolka = 0;
    int ilosckart = 0;
    int kolor = 0;
};


void utworzenietalii(int kociolki, Karta* karta, int* tablica, int bezzielonych, int ilpozostalych, int zielone, int wartosczielonych) {
    for (int i = 0; i < zielone; i++) {
        karta[i].wartosc = wartosczielonych;                                    //przypisanie wartosci i koloru 
        karta[i].kolor = 0;
    }

    for (int i = 0; i < kociolki; i++)
    {
        for (int j = ilpozostalych - (bezzielonych) / kociolki; j < ilpozostalych; j++)
        {
            karta[i * (bezzielonych / kociolki) + j - (ilpozostalych - bezzielonych / kociolki) + zielone].kolor = i + 1;
            karta[i * (bezzielonych / kociolki) + j - (ilpozostalych - bezzielonych / kociolki) + zielone].wartosc = tablica[j];
        }
    }

    for (int i = 0; i < kociolki * ilpozostalych + zielone; i++) {
        karta[i].ifhand = 1;                                    //podczas tworzenia talii każda z kart jest w rece gracza
    }
}



void rozdanietalii(Karta* karta, Gracz* gracz, int sumakart, int n) { //z tasowaniem
    for (int i = 0; i < n; i++) {
        gracz[i].taliagracza = new Karta[(sumakart / n) + sumakart % n + 1];
    }
    for (int i = 0; i < n; i++) {
        gracz[i].numergracza = i + 1;
    }

    bool* used = new bool[sumakart];
    for (int i = 0; i < sumakart; i++) {
        used[i] = 0;
    }

    int losowy = 0;
    for (int i = 0; i < sumakart; i++) {
        losowy = rand() % sumakart;

        while (used[losowy]) {
            losowy++;
            if (losowy == sumakart)
                losowy = 0;
        }

        karta[losowy].gracz = i % n;
        used[losowy] = true;
    }

    delete[] used;
}

void przypisaniekartdogracza(Karta* karta, Gracz* gracz, int n, int ilosckart) {
    for (int i = 0; i <= n; i++) {
        gracz[i].taliagracza = new Karta[ilosckart];
        gracz[i].numergracza = i + 1;
    }

    int licznik = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < ilosckart; j++) {
            if (karta[j].gracz == i && karta[j].ifkociolek == 0) {
                gracz[i].taliagracza[licznik] = karta[j];
                licznik++;
                if (karta[j].ifhand == 1) {
                    gracz[i].ilosckartreka++;
                }
                else if (karta[j].ifhand == 0)gracz[i].ilosckartprzed++;
            }
        }licznik = 0;
    }
}

const char* wypiszkolor(int kolor) {
    const char* nazwakoloru = "green";
    if (kolor == 0)nazwakoloru = "green";
    else if (kolor == 1)nazwakoloru = "blue";
    else if (kolor == 2)nazwakoloru = "red";
    else if (kolor == 3)nazwakoloru = "violet";
    else if (kolor == 4)nazwakoloru = "yellow";
    else if (kolor == 5)nazwakoloru = "white";
    else if (kolor == 6)nazwakoloru = "black";

    return nazwakoloru;
}


void zapisaniestanugry(FILE* plik, int activeplayer, int n, int explosion, int ilosckart, Karta* karta, int& k) {
    fprintf(plik, "active player = %d\n", activeplayer);
    fprintf(plik, "players number = %d\n", n);
    fprintf(plik, "explosion threshold = %d\n", explosion);
    for (int i = 0; i < n; i++) {
        if (i != 0)fprintf(plik, "\n");
        fprintf(plik, "%d player hand cards:", i + 1);
        for (int j = 0; j <= ilosckart; j++) {
            if (i == karta[j].gracz && karta[j].ifhand == 1 && karta[j].wartosc != 0) {
                fprintf(plik, " %d ", karta[j].wartosc);
                fprintf(plik, "%s", wypiszkolor(karta[j].kolor));
            }
        }
        fprintf(plik, "\n");
        fprintf(plik, "%d player deck cards:", i + 1);
        for (int j = 0; j <= ilosckart; j++) {
            if (i == karta[j].gracz && karta[j].ifhand == 0 && karta[j].ifkociolek != 1 && karta[j].wartosc != 0) {
                fprintf(plik, " %d ", karta[j].wartosc);
                fprintf(plik, "%s", wypiszkolor(karta[j].kolor));
            }
        }
    }
    for (int i = 0; i < k; i++) {
        fprintf(plik, "\n");
        fprintf(plik, "%d pile cards:", i + 1);
        for (int j = 0; j <= ilosckart; j++) {
            if (karta[j].ifkociolek == 1 && karta[j].kociolek == i && karta[j].wartosc != 0) {
                fprintf(plik, " %d ", karta[j].wartosc);
                fprintf(plik, "%s", wypiszkolor(karta[j].kolor));
            }
        }
    }
}


void przypisaniekart(Karta* karta, int ilosckart, int color, int playernumb, int wartosc, int nowalinia, int n) {

    karta[ilosckart].wartosc = wartosc;
    karta[ilosckart].kolor = color;
    karta[ilosckart].gracz = playernumb;
    if (nowalinia % 2 == 0 && nowalinia < 2 * n)karta[ilosckart].ifhand = 1;
    else if (nowalinia % 2 == 1 && nowalinia < 2 * n)karta[ilosckart].ifhand = 0;
    else if (nowalinia >= 2 * n) {
        karta[ilosckart].kociolek = nowalinia - 2 * n;
        karta[ilosckart].ifkociolek = 1;
    }
}


void wczytaniestanu(FILE* plik, Karta* karta, int& n, int& ilosckart, int& b, int& g, int& k, int& explosion, int& activeplayer) {

    if (plik != NULL) {
        int c, nowalinia = 0, playernumb = 0;
        int cyfra1 = 0, wartosc = 0, cyfra2 = -1, czyblue = 0;
        fscanf_s(plik, "active player = %d\n", &activeplayer);
        fscanf_s(plik, "players number = %d\n", &n);
        fscanf_s(plik, "explosion threshold = %d\n", &explosion);
        while ((c = fgetc(plik)) != EOF) {
            if (isdigit(c) && cyfra1 == 0) {
                cyfra1 = c - 48;
            }
            else if (isdigit(c) && cyfra1 != 0) {
                cyfra2 = c - 48;
            }
            else if (c == ' ' && cyfra1 != 0 && cyfra2 >= 0) {
                wartosc = 10 * cyfra1 + cyfra2;
                cyfra1 = 0; cyfra2 = -1;
            }
            else if (c == ' ' && cyfra1 != 0) {
                wartosc = cyfra1;
                cyfra1 = 0;
            }
            else if (c == 'g' && wartosc != 0) {
                przypisaniekart(karta, ilosckart, 0, playernumb, wartosc, nowalinia, n);
                ilosckart++; cyfra1 = 0; wartosc = 0; g++;
            }
            else if (c == 'r' && wartosc != 0) {
                przypisaniekart(karta, ilosckart, 2, playernumb, wartosc, nowalinia, n);
                ilosckart++; cyfra1 = 0; wartosc = 0;
            }
            else if (c == 'v' && wartosc != 0) {
                przypisaniekart(karta, ilosckart, 3, playernumb, wartosc, nowalinia, n);
                ilosckart++; cyfra1 = 0; wartosc = 0;
            }
            else if (c == 'y' && wartosc != 0) {
                przypisaniekart(karta, ilosckart, 4, playernumb, wartosc, nowalinia, n);
                ilosckart++; cyfra1 = 0; wartosc = 0;
            }
            else if (c == 'w' && wartosc != 0) {
                przypisaniekart(karta, ilosckart, 5, playernumb, wartosc, nowalinia, n);
                ilosckart++; cyfra1 = 0; wartosc = 0;
            }
            else if (c == 'b' && wartosc != 0)czyblue = 1;
            else if (c == 'l' && czyblue == 1)czyblue = 2;
            else if (c == 'u' && wartosc != 0 && czyblue == 2) {
                przypisaniekart(karta, ilosckart, 1, playernumb, wartosc, nowalinia, n);
                ilosckart++; cyfra1 = 0; wartosc = 0; b++;
            }
            else if (c == 'a' && wartosc != 0 && czyblue == 2) {
                przypisaniekart(karta, ilosckart, 6, playernumb, wartosc, nowalinia, n);
                ilosckart++; cyfra1 = 0; wartosc = 0;
            }
            else if (c == '\n') {
                nowalinia++;
                if (nowalinia % 2 == 0) {
                    playernumb++;
                }
                if (nowalinia >= 2 * n) k++;
            }
            else { wartosc = 0; czyblue = 0; }
        }
    }
}

void kartykociolek(Kociolek* kociolek, int ilosckart, int k, Karta* karta) {
    for (int i = 0; i < k; i++) {
        kociolek[i].kartywkociolku = new Karta[ilosckart + 1];
    }
    for (int i = 0; i < k; i++) {
        kociolek[i].numerkociolka = i + 1;
    }
    int licznik = 0;                                                                //licznik dla kart w każdym kociołku
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < ilosckart; j++) {
            if (karta[j].kociolek == i && karta[j].ifkociolek == 1) {
                kociolek[i].kartywkociolku[licznik] = karta[j];
                licznik++;
                if (karta[j].kolor != 0)kociolek[i].wartosckociolka += karta[j].wartosc;
                kociolek[i].ilosckart++;
            }
        }licznik = 0;
    }
}

void sprawdzeniekociolkow(Kociolek* kociolek, int explosion, int k, int& poprawnoscstanugry) {
    int poprawnosc = 2;
    for (int i = 0; i < k; i++) {

        for (int j = 1; j <= kociolek[i].ilosckart; j++) {
            if (kociolek[i].kartywkociolku[j - 1].kolor != kociolek[i].kartywkociolku[j].kolor) {
                if (kociolek[i].kartywkociolku[j - 1].kolor != 0 && kociolek[i].kartywkociolku[j].kolor != 0) {
                    cout << "Two different colors were found on the " << kociolek[i].numerkociolka << " pile" << endl;
                    poprawnosc--;
                }
                else if (kociolek[i].kartywkociolku[j - 1].kolor != 0) {
                    kociolek[i].kolor = kociolek[i].kartywkociolku[j - 1].kolor;
                }
                else if (kociolek[i].kartywkociolku[j].kolor != 0) {
                    kociolek[i].kolor = kociolek[i].kartywkociolku[j].kolor;
                }
            }
        }
    }
    for (int i = 0; i < k; i++) {
        if (kociolek[i].wartosckociolka >= explosion) {
            cout << "Pile number " << kociolek[i].numerkociolka << " should explode earlier" << endl;
            poprawnosc--;
        }
    }
    if (poprawnosc == 2)poprawnoscstanugry++;

}

void sprawdzamrozdaniekart(int& ilosckart, Karta* karta, Gracz* gracz, int aktywnygracz, int n, int& poprawnoscstanugry) {
    int kartyrozegrane = 0;
    int poprawnosc = 0;

    for (int i = 0; i < ilosckart; i++) {
        if (karta[i].ifkociolek == 1)kartyrozegrane++;
        else if (karta[i].ifkociolek == 0 && karta[i].ifhand == 0) kartyrozegrane++;
    }

    int* gracziloscreka;
    gracziloscreka = new int[n + 1];

    for (int i = 0; i < n; i++) {
        gracziloscreka[i] = gracz[i].ilosckartreka;
    }


    aktywnygracz--;
    while (kartyrozegrane > 0) {

        if (aktywnygracz >= 1) {
            aktywnygracz--;
            gracziloscreka[aktywnygracz]++;
            kartyrozegrane--;
        }
        else aktywnygracz = n;
    }
    int reszta = ilosckart % n;
    int ilosckartgracza = (ilosckart - reszta) / n;
    if (reszta > 0 && reszta < n) {
        for (int i = 0; i < reszta; i++) {
            if (gracziloscreka[i] == ilosckartgracza + 1)poprawnosc++;

        }
    }
    for (int i = reszta; i < n; i++) {
        if (gracziloscreka[i] == ilosckartgracza)poprawnosc++;

    }

    if (poprawnosc == n)poprawnoscstanugry++;
    else {
        cout << "The number of players cards on hand is wrong" << endl;

    }
    aktywnygracz++;
    delete[] gracziloscreka;
}

void posortowanatablica(int n, int* t) {    //posortowana rosnąco tablica pozostalych kolorow

    for (int pocz = 0; pocz < n - 1; pocz++)
        for (int i = 0; i < n - pocz - 1; i++)
            if (t[i] > t[i + 1]) {
                int p = t[i];
                t[i] = t[i + 1];
                t[i + 1] = p;
            }
}

void wpisaniedotablicy(int* tab, Karta* karta, int ilosckart, int kolor, int rozmiar) {
    int index = 0;

    for (int i = 0; i < ilosckart; i++) {
        if (karta[i].kolor == kolor) {
            tab[index] = karta[i].wartosc; index++;
        }
    }
    posortowanatablica(rozmiar, tab);
}

void sprawdzeniewartosciiiloscikart(int k, int ilosckart, Karta* karta, int& poprawnoscstanugry) {
    //sprawdzanie ilosci kart
    //____________________________________
    int* tab;
    tab = new int[k + 1];

    for (int i = 0; i <= k; i++) {
        tab[i] = 0;
    }

    for (int i = 0; i <= k; i++) {
        for (int z = 0; z < ilosckart; z++) {
            if (karta[z].kolor == i)tab[i]++;
        }
    }

    int czyrownailosc = 0;
    for (int i = 1; i <= k; i++) {
        if (tab[i - 1] == tab[i])czyrownailosc++;

    }

    //_________________________________________
    //sprawdzenie wartosci kart
    if (czyrownailosc >= k - 1) {

        int* b, * red, * violet, * yellow, * white, * black;
        int rozmiar = tab[1] + 1;
        b = new int[rozmiar]; red = new int[rozmiar]; violet = new int[rozmiar]; yellow = new int[rozmiar]; white = new int[rozmiar];
        black = new int[rozmiar];

        for (int i = 0; i < tab[1]; i++) {
            b[i] = 0; red[i] = 0; violet[i] = 0; yellow[i] = 0; white[i] = 0; black[i] = 0;
        }

        if (k >= 1) wpisaniedotablicy(b, karta, ilosckart, 1, tab[1]);
        else if (k >= 2 && tab[1] == tab[2]) wpisaniedotablicy(red, karta, ilosckart, 2, tab[2]);
        else if (k >= 3 && tab[3] == tab[2]) wpisaniedotablicy(violet, karta, ilosckart, 3, tab[3]);
        else if (k >= 4 && tab[4] == tab[3]) wpisaniedotablicy(yellow, karta, ilosckart, 4, tab[4]);
        else if (k >= 5 && tab[4] == tab[5]) wpisaniedotablicy(white, karta, ilosckart, 5, tab[5]);
        else if (k >= 6 && tab[5] == tab[6]) wpisaniedotablicy(black, karta, ilosckart, 6, tab[6]);

        int czyrownewartosci = 0;

        for (int j = 0; j < tab[1]; j++) {
            if (k >= 2) if (b[j] == red[j])czyrownewartosci++;
            if (k >= 3) if (violet[j] == red[j])czyrownewartosci++;
            if (k >= 4) if (violet[j] == yellow[j])czyrownewartosci++;
            if (k >= 5) if (white[j] == yellow[j])czyrownewartosci++;
            if (k >= 6) if (white[j] == black[j])czyrownewartosci++;
        }
        if (czyrownewartosci / (k - 2) == tab[1])poprawnoscstanugry++;

        delete[] b;
        delete[] red;
        delete[] violet;
        delete[] yellow;
        delete[] white;
        delete[] black;
    }
    else cout << "ilosc kart nie jest rowna" << endl;

    delete[] tab;
}

void wybuch(Gracz* gracz, int n, Kociolek* kociolek, int activeplayer, int& ilosckart, Karta* karta, int i) {
    int numerkarty = 0;
    for (int h = 0; h < n; h++) {
        numerkarty = numerkarty + gracz[h].ilosckartreka + gracz[h].ilosckartprzed;

    }
    for (int h = 0; h < i; h++) {
        numerkarty += kociolek[h].ilosckart;
    }

    for (int z = 0; z <= kociolek[i].ilosckart - 1; z++) {
        gracz[activeplayer - 1].ilosckartprzed++;

        karta[ilosckart].kolor = kociolek[i].kartywkociolku[z].kolor;
        karta[ilosckart].wartosc = kociolek[i].kartywkociolku[z].wartosc;
        karta[ilosckart].ifkociolek = 0;
        karta[ilosckart].ifhand = 0;
        karta[ilosckart].gracz = activeplayer - 1;
        gracz[activeplayer - 1].taliagracza[gracz[activeplayer - 1].ilosckartreka + gracz[activeplayer - 1].ilosckartprzed] = karta[ilosckart];
        ilosckart++;

        if (kociolek[i].numerkociolka - 1 == karta[numerkarty].kociolek) {
            karta[numerkarty].wartosc = 0;
            karta[numerkarty].ifkociolek = 0;
            karta[numerkarty].ifhand = 0;
            karta[numerkarty].kolor = 0;
            karta[numerkarty].kociolek = 0;
        }
        numerkarty++;
    }
    kociolek[i].wartosckociolka = 0;
    kociolek[i].kolor = 0;
    kociolek[i].ilosckart = 0;
}

void wybornajmniejszejkartyikociolka(Gracz* gracz, int aktywnygracz, int k, Kociolek* kociolek, Karta* karta, int& numerkociolka, int& index, int explosion) {
    int* wartosci;
    wartosci = new int[gracz[aktywnygracz - 1].ilosckartreka];

    for (int i = 0; i < gracz[aktywnygracz - 1].ilosckartreka; i++) {
        wartosci[i] = gracz[aktywnygracz - 1].taliagracza[i].wartosc;
    }
    posortowanatablica(gracz[aktywnygracz - 1].ilosckartreka, wartosci);

    int iloscnajmniejszych = 0;
    for (int i = 0; i < gracz[aktywnygracz - 1].ilosckartreka; i++) {
        if (gracz[aktywnygracz - 1].taliagracza[i].wartosc == wartosci[0])iloscnajmniejszych++;
    }

    int* kolorynajmniejszych;
    kolorynajmniejszych = new int[iloscnajmniejszych];
    int j = 0;
    for (int i = 0; i < gracz[aktywnygracz - 1].ilosckartreka; i++) {
        if (gracz[aktywnygracz - 1].taliagracza[i].wartosc == wartosci[0]) {
            kolorynajmniejszych[j] = gracz[aktywnygracz - 1].taliagracza[i].kolor;
            j++;
        }
    }
    int najmniejszawartosc = wartosci[0];

    int* wartoscikociolka;
    wartoscikociolka = new int[k];

    for (int i = 0; i < k; i++) {
        wartoscikociolka[i] = kociolek[i].wartosckociolka;
    }
    posortowanatablica(k, wartoscikociolka);


    int kolor = 1;
    int czyistnieje = 0;

    for (int j = 1; j <= k; j++) {
        for (int i = 0; i < k; i++) {
            if (kociolek[i].kolor == j)czyistnieje = 1;
        }
        if (czyistnieje == 0) {
            for (int i = 0; i < k; i++) {
                if (kociolek[i].kolor == 0) {
                    kociolek[i].kolor = j;
                    break;
                }
            }
        }
        czyistnieje = 0;
    }


    int pile = 0;
    for (int z = 0; z < k; z++) {
        for (int i = 0; i < k; i++) {
            if (kociolek[i].wartosckociolka == wartoscikociolka[z]) {
                for (int j = 0; j < iloscnajmniejszych; j++) {
                    if (kociolek[i].kolor == kolorynajmniejszych[j] && kolorynajmniejszych[j] != 0) {
                        numerkociolka = i; pile = 1;
                        break;
                    }
                    else if (kolorynajmniejszych[j] == 0) {
                        if (kociolek[i].wartosckociolka == wartoscikociolka[0]) {
                            numerkociolka = i; pile = 1;
                            break;
                        }
                    }
                }
            }
            //      else if(kociolek[i].wartosckociolka == wartoscikociolka[pile] && kociolek[i].wartosckociolka == 0)

        }
        if (pile == 1)break;
    }

    for (int i = 0; i < gracz[aktywnygracz - 1].ilosckartreka; i++) {
        if (gracz[aktywnygracz - 1].taliagracza[i].kolor == kociolek[numerkociolka].kolor && gracz[aktywnygracz - 1].taliagracza[i].wartosc == najmniejszawartosc) {
            index = i;
            break;
        }
        else if (gracz[aktywnygracz - 1].taliagracza[i].kolor == 0 && gracz[aktywnygracz - 1].taliagracza[i].wartosc == najmniejszawartosc) {
            index = i;
            break;
        }
    }

    //wybor najwiekszej jesli najmniejsza powoduje wybuch

    bool czynajmniejsza = 1;
    if (kociolek[numerkociolka].wartosckociolka + gracz[aktywnygracz - 1].taliagracza[index].wartosc >= explosion) {
        czynajmniejsza = 0;

        int najwieksza = wartosci[gracz[aktywnygracz - 1].ilosckartreka - 1];

        int* kolorynajwiekszych;
        kolorynajwiekszych = new int[iloscnajmniejszych];
        int iloscnajwiekszych = 0;
        for (int i = gracz[aktywnygracz - 1].ilosckartreka - 1; i >= 0; i--) {
            if (gracz[aktywnygracz - 1].taliagracza[i].wartosc == najwieksza) {
                kolorynajwiekszych[iloscnajwiekszych] = gracz[aktywnygracz - 1].taliagracza[i].kolor;
                iloscnajwiekszych++;
            }
        }

        int pile = 0;
        for (int z = 0; z < k; z++) {
            for (int i = 0; i < k; i++) {
                if (kociolek[i].wartosckociolka == wartoscikociolka[z]) {
                    for (int j = 0; j < iloscnajwiekszych; j++) {
                        if (kociolek[i].kolor == kolorynajwiekszych[j] && kolorynajwiekszych[j] != 0) {
                            numerkociolka = i; pile = 1;
                            break;
                        }
                        else if (kolorynajwiekszych[j] == 0) {
                            if (kociolek[i].wartosckociolka == najwieksza) {
                                numerkociolka = i; pile = 1;
                                break;
                            }
                        }
                    }
                }
                //      else if(kociolek[i].wartosckociolka == wartoscikociolka[pile] && kociolek[i].wartosckociolka == 0)

            }
            if (pile == 1)break;
        }
        for (int i = 0; i < gracz[aktywnygracz - 1].ilosckartreka; i++) {
            if (gracz[aktywnygracz - 1].taliagracza[i].kolor == kociolek[numerkociolka].kolor && gracz[aktywnygracz - 1].taliagracza[i].wartosc == najwieksza) {
                index = i;
                break;
            }
            else if (gracz[aktywnygracz - 1].taliagracza[i].kolor == 0 && gracz[aktywnygracz - 1].taliagracza[i].wartosc == najwieksza) {
                index = i;
                break;
            }
        }

        bool czyoptymalna = 0;
        if (kociolek[numerkociolka].wartosckociolka + gracz[aktywnygracz - 1].taliagracza[index].wartosc >= explosion) {

            for (int z = gracz[aktywnygracz - 1].ilosckartreka - 1; z > 0; z--) {
                for (int i = 0; i < gracz[aktywnygracz - 1].ilosckartreka; i++) {
                    if (wartosci[z] == gracz[aktywnygracz - 1].taliagracza[i].wartosc) {
                        for (int j = 0; j < k; j++) {
                            if (gracz[aktywnygracz - 1].taliagracza[i].kolor == kociolek[j].kolor) {
                                if (kociolek[j].wartosckociolka + gracz[aktywnygracz - 1].taliagracza[i].wartosc < explosion) {
                                    numerkociolka = j;
                                    index = i;
                                    czyoptymalna = 1;
                                }
                            }
                            if (czyoptymalna == 1)break;
                        }
                    }
                }
            }
        }
        if (czyoptymalna == 0)cout << "WYBRANO NAJWIEKSZA KARTE" << endl;
        if (czyoptymalna == 1)cout << "WYBRANO OPTYMALNA KARTE" << endl;

    }
    if (czynajmniejsza == 1)cout << "WYBRANO NAJMNIEJSZA KARTE" << endl;

    //cout << kociolek[numerkociolka].wartosckociolka << " " << gracz[aktywnygracz - 1].taliagracza[index].wartosc << " " << explosion << endl;
    //cout << index << numerkociolka;




    delete[] wartosci;
    delete[] kolorynajmniejszych;
    delete[] wartoscikociolka;
}

void ruch(Gracz* gracz, Karta* karta, Kociolek* kociolek, int& ilosckart, int& activeplayer, int& k, int& n, int explosion) {
    int index = 0; int numerkociolka = 0;
    wybornajmniejszejkartyikociolka(gracz, activeplayer, k, kociolek, karta, numerkociolka, index, explosion);

    //sprawdzenie kociolka
    if (gracz[activeplayer - 1].taliagracza[index].kolor != kociolek[numerkociolka].kolor && gracz[activeplayer - 1].taliagracza[index].kolor != 0) {
        for (int i = 0; i > k; k++) {
            if (kociolek[i].kolor == gracz[activeplayer - 1].taliagracza[index].kolor)
                numerkociolka = i;
        }
    }

    if (gracz[activeplayer - 1].ilosckartreka > 0) {
        int numerkarty = index;
        int kociolekdowybuchu = -1;
        for (int h = 0; h < activeplayer - 1; h++) {
            numerkarty += gracz[h].ilosckartprzed + gracz[h].ilosckartreka;
        }

        kociolek[numerkociolka].ilosckart++; ilosckart++;
        karta[ilosckart].kolor = gracz[activeplayer - 1].taliagracza[index].kolor;
        karta[ilosckart].wartosc = gracz[activeplayer - 1].taliagracza[index].wartosc;
        karta[ilosckart].ifkociolek = 1;
        karta[ilosckart].kociolek = numerkociolka;
        if (gracz[activeplayer - 1].taliagracza[index].kolor != 0)
            kociolek[numerkociolka].wartosckociolka += karta[ilosckart].wartosc;
        kociolek[numerkociolka].kartywkociolku[kociolek[numerkociolka].ilosckart - 1] = karta[ilosckart];


        if (kociolek[numerkociolka].wartosckociolka >= explosion)
            kociolekdowybuchu = numerkociolka;

        for (int z = numerkarty; z < ilosckart; z++) {
            karta[z].kolor = karta[z + 1].kolor;
            karta[z].wartosc = karta[z + 1].wartosc;
            karta[z].gracz = karta[z + 1].gracz;
            karta[z].ifhand = karta[z + 1].ifhand;
            karta[z].ifkociolek = karta[z + 1].ifkociolek;
            karta[z].kociolek = karta[z + 1].kociolek;
        }
        karta[ilosckart].wartosc = 0; ilosckart--; gracz[activeplayer - 1].ilosckartreka--;

        //cout << "do rozegrania" << karta[ilosckart].wartosc << endl;

        if (kociolekdowybuchu >= 0) {
            wybuch(gracz, n, kociolek, activeplayer, ilosckart, karta, kociolekdowybuchu);
        }
        ilosckart++;
        karta[ilosckart].ifhand = 0;
        karta[ilosckart].ifkociolek = 0;
        karta[ilosckart].gracz = activeplayer - 1;

    }

    activeplayer++;
    if (activeplayer > n) activeplayer = 1;


}


void odpornosc(int* tab, int rozmiar, Gracz* gracz, int kolor) {
    int max = tab[0], index = 0, powtorka = 0;
    for (int i = 0; i < rozmiar; i++) {
        if (max < tab[i]) {
            max = tab[i];
            index = i;
            powtorka = 0;
        }
        else if (max == tab[i] && i > 0) {
            powtorka++;
        }
    }
    if (powtorka > 0)index = -1;

    if (index > -1) {
        gracz[index].odpornoscna = kolor;
        gracz[index].wynik -= tab[index];
        cout << "Na kolor " << wypiszkolor(kolor) << " odporny jest gracz " << gracz[index].numergracza << endl;
    }
}

void koniecrundy(int n, Gracz* gracz, int& poprawnoscstanugry) {
    int* red, * b, * violet, * yellow, * white, * black;
    red = new int[n]; b = new int[n]; violet = new int[n]; yellow = new int[n]; white = new int[n]; black = new int[n];
    int koniecrundy = 0;


    for (int h = 0; h < n; h++) {
        if (gracz[h].ilosckartreka != 0)koniecrundy++;
        red[h] = 0; b[h] = 0; violet[h] = 0; yellow[h] = 0; white[h] = 0; black[h] = 0;
    }
    if (koniecrundy == 0) {
        for (int h = 0; h < n; h++) {
            for (int z = gracz[h].ilosckartreka; z < gracz[h].ilosckartprzed; z++) {
                if (gracz[h].taliagracza[z].kolor == 0)
                    gracz[h].wynik += 2;
                else if (gracz[h].taliagracza[z].kolor == 1) {
                    b[h]++; gracz[h].wynik++;
                }
                else if (gracz[h].taliagracza[z].kolor == 2) {
                    red[h]++;       gracz[h].wynik++;
                }
                else if (gracz[h].taliagracza[z].kolor == 3) {
                    violet[h]++;    gracz[h].wynik++;
                }
                else if (gracz[h].taliagracza[z].kolor == 4) {
                    yellow[h]++;    gracz[h].wynik++;
                }
                else if (gracz[h].taliagracza[z].kolor == 5) {
                    white[h]++;     gracz[h].wynik++;
                }
                else if (gracz[h].taliagracza[z].kolor == 6) {
                    black[h]++;     gracz[h].wynik++;
                }
            }

        }
        odpornosc(b, n, gracz, 1);
        odpornosc(red, n, gracz, 2);
        odpornosc(violet, n, gracz, 3);
        odpornosc(yellow, n, gracz, 4);
        odpornosc(white, n, gracz, 5);
        odpornosc(black, n, gracz, 6);


        for (int h = 0; h < n; h++) {
            cout << "Wynik gracza " << h + 1 << " = " << gracz[h].wynik << endl;
        }

    }
    else poprawnoscstanugry++;

    delete[] b;
    delete[] red;
    delete[] violet;
    delete[] yellow;
    delete[] white;
    delete[] black;

}

int main()
{
    int n = 0, k = 0, g = 0;
    int explosion = 0, activeplayer;
    Karta* karta;
    Gracz* gracz;
    Kociolek* kociolek;
    kociolek = new Kociolek[ILOSC_KOCIOLKOW + 1];
    karta = new Karta[KARTY];
    gracz = new Gracz[ILOSC_GRACZY + 1];



    FILE* plik, * plik2;
    plik2 = fopen("plik.txt", "r+");
    plik = fopen("stangry.txt", "r+");





    cout << "Czy chcesz podac nowe dane do gry?" << endl << "1 - TAK, 2 - NIE" << endl;
    int tryb;
    cin >> tryb;

    if (tryb == 1) {
        int* tablica;
        int n, k, g, gv, o;
        activeplayer = 1;
        cout << "Podaj liczbe graczy: "; cin >> n;
        cout << "Podaj liczbe kociolkow: "; cin >> k;
        cout << "Podaj ilosc zielonych kart: "; cin >> g;
        cout << "Podaj wartosc zielonych kart: "; cin >> gv;
        cout << "Podaj ilosc pozostalych kart: "; cin >> o;
        cout << "Podaj wartosci pozostalych kart: ";
        tablica = new int[o + 1];
        for (int i = 0; i < o; i++) cin >> tablica[i];


        cout << "Podaj wartosc eksplozji: ";
        cin >> explosion;

        utworzenietalii(k, karta, tablica, k * o, o, g, gv);
        rozdanietalii(karta, gracz, g + k * o, n);
        przypisaniekartdogracza(karta, gracz, n, g + k * o);
        zapisaniestanugry(plik, activeplayer, n, explosion, g + k * o, karta, k);
        delete[] tablica;
    }
    else {
        int blue = 0;
        int poprawnoscstanugry = 0;
        int ilosckart = 0;
        //przygotowanie danych do gry, rozdanie kart, umiejscowienie kart w kociolku

        wczytaniestanu(plik, karta, n, ilosckart, blue, g, k, explosion, activeplayer);
        kartykociolek(kociolek, ilosckart, k, karta);
        przypisaniekartdogracza(karta, gracz, n, ilosckart);

        //sprawdzeniestanu gry
        sprawdzeniewartosciiiloscikart(k, ilosckart, karta, poprawnoscstanugry);
        sprawdzamrozdaniekart(ilosckart, karta, gracz, activeplayer, n, poprawnoscstanugry);
        sprawdzeniekociolkow(kociolek, explosion, k, poprawnoscstanugry);
        koniecrundy(n, gracz, poprawnoscstanugry);



        if (poprawnoscstanugry == 4) {
            ruch(gracz, karta, kociolek, ilosckart, activeplayer, k, n, explosion);
            cout << "Ruch wykonany poprawnie" << endl;
            zapisaniestanugry(plik2, activeplayer, n, explosion, ilosckart, karta, k);
        }
    }




    delete[] gracz;
    delete[] kociolek;
    delete[] karta;

}


/*
printf("active player = %d\n", activeplayer);
                printf("players number = %d\n", n);
                printf("explosion threshold = %d\n", explosion);
                for (int i = 0; i < n; i++) {
                        if (i != 0)printf("\n");
                        printf("%d player hand cards:", i + 1);
                        for (int j = 0; j <= ilosckart; j++) {
                                if (i == karta[j].gracz && karta[j].ifhand == 1 && karta[j].wartosc != 0) {
                                        printf(" %d ", karta[j].wartosc);
                                        printf("%s", wypiszkolor(karta[j].kolor));
                                }
                        }
                        printf("\n");
                        printf("%d player deck cards:", i + 1);
                        for (int j = 0; j <= ilosckart; j++) {
                                if (i == karta[j].gracz && karta[j].ifhand == 0 && karta[j].ifkociolek != 1 && karta[j].wartosc != 0) {
                                        printf(" %d ", karta[j].wartosc);
                                        printf("%s", wypiszkolor(karta[j].kolor));
                                }
                        }

                }
                for (int i = 0; i < k; i++) {
                        printf("\n");
                        printf("%d pile cards:", i + 1);
                        for (int j = 0; j <= ilosckart; j++) {
                                if (karta[j].ifkociolek == 1 && karta[j].kociolek == i && karta[j].wartosc != 0) {
                                        printf(" %d ", karta[j].wartosc);
                                        printf("%s", wypiszkolor(karta[j].kolor));
                                }

                        }

                }


*/