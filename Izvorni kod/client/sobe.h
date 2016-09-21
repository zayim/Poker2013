//---------------------------------------------------------------------------

#ifndef sobeH
#define sobeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ScktComp.hpp>
#include "../includes/korisnik.h"
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <Grids.hpp>
#include "../includes/konstante.h"
#include "soba_igraci.h"
#include "data_module.h"
//---------------------------------------------------------------------------
class TFormSobe : public TForm
{
__published:	// IDE-managed Components
        TImage *im_pozadina;
        TEdit *ed_imeSobe;
        TButton *bt_napraviSobu;
        TStringGrid *sg_sobe;
        TButton *bt_izaberiSobu;
        TButton *bt_osvjeziSobe;
        TLabel *l_ime;
        TLabel *l_prezime;
        TLabel *l_cipoviNaslov;
        TLabel *l_cipovi;
        void __fastcall FormSobeOnClose(TObject *Sender,
          TCloseAction &Action);
        void __fastcall bt_napraviSobuClick(TObject *Sender);
        void __fastcall bt_osvjeziSobeClick(TObject *Sender);
        void __fastcall sg_sobeOnSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
        void __fastcall bt_izaberiSobuClick(TObject *Sender);
        void __fastcall FormSobeOnShow(TObject *Sender);
private:	// User declarations
        //Korisnik korisnik;
        int *sobeID;
        char sobeImena[2000][20];
        int odabranaSoba;
        char odabranoIme[20];
public:		// User declarations
        __fastcall TFormSobe(TComponent* Owner);
        TClientSocket *cs_clientSocket;
        void __fastcall (__closure *__OnRead)(TObject*, TCustomWinSocket*);
        void __fastcall cs_OnRead(TObject*, TCustomWinSocket*);
        void set_korisnikUsername(const char un[]);
        void init();
        void odjaviKorisnika();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSobe *FormSobe;
//---------------------------------------------------------------------------
#endif
