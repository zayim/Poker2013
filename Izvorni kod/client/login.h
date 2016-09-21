//---------------------------------------------------------------------------

#ifndef loginH
#define loginH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ScktComp.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include "data_module.h"
//---------------------------------------------------------------------------
class TFormLogin : public TForm
{
__published:	// IDE-managed Components
        TImage *im_pozadina;
        TLabel *l_usernameNaslov;
        TLabel *l_passwordNaslov;
        TEdit *ed_username;
        TEdit *ed_password;
        TButton *bt_prijaviSe;
        TLabel *l_registrujSe;
        void __fastcall FormLoginOnClose(TObject *Sender,
          TCloseAction &Action);
        void __fastcall l_registrujSeClick(TObject *Sender);
        void __fastcall bt_prijaviSeClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
        TClientSocket *cs_clientSocket;
        void __fastcall (__closure *__OnRead)(TObject*, TCustomWinSocket*);
        void __fastcall cs_OnRead(TObject*, TCustomWinSocket*);
        __fastcall TFormLogin(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormLogin *FormLogin;
//---------------------------------------------------------------------------
#endif
