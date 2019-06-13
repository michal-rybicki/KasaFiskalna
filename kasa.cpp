#include <fstream> //do obslugi plikow
#include <iostream> // do strumienia wejscia/wyjscia
#include <iomanip> //do wyswietlania z zaokrogleniem
#include <cstdlib> //do czyszczenia ekranu
#include <conio.h> //do sterowania klawiszami
#include <cctype> //do analizy znakow wprowadzonych przez uzytkownika
#include "towar.hpp"

using namespace std;

//---KLASA KASA---------------------------------------------------------------------------------------------------------------------------
class kasa
{

private:
     //Pola:
     towar* listaTowarow;
     double suma; //aktualna suma wartosci zliczonych towarow
     int n; //ilosc towarow w bazie danych;
     waga wagaSklepowa; //pobiera odczyt z wagi

public:
    //Metody:
    kasa(); //konstruktor
    void menuZliczania(); //zlicza cene podawanych przez uzytkownika towarow - przechwytuje poszczegolne klawisze
    void zliczanie(); //zlicza cene podawanych przez uzytkownika towarow - korzysta ze strumienia cin
private:
    void naglowek(int waga=1); //wyswietla naglowek z aktualnie zliczona sumaryczna cena
    int menu(); //menu opcji zwraca 1 jezeli uzytnownik chce wyjsc, inaczej zwraca 0;
    int menuUsuwania(); //menu usuwania zwraca numer towaru na liscie, ktory ma byc usuniety
    void menuDodawania(); //menu dodawania nowego towaru do bazy
    void wyswietlTowar(towar towar1); //wyswietla wiersz z informacjami o towarze1
    void podpowiedzi(int idSzukane); //wyswietla liste produktow o id zaczynajacym sie jak idSzukane i dluzszym o jeden znak
    void sortuj(int l=0, int p=-1); //sortuje towary na liscie towarow rosnaco wedlug id
    void zamien(towar& x,towar& y); //potrzebne do sortowania
    void wczytajPlik();//operacje na pliku z baza towarow
    void zapiszPlik();//operacje na pliku z baza towarow
    void usunTowar(int i); //usuwa towar z listy i pliku
    void dodajTowar(towar nowyTowar); //dodaje towar do listy i do pliku;
    void przegladTowarow(); //wyswietla liste wszystkich towarow w bazie
};

//---Konstruktor-----------------------------------------------------------------------------------------------------------------------------
kasa::kasa()
{
    //Wstepne zerowanie wyniku
    suma=0;

    //wczytywanie towarow z pliku baza.txt
    wczytajPlik();
    zapiszPlik();

    //Ustawienie wyswietlania liczb z zaokragleniem do 2 miejsc po przecinku
    cout << fixed << setprecision(2);
}

