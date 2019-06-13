#ifndef towar_hpp
#define towar_hpp

#include <ctime> //do losowania
#include <cstdlib> //do losowania

using namespace std;

//---KLASA WAGA----------------------------------------------------------------------------------------------------------------------------
//Klasa sluzy do celow pokazowych.
class waga
{
public:
    double odczyt;
    void pomiar()
    {
        //MIEJSCE DO WPISANIA INSTRUKCJI POBIERANIA ODCZYTU Z ZEWNETRZNEJ WAGI
        odczyt=(rand()%3+1);
        double odczytKoncowki=rand()%100;
        odczyt=odczyt+odczytKoncowki/100;

    }

    waga()
    {
        srand(time(NULL)); //do losowania
        pomiar();
    }

};

//---KLASA TOWAR---------------------------------------------------------------------------------------------------------------------------
class towar
{
public:
    //Pola:
    int id; //unikatowy identyfikator produktu
    string nazwa; //nazwa towaru
    double cena; //cena jednostkowa w zlotych
    int typ; //typ jednostki w ktorej sprzedawany jest towar
    //0 - kg
    //1 - sztuki

    //Metody:
    towar(int id0=0, string nazwa0="0", double cena0=0, int typ0=0);//konstruktor
    void ustaw(int id0=0, string nazwa0="0", double cena0=0, int typ0=0);//ustawia parametry towaru
    double operator*(double iloscTowaru); //mnozenie wykorzystywane przy zliczaniu ceny
    int operator=(towar towar2); //operator przypisania
    bool operator<(towar towar2); //operatory wykorzystywane przy sortowaniu towarow po id
    bool operator>=(towar towar2);//operatory wykorzystywane przy sortowaniu towarow po id
    bool operator>(towar towar2);//operatory wykorzystywane przy sortowaniu towarow po id
    bool operator<=(towar towar2);//operatory wykorzystywane przy sortowaniu towarow po id
    bool operator==(towar towar2);//operatory wykorzystywane przy sortowaniu towarow po id
};

//---Konstruktor----------------------------------------
towar::towar(int id0, string nazwa0, double cena0, int typ0)
{
    id=id0;
    nazwa=nazwa0;
    cena=cena0;
    typ=typ0;
}

//---Ustawianie wartosci---------------------------------
void towar::ustaw(int id0, string nazwa0, double cena0, int typ0)
{
    id=id0;
    nazwa=nazwa0;
    cena=cena0;
    typ=typ0;
}

//---operator mnozenia przez double-------------------------------------------
double towar::operator*(double iloscTowaru) //mnozenie wykorzystywane przy zliczaniu ceny
{
    return cena*iloscTowaru;
}

//---operator mnozenia przez double-------------------------------------------
int towar::operator=(towar towar2)
{
    id=towar2.id;
    nazwa=towar2.nazwa;
    cena=towar2.cena;
    typ=towar2.typ;
    return 1;
}

//---operatory porownania-----------------------------------------------------
bool towar::operator<(towar towar2)
{
    if (id<towar2.id)
        return true;
    else
        return false;
}

bool towar::operator<=(towar towar2)
{
    if (id<=towar2.id)
        return true;
    else
        return false;
}

bool towar::operator>(towar towar2)
{
    if (id>towar2.id)
        return true;
    else
        return false;
}

bool towar::operator>=(towar towar2)
{
    if (id>=towar2.id)
        return true;
    else
        return false;
}

bool towar::operator==(towar towar2)
{
    if (id==towar2.id)
        return true;
    else
        return false;
}

#endif
