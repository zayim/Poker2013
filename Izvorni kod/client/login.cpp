//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "login.h"
#include "registracija.h"
#include "../includes/procotol.h"
#include "sobe.h"
#include "../includes/konstante.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormLogin *FormLogin;
//---------------------------------------------------------------------------
__fastcall TFormLogin::TFormLogin(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormLogin::FormLoginOnClose(TObject *Sender,
      TCloseAction &Action)
{
        ((TForm*)Owner)->Show();
        cs_clientSocket->OnRead=__OnRead;
}
//---------------------------------------------------------------------------

void __fastcall TFormLogin::l_registrujSeClick(TObject *Sender)
{
        FormRegistracija = new TFormRegistracija(this);
        FormRegistracija->cs_clientSocket=cs_clientSocket;
        FormRegistracija->__OnRead=cs_clientSocket->OnRead;
        FormRegistracija->cs_clientSocket->OnRead=FormRegistracija->cs_OnRead;

        FormRegistracija->Show();
        this->Enabled=false;        
}
//---------------------------------------------------------------------------

void __fastcall TFormLogin::bt_prijaviSeClick(TObject *Sender)
{
        if (ed_username->Text=="" || ed_password->Text=="")
        {
                ShowMessage("Niste unijeli sve podatke!");
                return;
        }

        PaketLogin paket;
        strcpy(paket.username,ed_username->Text.c_str());
        strcpy(paket.password,ed_password->Text.c_str());

        cs_clientSocket->Socket->SendBuf(&paket,sizeof(paket));
}
//---------------------------------------------------------------------------
void __fastcall TFormLogin::cs_OnRead(TObject* Sender, TCustomWinSocket* Socket)
{
        Paket* paket;
        paket = (Paket*)malloc(MAX_VELICINA_PAKETA);

        int velicina=Socket->ReceiveLength();
        Socket->ReceiveBuf(paket,velicina);

        if (paket->Tip() == pktLoginResponse)
        {
                if (((PaketLoginResponse*)paket)->status==PaketLoginResponse::LOGIN_USPJESAN)
                {
                        ShowMessage("Uspješno prijavljen!");

                        this->Close();
                        ((TForm*)Owner)->Hide();

                        FormSobe->cs_clientSocket=cs_clientSocket;
                        FormSobe->__OnRead=cs_clientSocket->OnRead;
                        FormSobe->cs_clientSocket->OnRead=FormSobe->cs_OnRead;
                        FormSobe->set_korisnikUsername(ed_username->Text.c_str());
                        FormSobe->init();
                        FormSobe->Show();
                }
                else if (((PaketLoginResponse*)paket)->status==PaketLoginResponse::VEC_LOGOVAN)
                {
                        ShowMessage("Veæ ste logovani!");
                }
                else
                {
                        ShowMessage("Greška, neispravni podaci!");
                }
        }

        free(paket);
}