//---Zliczanie----------------------------------------------------------------------------------------------------------------------------------
void kasa::menuZliczania()
{
    //FLAGI I ZMIENNE TYMCZASOWE
    int wyjscie=0;
    //0 - program dziala w nieskonczonej petli
    //1 - program konczy sie
    int tryb=1;
    //1 - wpisywanie id przez uzytkownika
    //2 - wpisywanie ilosci przez uzytkownika
    int blad=0; //przechowuje informacje o bledach znalezionych podczas przeszukiwania bazy towarow
    //0 - brak bledow
    //1 - nie znaleziono towaru w bazie
    //2 - wartosc ma za duzo znakow
    int id=0; //id wpisywane przez uzytkownika
    double ilosc=0; //ilosc wpisywana przez uzytkownika
    int przecinek=0; //pamieta ilosc cyfr po przecinku we wprowadzanej ilosci powiekszona o 1
    bool poPrzecinku=false; //pamieta czy podajemy juz ilosc po przecinku
    char wybor; //informacja o klawiszu wcisnietym przez uzytkownika
    int i=0; //licznik
    int dlugosc=0; //pamieta dlugosc wprowadzonej wartosci
    int z=0; //przechowuje prosbe o zerowanie zmiennych tymczasowych na koncu petli glownej metody

    do
    {
        system( "cls" ); //czyszczenie ekranu

        //WPROWADZENIE KODU PRODUKTU PRZEZ UZYTKOWNIKA
        if (tryb==1)
        {
            naglowek(); //wyswietlanie menu i aktualnej sumy

            //wyswietlanie bledu wyszukiwania z poprzedniego obrotu petli
            if (blad==1)
            {
                cout << "Brak towaru w bazie." << endl;
                podpowiedzi(id);
                cout << endl;
                blad=0; //zerowanie bledu
                id=0; //zerowanie id;
                dlugosc=0;
            }
            else if (blad==2)
            {
                cout << "Kod za dlugi." << endl;
                cout << endl;
                blad=0; //zerowanie bledu
                id=0; //zerowanie id;
                dlugosc=0;
            }

            //Zapytanie o wprowadzenie id produktu
            cout << "Podaj identyfikator produktu:" << endl;

            //Wyswietlenie wprowadzonego dotychczas id
            if (id>0) cout << id;

            //Pobranie przycisku wduszonego przez uzytkownika
            wybor=getch();

            //Reakcja tylko na cyfry i klawisze specjalne
            if (isdigit(wybor))
            {
                char * cyfra;
                cyfra = new char[2];
                cyfra[0]=wybor;
                cyfra[1]='k';
                id=id*10+atoi(cyfra);
                delete [] cyfra;
                dlugosc++;
                if (dlugosc==4)
                {
                    blad=2;
                }
            }
            else if ((wybor=='e' || wybor=='E') && dlugosc>0)
            {
                i=0;
                while(tryb==1 && i<n)
                {
                    if (listaTowarow[i].id==id)
                    {
                        tryb=2;
                        i--;
                    }
                    i++;
                }
                if (i==n)
                {
                    blad=1;
                }
                dlugosc=0;
            }
        }

        //WPROWADZANIE ILOSCI PRZEZ UZYTKOWNIKA
        else if (tryb==2)
        {
            naglowek(listaTowarow[i].typ); //wyswietlanie menu, aktualnej sumy i ewentualnie odczytu z wagi

            //wyswietlanie bledu wpisywania z poprzedniego obrotu petli
            if (blad==2)
            {
                cout << "Wartosc za dluga." << endl;
                cout << endl;
                blad=0; //zerowanie bledu
                ilosc=0; //zerowanie ilosci;
                przecinek=0;
                poPrzecinku=false;
                dlugosc=0;
            }

            wyswietlTowar(listaTowarow[i]); //kod i nazwa towaru
            cout << endl;

            if (listaTowarow[i].typ==0)
                {
                    cout << "Podaj ilosc [kg]: "<< endl;
                }
            else
                {
                    cout << "Podaj ilosc [szt.]: " << endl;
                }

            //Wyswietlenie wprowadzonej dotychczas ilosci
            if (ilosc>0)
            {
                if (przecinek>0)
                {
                    double iloscZPrzecinkiem=ilosc;
                    for (int j=0; j<przecinek; j++)
                    {
                        iloscZPrzecinkiem=iloscZPrzecinkiem/10;
                    }
                    cout << fixed << setprecision(przecinek) << iloscZPrzecinkiem;
                }
                else
                {
                    cout << fixed << setprecision(0) << ilosc;
                    if (poPrzecinku && przecinek==0) cout << ".";
                }
                cout << fixed << setprecision(2);

            }

            //Pobranie przycisku wduszonego przez uzytkownika
            wybor=getch();

            //Reakcja tylko na cyfry, znaki '.' oraz ',' i klawisze specjalne
            if (isdigit(wybor))
            {
                if (poPrzecinku) przecinek++;
                char * cyfra;
                cyfra = new char[2];
                cyfra[0]=wybor;
                cyfra[1]='k';
                ilosc=ilosc*10+atoi(cyfra);
                delete [] cyfra;
                dlugosc++;
                if (dlugosc==10)
                {
                    blad=2;
                }
            }
            else if ((wybor==',' || wybor=='.') && !poPrzecinku)
            {
                poPrzecinku=true;

            }
            //koniec wprowadzania nowego towaru do sumy
            else if (wybor=='e' || wybor=='E')
            {
                if (przecinek > 0)
                {
                    double iloscZPrzecinkiem=ilosc;
                    for (int j=0; j<przecinek; j++)
                    {
                        iloscZPrzecinkiem=iloscZPrzecinkiem/10;
                    }
                    suma = suma + listaTowarow[i] * iloscZPrzecinkiem;
                }
                else
                {
                    suma = suma + listaTowarow[i] * ilosc;
                }
                z=1;//prosba o zerowanie zmiennych pomocniczych na koncu petli
            }
            else if ((wybor=='w' || wybor=='W') && listaTowarow[i].typ==0)
            {

                suma = suma + listaTowarow[i] * wagaSklepowa.odczyt;

                z=1;//prosba o zerowanie zmiennych pomocniczych na koncu petli
            }
        }

        //Sprawdzenie czy uzytkownik nie chce obsluzyc nowego klienta
        if (wybor=='n' || wybor=='N')
            {
                suma=0;//zerowanie sumy pod nowego klienta
                z=1;//prosba o zerowanie zmiennych pomocniczych na koncu petli
            }

        //Sprawdzenie czy uzytkownik nie chce wyjsc do menu
        else if (wybor=='m' || wybor=='M')
        {
            wyjscie=menu();
            z=1;//prosba o zerowanie zmiennych pomocniczych na koncu petli

        }

        //zerowanie zmiennych pobierania wartosci od uzytkonika
        if (z==1)
        {
                //ustawienie trybu na zapytanie o kod towaru
                tryb=1;

                //zmienne do obslugi id
                id=0;

                //zmienne do obslugi ilosci
                przecinek=0;
                poPrzecinku=false;
                ilosc=0;

                //zmienne do obslugi ilosci i id
                dlugosc=0;

                z=0;
        }


    }
    while(wyjscie==0);

}

