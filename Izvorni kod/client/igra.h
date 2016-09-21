//---------------------------------------------------------------------------

#ifndef igraH
#define igraH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "soba_igraci.h"
#include "sobe.h"
#include <ScktComp.hpp>
#include "../includes/korisnik.h"
#include "../includes/konstante.h"
#include "../includes/procotol.h"
#include "logika.h"
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <vector>
//---------------------------------------------------------------------------
enum Stadij { stdPreFlop, stdFlop, stdTurn, stdRiver };
class TFormIgra : public TForm
{
__published:	// IDE-managed Components
        TImage *im_pozadina;
        TImage *im_flop1;
        TImage *im_K1igrac0;
        TImage *im_K2igrac0;
        TImage *im_flop2;
        TImage *im_flop3;
        TImage *im_turn;
        TImage *im_river;
        TImage *im_K1igrac3;
        TImage *im_K2igrac3;
        TImage *im_K1igrac2;
        TImage *im_K2igrac2;
        TImage *im_K1igrac4;
        TImage *im_K2igrac4;
        TImage *im_K1igrac1;
        TImage *im_K2igrac1;
        TLabel *l_ime4;
        TLabel *l_cipovi4;
        TLabel *l_ime3;
        TLabel *l_cipovi3;
        TLabel *l_ime2;
        TLabel *l_cipovi2;
        TLabel *l_ime1;
        TLabel *l_cipovi1;
        TLabel *l_ime0;
        TLabel *l_cipovi0;
        TLabel *l_pot1Naslov;
        TLabel *l_pot1;
        TLabel *l_pot3Naslov;
        TLabel *l_pot3;
        TLabel *l_pot2;
        TLabel *l_pot4;
        TLabel *l_pot5;
        TLabel *l_pot4Naslov;
        TLabel *l_pot2Naslov;
        TLabel *l_pot5Naslov;
        TLabel *l_ulozeno0;
        TLabel *l_ulozeno4;
        TLabel *l_ulozeno1;
        TLabel *l_ulozeno3;
        TLabel *l_ulozeno2;
        TButton *bt_fold;
        TButton *bt_call;
        TEdit *ed_raise;
        TButton *bt_raise;
        TImage *im_potez0;
        TImage *im_potez1;
        TImage *im_potez2;
        TImage *im_potez3;
        TImage *im_potez4;
        TLabel *l_pobjednikNaslov;
        TLabel *l_pobjednik;
        TLabel *l_handName;
        void __fastcall FormIgraOnClose(TObject *Sender,
          TCloseAction &Action);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall bt_callClick(TObject *Sender);
        void __fastcall bt_foldClick(TObject *Sender);
        void __fastcall bt_raiseClick(TObject *Sender);
private:	// User declarations
        //static const int IZNOS_BIG_BLIND=400;
        //static const int IZNOS_SMALL_BLIND=200;

        TJPEGImage *kartaDefault;

        std::vector<Igrac*> igraci;
        Stadij stadij;
        int igracNaPotezu;
        int igracZadnjiRaise;
        int igracBigBlind;
        Karta flop1, flop2, flop3, turn, river;
        std::vector<Pot*> potovi;
        int indexByID(int id);

        int prethodniIndex(int ind) { return (ind==0 ? igraci.size()-1 : ind-1); }
        int sljedeciIndex(int ind) { return (ind>=igraci.size()-1 ? 0 : ind+1); }

        void dodijeliSlucajnoKarte();
        void rasporediKarte(PaketKarte* pkt);
        int izracunajUkupanUlog(int ind);
        void iscrtajFlop();
        void iscrtajTurn();
        void iscrtajRiver();
        void sakrijFlop();
        void sakrijTurn();
        void sakrijRiver();
        void sakrijOstaleIgrace();
        bool provjeriValidnostRaiseIznosa(int iznos);
        void updateujIgricu();
        void zavrsiPartiju();

        void odigrajCall(int id__);
        void odigrajFold(int id__);
        void odigrajRaise(int id__, int iznos__);

        void ispisiIgruUDatoteku();

        void foo();

public:		// User declarations
        __fastcall TFormIgra(TComponent* Owner);
        TForm* kreator;

        TClientSocket *cs_clientSocket;
        void __fastcall (__closure *__OnRead)(TObject*, TCustomWinSocket*);
        void __fastcall cs_OnRead(TObject*, TCustomWinSocket*);

        int mojID;
        int idSobe;
        void init();
        void dodajIgraca(const Korisnik &k, int id) { igraci.push_back(new Igrac(k,id)); }
};
//---------------------------------------------------------------------------
extern PACKAGE TFormIgra *FormIgra;
//---------------------------------------------------------------------------
#endif
