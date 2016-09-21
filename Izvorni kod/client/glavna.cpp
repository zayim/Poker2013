//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "glavna.h"
#include "login.h"
#include "registracija.h"
#include "sobe.h"
#include "soba_igraci.h"
#include <jpeg.hpp>
#include "data_module.h"
#include "../includes/konstante.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormConnect *FormConnect;
//---------------------------------------------------------------------------
__fastcall TFormConnect::TFormConnect(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormConnect::bt_konektujSeClick(TObject *Sender)
{
        if (!cs_clientSocket->Active)
        {
                bt_konektujSe->Enabled=false;
                if (!daLiJeAdresa(ed_adresaServera->Text))
                {
                        ShowMessage("Neispravna adresa!");
                        return;
                }
                cs_clientSocket->Host=ed_adresaServera->Text;
                cs_clientSocket->Active=true;
        }
        else
        {
                cs_clientSocket->Active=false;
                cs_clientSocket->Host="";
                bt_konektujSe->Caption="Konektuj se";
                ed_adresaServera->Enabled=true;
        }
}
//---------------------------------------------------------------------------
bool TFormConnect::daLiJeAdresa(const AnsiString&)
{
        return true;
}
void __fastcall TFormConnect::csOnRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
        Paket* paket;
        int velicina=Socket->ReceiveLength();
        paket=(Paket*)malloc(velicina);

        Socket->ReceiveBuf(paket,velicina);

        if (paket->Tip()==pktKonekcijaResponse)
        {
                otvoriFormuZaLogin();
                bt_konektujSe->Enabled=true;
        }

        free(paket);
}
//---------------------------------------------------------------------------
void TFormConnect::otvoriFormuZaLogin()
{
        ShowMessage("Uspješna konekcija!");
        FormLogin = new TFormLogin(this);
        FormLogin->cs_clientSocket=cs_clientSocket;
        FormLogin->__OnRead=cs_clientSocket->OnRead;
        FormLogin->cs_clientSocket->OnRead=FormLogin->cs_OnRead;
        FormLogin->Show();
        this->Hide();
        bt_konektujSe->Caption="Diskonektuj se";
        ed_adresaServera->Enabled=false;
}

void __fastcall TFormConnect::cs_OnError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
        ShowMessage("Greska pri konekciji!");
        ErrorCode=0;
        bt_konektujSe->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TFormConnect::cs_OnDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
        ShowMessage("Diskonektovani ste sa servera!");

        FormLogin->Hide();
        FormRegistracija->Hide();
        FormSobe->Hide();
        FormSobaIgraci->Hide();

        cs_clientSocket->Active=false;
        cs_clientSocket->OnRead=csOnRead;
        cs_clientSocket->Host="";
        bt_konektujSe->Caption="Konektuj se";
        ed_adresaServera->Enabled=true;
        this->Show();
}
//---------------------------------------------------------------------------