//---menu opcji-----------------------------------------------------------------------------------------------------------------------------
int kasa::menu()
{
    //FLAGI I ZMIENNE TYMCZASOWE
    int wyjscie=0;
    char wybor;

    while (wyjscie==0)
    {
        system( "cls" ); //czyszczenie ekranu

        //OPCJE MENU
        cout << "e - powrot" << endl;
        cout << "u - usuwanie produktu z bazy" << endl;
        cout << "d - dodawanie produktu do bazy" << endl;
        cout << "w - wyswietl wszystkie towary" << endl;
        cout << "x - wyjscie" << endl;

        //Pobranie przycisku wduszonego przez uzytkownika
        wybor=getch();


        if (wybor=='x' || wybor=='X')
        {
            return 1;
        }
        else if (wybor=='e' || wybor=='E')
        {
            wyjscie=1;
        }
        else if (wybor=='u' || wybor=='U')
        {
            usunTowar(menuUsuwania());
        }
        else if (wybor=='d' || wybor=='D')
        {
            menuDodawania();
        }
        else if (wybor=='w' || wybor=='W')
        {
            przegladTowarow();
        }
    }
    return 0;
}

//---menu wyboru towaru do usuniecia-----------------------------------------------------------------------------------------------
int kasa::menuUsuwania()
{
    //FLAGI I ZMIENNE TYMCZASOWE
    int wyjscie=0; //steruje glowna petla metody
    char wybor; //informacja o wciskanym przez usytkownika klawiszu
    int blad=0; //informacja o bledach
    //0 - brak bledow
    //1 - nie znaleziono towaru w bazie
    //2 - wartosc ma za duzo znakow
    int id=0; //id wpisywane przez uzytkownika
    int dlugosc=0; //dlugosc wprowadzanego id

    while (wyjscie==0)
    {
        system( "cls" ); //czyszczenie ekranu

        //OPCJE MENU
        cout << "c - anuluj" << endl;
        cout << "e - potwierdz" << endl << endl;

        //wyswietlanie bledu wyszukiwania z poprzedniego obrotu petli
        if (blad==1)
        {
            cout << "Brak towaru w bazie." << endl;
            podpowiedzi(id);
            cout << endl;
            blad=0; //zerowanie bledu
            id=0; //zerowanie id;
            dlugosc=0;
        }
        else if (blad==2)
        {
            cout << "Kod za dlugi." << endl;
            cout << endl;
            blad=0; //zerowanie bledu
            id=0; //zerowanie id;
            dlugosc=0;
        }

        //Zapytanie o wprowadzenie id produktu
        cout << "Podaj identyfikator produktu do usuniecia z bazy:" << endl;

        //Wyswietlenie wprowadzonego dotychczas id
        if (id>0) cout << id;

        //Pobranie przycisku wduszonego przez uzytkownika
        wybor=getch();

        //Reakcja tylko na cyfry i klawisze specjalne
        if (isdigit(wybor))
        {
            char * cyfra;
            cyfra = new char[2];
            cyfra[0]=wybor;
            cyfra[1]='k';
            id=id*10+atoi(cyfra);
            delete [] cyfra;
            dlugosc++;
            if (dlugosc==4)
            {
                blad=2;
            }
        }
        else if ((wybor=='e' || wybor=='E') && dlugosc>0)
        {
            int i=0;
            while(i<n)
            {
                if (listaTowarow[i].id==id)
                {
                    return i;
                }
                i++;
            }
            if (i==n)
            {
                blad=1;
            }
            dlugosc=0;
        }

        //sprawdzenie czy uzytkownik nie chce anulowac
        else if (wybor=='c' || wybor=='C')
        {
            wyjscie=1;
        }
    }
    return -1;
}

