//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "registracija.h"
#include "../includes/procotol.h"
#include "../includes/konstante.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormRegistracija *FormRegistracija;
//---------------------------------------------------------------------------
__fastcall TFormRegistracija::TFormRegistracija(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormRegistracija::FormRegistracijaOnClose(TObject *Sender,
      TCloseAction &Action)
{
        ((TForm*)Owner)->Enabled=true;
        cs_clientSocket->OnRead=__OnRead;
}
//---------------------------------------------------------------------------

void __fastcall TFormRegistracija::bt_registrujSeClick(TObject *Sender)
{
        if (ed_username->Text == "" || ed_password->Text == "" ||
            ed_ime->Text == "" || ed_prezime->Text == "")
        {
                ShowMessage("Niste unijeli sve podatke!");
                return;
        }

        PaketRegistracija paket;
        strcpy(paket.ime,ed_ime->Text.c_str());
        strcpy(paket.prezime,ed_prezime->Text.c_str());
        strcpy(paket.username,ed_username->Text.c_str());
        strcpy(paket.password,ed_password->Text.c_str());

        cs_clientSocket->Socket->SendBuf(&paket,sizeof(paket));
}
//---------------------------------------------------------------------------
void __fastcall TFormRegistracija::cs_OnRead(TObject* Sender, TCustomWinSocket* Socket)
{
        Paket* paket;
        paket = (Paket*)malloc(MAX_VELICINA_PAKETA);

        int velicina=Socket->ReceiveLength();
        Socket->ReceiveBuf(paket,velicina);

        if (paket->Tip() == pktRegistracijaResponse)
        {
                if (((PaketRegistracijaResponse*)paket)->status==true)
                {
                        ShowMessage("Uspješno registrovan!");
                        this->Close();
                }
                else
                {
                        ShowMessage("Korisnièko ime je zauzeto!");
                }
        }

        free(paket);
}

