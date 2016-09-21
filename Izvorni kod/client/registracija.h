//---------------------------------------------------------------------------

#ifndef registracijaH
#define registracijaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <ScktComp.hpp>
//---------------------------------------------------------------------------
class TFormRegistracija : public TForm
{
__published:	// IDE-managed Components
        TImage *im_pozadina;
        TEdit *ed_ime;
        TEdit *ed_prezime;
        TEdit *ed_username;
        TEdit *ed_password;
        TLabel *l_imeNaslov;
        TLabel *ed_prezimeNaslov;
        TLabel *ed_usernameNaslov;
        TLabel *ed_passwordNaslov;
        TButton *bt_registrujSe;
        void __fastcall FormRegistracijaOnClose(TObject *Sender,
          TCloseAction &Action);
        void __fastcall bt_registrujSeClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        TClientSocket *cs_clientSocket;
        void __fastcall (__closure *__OnRead)(TObject*, TCustomWinSocket*);
        void __fastcall cs_OnRead(TObject*, TCustomWinSocket*);
        __fastcall TFormRegistracija(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormRegistracija *FormRegistracija;
//---------------------------------------------------------------------------
#endif