//---Menu dodawania nowego towaru do bazy---------------------------------------------------------------------------------------------------

void kasa::menuDodawania()
{
    //FLAGI I ZMIENNE TYMCZASOWE
    int wyjscie=0;
    //0 - menu dziala w nieskonczonej petli
    int tryb=1;
    //1 - wpisywanie id przez uzytkownika
    //2 - wpisywanie nazwy przez uzytkownika
    //3 - wpisywanie ceny przez uzytkownika
    //4 - wybor czy towar ma byc liczony na szt. czy kg.
    int blad=0; //przechowuje informacje o bledach znalezionych podczas przeszukiwania bazy towarow
    //0 - brak bledow
    //1 - kod juz istnieje w bazie
    //2 - wartosc ma za duzo znakow
    //3 - za duzo cyfr po przecinku
    int id=0; //id wpisywane przez uzytkownika
    double cena=0; //ilosc wpisywana przez uzytkownika
    int przecinek=0; //pamieta ilosc cyfr po przecinku we wprowadzanej ilosci powiekszona o 1
    bool poPrzecinku=false; //pamieta czy podajemy juz ilosc po przecinku
    char wybor; //informacja o klawiszu wcisnietym przez uzytkownika
    int dlugosc=0; //pamieta dlugosc wprowadzonej wartosci
    int z=0; //przechowuje prosbe o zerowanie zmiennych tymczasowych na koncu petli glownej metody
    string nazwa=""; //wprowadzana przez uzytkownika nazwa
    int typ=0; //typ towaru szt/kg

    do
    {
        system( "cls" ); //czyszczenie ekranu
        //OPCJE MENU
        if (tryb==1 || tryb==3)
        {
            cout << "c - anuluj" << endl;
            cout << "e - potwierdz" << endl << endl;
        }
        else if (tryb==2)
        {
            cout << "esc - anuluj" << endl;
            cout << "enter - potwierdz" << endl << endl;
        }
        else if (tryb==4)
        {
            cout << "c - anuluj" << endl;
            cout << "1 - Towar sprzedawny na sztuki." << endl;
            cout << "0 - Towar sprzedawany na kg." << endl << endl;
        }

        //WPROWADZENIE KODU PRODUKTU PRZEZ UZYTKOWNIKA
        if (tryb==1)
        {


            //wyswietlanie bledu wyszukiwania z poprzedniego obrotu petli
            if (blad==1)
            {
                cout << "Towar o takim kodzie juz istnieje." << endl;
                cout << endl;
                blad=0; //zerowanie bledu
                id=0; //zerowanie id;
                dlugosc=0;
            }
            else if (blad==2)
            {
                cout << "Kod za dlugi." << endl;
                cout << endl;
                blad=0; //zerowanie bledu
                id=0; //zerowanie id;
                dlugosc=0;
            }


            //Zapytanie o wprowadzenie id produktu
            cout << "Podaj nowy identyfikator produktu (tylko cyfry):" << endl;

            //Wyswietlenie wprowadzonego dotychczas id
            if (id>0) cout << id;

            //Pobranie przycisku wduszonego przez uzytkownika
            wybor=getch();

            //Reakcja tylko na cyfry i klawisze specjalne
            if (isdigit(wybor))
            {
                char * cyfra;
                cyfra = new char[2];
                cyfra[0]=wybor;
                cyfra[1]='k';
                id=id*10+atoi(cyfra);
                delete [] cyfra;
                dlugosc++;
                if (dlugosc==4)
                {
                    blad=2;
                }
            }
            else if ((wybor=='e' || wybor=='E') && dlugosc>0)
            {
                int i=0;
                while(blad==0 && i<n)
                {
                    if (listaTowarow[i].id==id)
                    {
                        blad=1;
                        i--;
                    }
                    i++;
                }
                if (i==n)
                {
                    tryb=2;
                }
                dlugosc=0;
            }
        }

        //WPROWADZANIE NAZWY PRZEZ UZYTKOWNIKA
        else if (tryb==2)
        {
            //wyswietlanie bledu wpisywania z poprzedniego obrotu petli
            if (blad==2)
            {
                cout << "Nazwa za dluga." << endl;
                cout << endl;
                blad=0; //zerowanie bledu
                nazwa=""; //zerowanie nazwy;
                dlugosc=0;
            }

            //Prosba o wprowadzenie danych
            cout << "Podaj nazwe (tylko litery i cyfry): "<< endl;


            //Wyswietlenie wprowadzonej dotychczas nazwy
            if (nazwa.length()>0)
            {
                cout << nazwa;
            }

            //Pobranie przycisku wduszonego przez uzytkownika
            wybor=getch();
            int kodZnaku = static_cast < int >( wybor );

            //Reakcja tylko na litery i cyfry
            if (isalnum(wybor))
            {
                nazwa=nazwa+wybor;
                dlugosc++;
                if (dlugosc==10)
                {
                    blad=2;
                }
                if (wybor=='c' || wybor=='C')
                {
                    wybor=' ';
                }
            }

            //koniec wprowadzania nazwy
            else if (kodZnaku==13 && dlugosc>0)
            {
                tryb=3;
            }
            else if (kodZnaku==27)
            {
                wybor='c';
            }
        }

        //WPROWADZANIE CENY PRZEZ UZYTKOWNIKA
        else if (tryb==3)
        {
            //wyswietlanie bledu wpisywania z poprzedniego obrotu petli
            if (blad==2)
            {
                cout << "Wartosc za dluga." << endl;
                cout << endl;
                blad=0; //zerowanie bledu
                cena=0; //zerowanie ceny;
                przecinek=0;
                poPrzecinku=false;
                dlugosc=0;
            }
            else if (blad==3)
            {
                cout << "Za duzo cyfr po przecinku." << endl;
                cout << endl;
                blad=0; //zerowanie bledu
                cena=0; //zerowanie ceny;
                przecinek=0;
                poPrzecinku=false;
                dlugosc=0;
            }

            //Prosba o wprowadzenie danych
            cout << "Podaj cene: "<< endl;


            //Wyswietlenie wprowadzonej dotychczas ceny
            if (cena>0)
            {
                if (przecinek>0)
                {
                    double iloscZPrzecinkiem=cena;
                    for (int j=0; j<przecinek; j++)
                    {
                        iloscZPrzecinkiem=iloscZPrzecinkiem/10;
                    }
                    cout << fixed << setprecision(przecinek) << iloscZPrzecinkiem;
                }
                else
                {
                    cout << fixed << setprecision(0) << cena;
                    if (poPrzecinku && przecinek==0) cout << ".";
                }
                cout << fixed << setprecision(2);

            }

            //Pobranie przycisku wduszonego przez uzytkownika
            wybor=getch();

            //Reakcja tylko na cyfry, znaki '.' oraz ',' i klawisze specjalne
            if (isdigit(wybor))
            {
                if (poPrzecinku) przecinek++;
                char * cyfra;
                cyfra = new char[2];
                cyfra[0]=wybor;
                cyfra[1]='k';
                cena=cena*10+atoi(cyfra);
                delete [] cyfra;
                dlugosc++;
                if (dlugosc==10)
                {
                    blad=2;
                }
                if (przecinek==3)
                {
                    blad=3;
                }
            }
            else if ((wybor==',' || wybor=='.') && !poPrzecinku)
            {
                poPrzecinku=true;

            }
            //koniec wprowadzania nowej ceny
            else if ((wybor=='e' || wybor=='E') && dlugosc>0)
            {
                if (przecinek > 0)
                {
                    double cenaZPrzecinkiem=cena;
                    for (int j=0; j<przecinek; j++)
                    {
                        cenaZPrzecinkiem=cenaZPrzecinkiem/10;
                    }
                    cena = cenaZPrzecinkiem;
                }

                tryb=4;
            }

        }

        //WPROWADZANIE TYPU TOWARU PRZEZ UZYTKOWNIKA
        else if (tryb==4)
        {
            //Prosba o wprowadzenie danych
            cout << "Wybierz (1 lub 0): "<< endl;

            //Pobranie przycisku wduszonego przez uzytkownika
            wybor=getch();
            if (wybor=='1' || wybor=='0')
            {
                if (wybor=='1')
                {
                    typ=1;
                }
                else if (wybor=='0')
                {
                    typ=0;
                }
                towar nowyTowar;
                nowyTowar.ustaw(id,nazwa,cena,typ);
                dodajTowar(nowyTowar);
                wyjscie=1;
            }

        }

        //Sprawdzenie czy uzytkownik nie chce anulowac
        if (wybor=='c' || wybor=='C')
        {
            wyjscie=1;
        }
    }
    while(wyjscie==0);

}



