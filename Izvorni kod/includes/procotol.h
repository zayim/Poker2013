//---------------------------------------------------------------------------

#ifndef procotolH
#define procotolH

#include <string.h>
#include <vcl.h>
#include "korisnik.h"
#include "konstante.h"

enum TipPaketa
{
                 pktKonekcijaResponse,
                 pktRegistracija,
                 pktRegistracijaResponse,
                 pktLogin,
                 pktLoginResponse,
                 pktLogout,
                 pktKorisnikZahtjev,
                 pktKorisnikResponse,
                 pktNapraviSobu,
                 pktNapraviSobuResponse,
                 pktZahtjevSveSobe,
                 pktSveSobeResponse,
                 pktUdjiUSobu,
                 pktUdjiUSobuResponse,
                 pktIzadjiIzSobe,
                 pktIzlazakIzSobeNotifikacija,
                 pktIzbacenIzSobe,
                 pktIzbacenIzSobeNotifikacija,
                 pktZatvoriSobu,
                 pktSviIgraciUSobi,
                 pktSviIgraciUSobiResponse,
                 pktZapocniIgruUSobi,
                 pktPocetakIgreNotifikacija,
                 pktKarte,
                 pktPotez
};
enum TipPoteza
{
                 potezCall,
                 potezFold,
                 potezRaise
};

class Paket
{
        protected:
        TipPaketa tip;

        public:
        Paket(TipPaketa tip) : tip(tip) {}
        TipPaketa Tip() { return tip; }
};

class PaketKonekcijaResponse : public Paket
{
        public:
        bool status;

        PaketKonekcijaResponse() : Paket(pktKonekcijaResponse) {}
        PaketKonekcijaResponse(bool s) : Paket(pktKonekcijaResponse), status(s) {}
};

class PaketRegistracija : public Paket
{
        public:
        char ime[10];
        char prezime[20];
        char username[20];
        char password[20];

        PaketRegistracija() : Paket(pktRegistracija) {}
        PaketRegistracija(const Korisnik &k);

};
class PaketRegistracijaResponse : public Paket
{
        public:
        bool status;

        PaketRegistracijaResponse() : Paket(pktRegistracijaResponse) {}
        PaketRegistracijaResponse(bool s) : Paket(pktRegistracijaResponse), status(s) {}
};
class PaketLogin : public Paket
{
        public:
        char username[20];
        char password[20];

        PaketLogin() : Paket(pktLogin) {}
        PaketLogin(const Korisnik &k);
};
class PaketLoginResponse : public Paket
{
        public:
        int status;
        const static int LOGIN_USPJESAN=0;
        const static int LOGIN_POGRESAN=1;
        const static int VEC_LOGOVAN=2;

        PaketLoginResponse() : Paket(pktLoginResponse) {}
        PaketLoginResponse(int s) : Paket(pktLoginResponse), status(s) {}
};
class PaketLogout : public Paket
{
        public:
        char username[20];
        int cipovi;

        PaketLogout() : Paket(pktLogout) {}
        PaketLogout(const char un[20]) : Paket(pktLogout) { strcpy(username,un); }
};
class PaketKorisnikZahtjev : public Paket
{
        public:
        char username[20];

        PaketKorisnikZahtjev() : Paket(pktKorisnikZahtjev) {}
        PaketKorisnikZahtjev(const char un[20]) : Paket(pktKorisnikZahtjev) {
                                                      strcpy(username,un);}
};
class PaketKorisnikResponse : public Paket
{
        public:
        bool status;
        char ime[10];
        char prezime[20];
        char username[20];
        int cipovi;

        PaketKorisnikResponse() : Paket(pktKorisnikResponse) {}
        PaketKorisnikResponse(bool s) : Paket(pktKorisnikResponse), status(s) {}
        PaketKorisnikResponse(const char im[10], const char pr[20], const char un[20], int);
};
class PaketNapraviSobu : public Paket
{
        public:
        char ime[20];
        char imeKorisnika[10];
        char prezimeKorisnika[20];
        char usernameKorisnika[20];
        int cipoviKorisnika;

        PaketNapraviSobu() : Paket(pktNapraviSobu) {}
        PaketNapraviSobu(const char im[20]) : Paket(pktNapraviSobu) {
                                                      strcpy(ime,im);}
};
class PaketNapraviSobuResponse : public Paket
{
        public:
        int idSobe;

        PaketNapraviSobuResponse() : Paket(pktNapraviSobuResponse) {}
        PaketNapraviSobuResponse(int id) : Paket(pktNapraviSobuResponse), idSobe(id) {}
};
class PaketZahtjevSveSobe : public Paket
{
        public:
        PaketZahtjevSveSobe() : Paket(pktZahtjevSveSobe) {}
};
class PaketSveSobeResponse : public Paket
{
        public:

        struct Soba
        {
                char ime[20];
                int id;
                int brojIgraca;
        };

        int brojSoba;
        Soba sobe[2000];

        PaketSveSobeResponse() : Paket(pktSveSobeResponse) {}
        PaketSveSobeResponse(int n) : Paket(pktSveSobeResponse), brojSoba(n) {}
};
class PaketUdjiUSobu : public Paket
{
        public:

        char ime[10];
        char prezime[20];
        char username[20];
        int cipovi;
        int idSobe;

        PaketUdjiUSobu() : Paket(pktUdjiUSobu) {}
        PaketUdjiUSobu(int id) : Paket(pktUdjiUSobu), idSobe(id) {}
};
class PaketUdjiUSobuResponse : public Paket
{
        public:
        int dodijeljeniID;

        PaketUdjiUSobuResponse() : Paket(pktUdjiUSobuResponse) {}
        PaketUdjiUSobuResponse(int id) : Paket(pktUdjiUSobuResponse), dodijeljeniID(id) {}
};
class PaketIzadjiIzSobe : public Paket
{
        public:
        int idSobe;
        int idIgracaUSobi;

        PaketIzadjiIzSobe () : Paket(pktIzadjiIzSobe) {}
        PaketIzadjiIzSobe (int ids, int idi) : Paket(pktIzadjiIzSobe), idSobe(ids), idIgracaUSobi(idi) {}
};
class PaketIzbacenIzSobe : public Paket
{
        public:
        int idSobe;
        int idIgracaUSobi;

        PaketIzbacenIzSobe () : Paket(pktIzbacenIzSobe) {}
        PaketIzbacenIzSobe (int ids, int idi) : Paket(pktIzbacenIzSobe), idSobe(ids), idIgracaUSobi(idi) {}
};
class PaketIzbacenIzSobeNotifikacija : public Paket
{
        public:
        PaketIzbacenIzSobeNotifikacija() : Paket(pktIzbacenIzSobeNotifikacija) {}
};
class PaketIzlazakIzSobeNotifikacija : public Paket
{
        public:
        int idIgracaUSobi;

        PaketIzlazakIzSobeNotifikacija() : Paket(pktIzlazakIzSobeNotifikacija) {}
        PaketIzlazakIzSobeNotifikacija(int id) : Paket(pktIzlazakIzSobeNotifikacija), idIgracaUSobi(id) {}
};
class PaketZatvoriSobu : public Paket
{
        public:
        int idSobe;

        PaketZatvoriSobu() : Paket(pktZatvoriSobu) {}
        PaketZatvoriSobu(int id) : Paket(pktZatvoriSobu), idSobe(id) {}

};
class PaketSviIgraciUSobi : public Paket
{
        public:
        int idSobe;

        PaketSviIgraciUSobi() : Paket(pktSviIgraciUSobi) {}
        PaketSviIgraciUSobi(int id) : Paket(pktSviIgraciUSobi), idSobe(id) {}
};
class PaketSviIgraciUSobiResponse : public Paket
{
        public:
        struct Korisnik_ID
        {
                Korisnik korisnik;
                int idUSobi;
        };

        int brojIgraca;
        Korisnik_ID igraci[20];

        PaketSviIgraciUSobiResponse() : Paket(pktSviIgraciUSobiResponse) {}
        PaketSviIgraciUSobiResponse(int n) : Paket(pktSviIgraciUSobiResponse), brojIgraca(n) {}
};
class PaketZapocniIgruUSobi : public Paket
{
        public:
        int idSobe;

        PaketZapocniIgruUSobi() : Paket(pktZapocniIgruUSobi) {}
        PaketZapocniIgruUSobi(int id) : Paket(pktZapocniIgruUSobi), idSobe(id) {}

};
class PaketPocetakIgreNotifikacija : public Paket
{
        public:
        PaketPocetakIgreNotifikacija() : Paket(pktPocetakIgreNotifikacija) {}
};
class PaketKarte : public Paket
{
        public:
        int brojIgraca;
        int boje[50];
        int brojevi[50];
        int idSobe;

        PaketKarte() : Paket(pktKarte) {}
        PaketKarte(int n, int ids) : Paket(pktKarte), brojIgraca(n), idSobe(ids) {}
};
class PaketPotez : public Paket
{
        public:
        TipPoteza tipPoteza;
        int iznosRaise;
        int idSobe;
        int idIgacaUSobi;

        PaketPotez() : Paket(pktPotez) {}
        PaketPotez(TipPoteza tp) : Paket(pktPotez), tipPoteza(tp) {}
};
//---------------------------------------------------------------------------
#endif
