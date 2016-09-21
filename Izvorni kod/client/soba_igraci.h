//---------------------------------------------------------------------------

#ifndef soba_igraciH
#define soba_igraciH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <ScktComp.hpp>
#include "../includes/korisnik.h"
#include "../includes/konstante.h"
#include "../includes/procotol.h"
#include "data_module.h"
#include <Grids.hpp>
#include <vector>
//---------------------------------------------------------------------------
class TFormSobaIgraci : public TForm
{
__published:	// IDE-managed Components
        TImage *im_pozadina;
        TStringGrid *sg_igraci;
        TButton *bt_izbrisiIgraca;
        TButton *bt_zapocniIgru;
        TButton *bt_osvjezi;
        TButton *bt_napustiSobu;
        TLabel *l_ime;
        TLabel *l_prezime;
        TLabel *l_cipoviNaslov;
        TLabel *l_cipovi;
        void __fastcall FormSobaIgraciOnClose(TObject *Sender,
          TCloseAction &Action);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall bt_napustiSobuClick(TObject *Sender);
        void __fastcall bt_osvjeziClick(TObject *Sender);
        void __fastcall sg_igraciOnSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect);
        void __fastcall bt_izbrisiIgracaClick(TObject *Sender);
        void __fastcall bt_zapocniIgruClick(TObject *Sender);
        void __fastcall TFormSobaIgraciOnShow(TObject *Sender);
private:	// User declarations
        struct Korisnik_ID
        {
                Korisnik korisnik;
                int idUSobi;
        };
        std::vector<Korisnik_ID> ostaliIgraci;
        int brojOstalihIgraca;
        int izabraniIgracZaBrisanje;

public:		// User declarations
        TForm* kreator;
        __fastcall TFormSobaIgraci(TComponent* Owner);
        TClientSocket *cs_clientSocket;
        void __fastcall (__closure *__OnRead)(TObject*, TCustomWinSocket*);
        void __fastcall cs_OnRead(TObject*, TCustomWinSocket*);
        void updateujTabeluIgraca();
        bool daLiSamVlasnik;
        char imeSobe[20];
        int idSobe;
        int idIgracaUSobi;
        bool daLiSamIzbacen;
        void init();
        void pocniIgru();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSobaIgraci *FormSobaIgraci;
//---------------------------------------------------------------------------
#endif