//---Proste zliczanie------------------------------------------------------------------------------------------------------------------------
void kasa::zliczanie()
{
    int blad=0; //przechowuje informacje o bledach znalezionych podczas przeszukiwania bazy towarow
    int id;
    double ilosc;

    while (0==0)
    {
        system("cls");//czyszczenie ekranu

        cout << "SUMA: " << suma << "zl" << endl << endl;//wyswietlanie aktualnej sumy kosztu towarow

        //wyswietlanie bledu wyszukiwania z poprzedniego obrotu petli
        if (blad==1)
        {
            cout << "Brak towaru w bazie." << endl;
            podpowiedzi(id);
            blad=0; //zerowanie bledu
        }


        cout << "Podaj identyfikator produktu (0 - nowy klient):" << endl;
        cin >> id;
        if (id>0)
        {
            int i;
            for (i=0; i<n; i++)
            {
                if (listaTowarow[i].id==id)
                {
                    cout << listaTowarow[i].nazwa << endl;;
                    if (listaTowarow[i].typ==0)
                    {
                        cout << "Podaj ilosc [kg]: ";
                    }
                    else
                    {
                        cout << "Podaj ilosc [szt.]: ";
                    }
                    cin >> ilosc;
                    suma = suma + listaTowarow[i] * ilosc;
                }
            }
            if (i==n)
            {
                blad=1;
            }
        }
        else
        {
            if (id==0)
            {
                suma=0;
            }
        }
    }
}

