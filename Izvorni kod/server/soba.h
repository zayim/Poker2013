//---------------------------------------------------------------------------

#ifndef sobaH
#define sobaH
#include <ScktComp.hpp>
#include <vector>
class Soba
{
        public:

        struct IgracSocket
        {
                int id;
                char ime[10];
                char prezime[20];
                char username[20];
                int cipovi;
                TCustomWinSocket *igrac;

                IgracSocket(int ID, TCustomWinSocket* IGR, const char UN[20], const char IME[10], const char PR[20], int cip)
                        : id(ID), igrac(IGR), cipovi(cip) { strcpy(username,UN); strcpy(ime,IME); strcpy(prezime,PR); }
        };

        int id;
        int nextID;
        int brojIgraca;
        char ime[20];
        bool igraUToku;
        std::vector<IgracSocket*> igraci;

        const static unsigned short MAX_BROJ_IGRACA=5;
        Soba(unsigned short id,const char ime[]);
        ~Soba();
        int dodajIgraca(TCustomWinSocket *igrac,  const char UN[20], const char IME[10], const char PR[20], int cip, TMemo* log);
        void posaljiSvimaSpisakIgraca();
        void izbrisiIgracaPremaID(int idIgracaKojiIzlazi, TMemo* log);
        void izbrisiIgracaPremaUsernameu(char usernameIgracaKojiIzlazi[20]);
        void obavijestiSveIgraceOBrisanju(int idIgracaUSobi);
        void obavijestiSveIgraceOZatvaranju();
        void obavijestiSveIgraceOPocetku();
        TCustomWinSocket* vratiSoketIgracaPremaID(int idIgraca);
};
//---------------------------------------------------------------------------
#endif
