#ifndef glavnaH
#define glavnaH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <ScktComp.hpp>
#include <jpeg.hpp>
#include "../includes/procotol.h"
//---------------------------------------------------------------------------
class TFormConnect : public TForm
{


__published:	// IDE-managed Components
        TImage *im_pozadina;
        TEdit *ed_adresaServera;
        TButton *bt_konektujSe;
        TClientSocket *cs_clientSocket;
        void __fastcall bt_konektujSeClick(TObject *Sender);
        void __fastcall csOnRead(TObject *Sender,
          TCustomWinSocket *Socket);
        void __fastcall cs_OnError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
        void __fastcall cs_OnDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
private:	// User declarations
        bool daLiJeAdresa(const AnsiString&);
        void otvoriFormuZaLogin();
public:		// User declarations
        __fastcall TFormConnect(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormConnect *FormConnect;
//---------------------------------------------------------------------------
#endif