//---Naglowek kalkulatora-------------------------------------------------------------------------------------------------------------------
void kasa::naglowek(int waga)
{
    wagaSklepowa.pomiar();
    cout << "m - menu" << endl;
    cout << "n - nastepny klient" << endl;
    if (waga==0) cout << "w - pobierz wartosc z wagi" << endl;
    cout << "e - potwierdz" << endl << endl;

    cout << "SUMA: " << suma << "zl" << endl << endl;//wyswietlanie aktualnej sumy kosztu towarow
    if (waga==0)  cout << "Wartosc na wadze: " << wagaSklepowa.odczyt << "kg" << endl << endl;
}

//---Wyswietla wiersz z informacjami o towarze------------------------------------------------------------------
void kasa::wyswietlTowar(towar towar1)
{
    if (towar1.typ==0)
        cout << towar1.id << " " << towar1.nazwa << " " << towar1.cena << "zl/kg" << endl;
    else
        cout << towar1.id << " " << towar1.nazwa << " " << towar1.cena << "zl/szt." << endl;
};

//---Wyszukiwanie podpowiedzi-----------------------------------------------------------------------------------
void kasa::podpowiedzi(int idSzukane)
{
    bool pierwszaPopowiedz=true;
    for (int i=0; i<n; i++)
        {
            if ( (listaTowarow[i].id < (idSzukane*10+10) ) && (listaTowarow[i].id > idSzukane*10) )
            {
                if (pierwszaPopowiedz)
                {
                    cout << "Towary o zblizonym kodzie:" << endl;
                    pierwszaPopowiedz=false;
                }
                wyswietlTowar(listaTowarow[i]);
            }
        }
};

