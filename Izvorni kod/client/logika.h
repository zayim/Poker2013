//---------------------------------------------------------------------------

#ifndef logikaH
#define logikaH
#include "../includes/korisnik.h"
#include <vector>
#include <vcl.h>
#include <jpeg.hpp>
#include <fstream>
enum Boja { list, mak, srce, kocka };
enum Status { odustao, igra, allIn, izasao };
enum TipHanda { thHighCard, thOnePair, thTwoPairs, thTriling, thStraight, thFlush, thFullHouse,
                thPoker, thStraightFlush, thRoyalFlush };
class Karta
{
        public:
        Boja boja;
        int broj;
        TJPEGImage *slika;

        Karta() : slika(NULL) {}
        Karta(Boja b, int br);
        ~Karta() { delete slika; }
        AnsiString vratiImeFajla();
        bool operator < (const Karta &k) { return ( broj!=1 && broj<k.broj); }
        Karta& operator=(const Karta& k)
        {
                if (this==&k) return *this;

                delete slika;

                boja=k.boja; broj=k.broj;
                slika=new TJPEGImage;
                slika->Assign(k.slika);

                return *this;
        }
};

class Igrac
{
        public:
        char ime[10];
        char prezime[20];
        int cipovi;
        int idUSobi;
        Karta karta1,karta2;
        Status status;
        TPicture *canvasK1,*canvasK2;
        TImage* slikaPotez;
        TLabel *l_cipovi, *l_ulozeno;

        Igrac(const Korisnik& k, int id);
        void iscrtaj();
};

class Pot
{
        public:
        int brojIgracaNaPotu;
        int indexiIgracaNaPotu[20];
        int ulogPoIgracu;
        int uloziIgraca[20];
        int brojIgraca;

        TLabel* naslov;
        TLabel* iznos;

        Pot(int n);
        void dodajIndeksIgraca(int ind);
        void izbrisiIgracaPremaIndexu(int ind);
        bool ukloniIndeksIgraca(int ind);
        int vratiIznosPota();
};

class SkupKarti
{
        public:
        struct __Karta
        {
                Boja boja;
                int broj;

                __Karta() {}
                __Karta(Boja bj, int br) : boja(bj), broj(br) {}
                bool operator==(const __Karta &k) { return (boja==k.boja && broj==k.broj); }
                bool operator<(const __Karta &k) { return (broj < k.broj); }
        };

        struct __Hand
        {
                __Karta karte[5];
                TipHanda tip;

                bool operator>(const __Hand& h);
                bool operator==(const __Hand& h);
        };

        __Karta karte[7];
        __Hand hand;
        bool bits[52];

        SkupKarti(const Karta &k1, const Karta &k2, const Karta &k3, const Karta &k4,
                  const Karta &k5, const Karta &k6, const Karta &k7);
        SkupKarti(const __Karta &k1, const __Karta &k2, const __Karta &k3, const __Karta &k4,
                  const __Karta &k5, const __Karta &k6, const __Karta &k7);
        void konstruisiHand();
        bool operator>(const SkupKarti& sk) { return (hand > sk.hand); }
        bool operator==(const SkupKarti& sk) { return (hand == sk.hand); }

        int kartaToBitIndex(const __Karta &k);
        __Karta bitIndexToKarta(int bitInd);

        bool provjeriRoyalFlush();
        bool provjeriStraightFlush();
        bool provjeriPoker();
        bool provjeriFullHouse();
        bool provjeriFlush();
        bool provjeriStraight();
        bool provjeriTriling();
        void provjeriPairs();

        AnsiString vratiImeHanda();
};

//---------------------------------------------------------------------------
#endif
