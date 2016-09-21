//---------------------------------------------------------------------------

#ifndef glavnaH
#define glavnaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ScktComp.hpp>
#include "soba.h"
#include <korisnik.h>
#include <vector.h>
#include "../includes/procotol.h"
//---------------------------------------------------------------------------
class TFormGlavna : public TForm
{
__published:	// IDE-managed Components
        TMemo *me_log;
        TButton *bt_pokreniServer;
        TLabel *l_logNaslov;
        TServerSocket *ss_serverSocket;
        TButton *bt_sacuvajLog;
        void __fastcall bt_pokreniServerClick(TObject *Sender);
        void __fastcall ss_OnClientConnect(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall ss_OnClientDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall ss_OnClientRead(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall ss_OnClientError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
        void __fastcall bt_sacuvajLogClick(TObject *Sender);
private:	// User declarations
        vector<Soba*> sobe;
        vector<AnsiString> logovaniKorisnici;
        vector<Soba*>::iterator nadjiSobuPremaIDu(int idSobe);
        void registrujKorisnika(PaketRegistracija* paket, TCustomWinSocket* korisnik);
        void prijaviKorisnika(PaketLogin* paket, TCustomWinSocket* korisnik);
        void odjaviKorisnika(PaketLogout* paket, TCustomWinSocket* korisnik);
        void obradiKorisnikZahtjev(PaketKorisnikZahtjev* paket, TCustomWinSocket* korisnik);
        void obradiNapraviSobu(PaketNapraviSobu* paket, TCustomWinSocket* korisnik);
        void obradiZahtjevSveSobe(PaketZahtjevSveSobe* paket, TCustomWinSocket* korisnik);
        void obradiZatvoriSobu(PaketZatvoriSobu* paket, TCustomWinSocket* korisnik);
        void obradiUdjiUSobu(PaketUdjiUSobu* paket, TCustomWinSocket* korisnik);
        void obradiSviIgraciUSobi(PaketSviIgraciUSobi* paket, TCustomWinSocket* korisnik);
        void obradiIzadjiIzSobe(PaketIzadjiIzSobe* paket, TCustomWinSocket* korisnik);
        void obradiIzbacenIzSobe(PaketIzbacenIzSobe* paket, TCustomWinSocket* korisnik);
        void obradiZapocniIgruUSobi(PaketZapocniIgruUSobi* paket, TCustomWinSocket* korisnik);
        void obradiKarte(PaketKarte* paket, TCustomWinSocket* korisnik);
        void obradiPotez(PaketPotez* paket, TCustomWinSocket* korisnik);
        AnsiString trenutnoVrijeme();
        void odjaviSveKorisnike();
        unsigned long long int nextID;

        int sleepTime__;
public:		// User declarations
        __fastcall TFormGlavna(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormGlavna *FormGlavna;
//---------------------------------------------------------------------------
#endif