//---sortowanie---------------------------------------------------------------------------------------------------

void kasa::sortuj (int l, int p)
{
    if (p==-1) p=n-1;//pozwala wywolywac sortowanie na calej tablicy bez podawania parametrow
    int i=l,j=p;
    towar x=listaTowarow[(i+j)/2];
    while (i<=j)
    {
        while (listaTowarow[i]<x) i++;
        while (listaTowarow[j]>x) j--;
        if (i<=j)
        {
            zamien(listaTowarow[i],listaTowarow[j]);
            i++;
            j--;
        }
        if (l<j) sortuj(l,j);
        if (i<p) sortuj(i,p);
    }

    return;
}

//---zamienia wartosci dwoch towarow-----------------------------------------------------------------------------------
void kasa::zamien(towar& x,towar& y)
{
    towar t;
    t=x;
    x=y;
    y=t;
    return;
}

//---wczytywanie towarow z pliku---------------------------------------------------------------------------------------
void kasa::wczytajPlik()
{
    //Otwieranie pliku bazy towarow
    fstream plik;
    plik.open("baza.txt",ios::in);

    //Wczytywanie towarow z pliku do pamieci
    plik >> n;
    listaTowarow = new towar[n];
    for (int i=0; i<n; i++)
    {
        int id;
        double cena;
        int typ;
        string nazwa;
        plik >> id >> typ >> cena >> nazwa;
        listaTowarow[i].ustaw(id, nazwa,cena,typ);

    }

    //zamykanie pliku
    plik.close();

    //sortowanie bazy danych
    sortuj();
}

//---zapisywanie listy towarow do pliku----------------------------------------------------------------------------------
void kasa::zapiszPlik()
{
    //Otwieranie pliku bazy towarow
    fstream plik;
    plik.open("baza.txt",ios::out);

    //Wczytywanie towarow z pamieci do pliku
    plik << n << endl;
    for (int i=0; i<n; i++)
    {
        plik << listaTowarow[i].id << " " << listaTowarow[i].typ << " " << listaTowarow[i].cena << endl;
        plik << listaTowarow[i].nazwa << endl;
    }

    //zamykanie pliku
    plik.close();

}

//---usuwanie towaru------------------------------------------------------------------------------------------------------
void kasa::usunTowar(int i)
{
    if (i!=-1)
    {
        n--;
        towar * nowaLista = new towar[n];
        int k=0;
        for (int j=0; j<n; j++)
        {
            if (j!=i)
            {
            nowaLista[k]=listaTowarow[j];
            k++;
            }
        }
        delete [] listaTowarow;
        listaTowarow=nowaLista;
        zapiszPlik();
    }
}

//---dodawanie towaru------------------------------------------------------------------------------------------------------
void kasa::dodajTowar(towar nowyTowar)
{
    n++;
    towar * nowaLista = new towar[n];
    for (int j=0; j<n-1; j++)
    {
            nowaLista[j]=listaTowarow[j];
    }
    nowaLista[n-1]=nowyTowar;
    delete [] listaTowarow;
    listaTowarow=nowaLista;
    zapiszPlik();
}

//---wyswietla liste wszystkich towarow w bazoe----------------------------------------------------------------------------
void kasa::przegladTowarow()
{
    //FLAGI I ZMIENNE TYMCZASOWE
    int wyjscie=0;
    char wybor;

    while (wyjscie==0)
    {
        system( "cls" ); //czyszczenie ekranu

        //OPCJE MENU
        cout << "e - powrot" << endl << endl;

        for (int i=0; i<n; i++)
        {
            wyswietlTowar(listaTowarow[i]);
        }

        //Pobranie przycisku wduszonego przez uzytkownika
        wybor=getch();


        if (wybor=='e' || wybor=='E')
        {
            wyjscie=1;
        }

    }

}

//---MAIN--------------------------------------------------------------------------------------------------------------
main()
{
    kasa warzywniak;
    warzywniak.menuZliczania();
}
