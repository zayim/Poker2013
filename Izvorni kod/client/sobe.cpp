//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "sobe.h"
#include "../includes/procotol.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSobe *FormSobe;
//---------------------------------------------------------------------------
__fastcall TFormSobe::TFormSobe(TComponent* Owner)
        : TForm(Owner), odabranaSoba(0), sobeID(NULL)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormSobe::cs_OnRead(TObject* Sender, TCustomWinSocket* Socket)
{
        int velicina=Socket->ReceiveLength();

        Paket* paket;
        paket=(Paket*)malloc(velicina); //paket=(Paket*)malloc(MAX_VELICINA_PAKETA);
        Socket->ReceiveBuf(paket,velicina);

        if (paket->Tip()==pktKorisnikResponse)
        {

                strcpy( dm_dataModule->korisnik.ime, ((PaketKorisnikResponse*)paket)->ime );
                strcpy( dm_dataModule->korisnik.prezime, ((PaketKorisnikResponse*)paket)->prezime );
                dm_dataModule->korisnik.cipovi=((PaketKorisnikResponse*)paket)->cipovi;

                l_ime->Caption = dm_dataModule->korisnik.ime;
                l_prezime->Caption = dm_dataModule->korisnik.prezime;
                l_cipovi->Caption = AnsiString(dm_dataModule->korisnik.cipovi) + " €";
        }
        else if (paket->Tip()==pktSveSobeResponse)
        {
                PaketSveSobeResponse* pkt=(PaketSveSobeResponse*)paket;
                int brojSoba=pkt->brojSoba;

                delete[] sobeID;
                sobeID = new int[brojSoba];

                sg_sobe->RowCount=brojSoba;
                sg_sobe->Rows[0]->Text="";
                for (int i=0; i<brojSoba; i++)
                {
                        sg_sobe->Rows[i]->Text = AnsiString(pkt->sobe[i].ime) + " (" +
                                pkt->sobe[i].brojIgraca + ")";
                        sobeID[i]=pkt->sobe[i].id;
                        strcpy(sobeImena[i],pkt->sobe[i].ime);
                }
        }
        else if(paket->Tip()==pktNapraviSobuResponse)
        {
                FormSobaIgraci->kreator=this;

                FormSobaIgraci->cs_clientSocket=cs_clientSocket;
                FormSobaIgraci->__OnRead=cs_clientSocket->OnRead;
                FormSobaIgraci->cs_clientSocket->OnRead=FormSobaIgraci->cs_OnRead;

                FormSobaIgraci->daLiSamVlasnik=true;
                FormSobaIgraci->daLiSamIzbacen=false;
                //FormSobaIgraci->set_korisnik(dm_dataModule->korisnik);
                FormSobaIgraci->idIgracaUSobi=0;

                FormSobaIgraci->idSobe=((PaketNapraviSobuResponse*)paket)->idSobe;
                strcpy(FormSobaIgraci->imeSobe , ed_imeSobe->Text.c_str() );

                this->Hide();
                FormSobaIgraci->init();
                FormSobaIgraci->Show();
        }
        else if (paket->Tip()==pktUdjiUSobuResponse)
        {
                if ( ((PaketUdjiUSobuResponse*)paket)->dodijeljeniID == -1)
                        ShowMessage("Došlo je do greške! Molimo odaberite drugu sobu!");

                else
                {
                        FormSobaIgraci->kreator=this;

                        FormSobaIgraci->cs_clientSocket=cs_clientSocket;
                        FormSobaIgraci->__OnRead=cs_clientSocket->OnRead;
                        FormSobaIgraci->cs_clientSocket->OnRead=FormSobaIgraci->cs_OnRead;

                        FormSobaIgraci->daLiSamVlasnik=false;
                        FormSobaIgraci->daLiSamIzbacen=false;
                        //FormSobaIgraci->set_korisnik(dm_dataModule->korisnik);
                        FormSobaIgraci->idIgracaUSobi=((PaketUdjiUSobuResponse*)paket)->dodijeljeniID;

                        FormSobaIgraci->idSobe=odabranaSoba;
                        strcpy(FormSobaIgraci->imeSobe , odabranoIme);

                        this->Hide();
                        FormSobaIgraci->init();
                        FormSobaIgraci->Show();
                }
        }


        free(paket);
}
void __fastcall TFormSobe::FormSobeOnClose(TObject *Sender,
      TCloseAction &Action)
{
        odjaviKorisnika();
        Application->Terminate();        
}
//---------------------------------------------------------------------------
void TFormSobe::set_korisnikUsername(const char un[])
{
        strcpy(dm_dataModule->korisnik.username,un);
}
//---------------------------------------------------------------------------
void TFormSobe::init()
{
        PaketKorisnikZahtjev paket;
        strcpy(paket.username,dm_dataModule->korisnik.username);
        cs_clientSocket->Socket->SendBuf(&paket,sizeof(paket));

        Sleep(100);

        PaketZahtjevSveSobe paket2;
        cs_clientSocket->Socket->SendBuf(&paket2,sizeof(paket2));
}
void __fastcall TFormSobe::bt_napraviSobuClick(TObject *Sender)
{
        if (dm_dataModule->korisnik.cipovi < IZNOS_BIG_BLIND)
        {
                ShowMessage("Nemate dovoljno èipova!");
                return;
        }

        PaketNapraviSobu paket;

        strcpy(paket.ime,ed_imeSobe->Text.c_str());
        strcpy(paket.imeKorisnika,dm_dataModule->korisnik.ime);
        strcpy(paket.prezimeKorisnika,dm_dataModule->korisnik.prezime);
        strcpy(paket.usernameKorisnika,dm_dataModule->korisnik.username);
        paket.cipoviKorisnika=dm_dataModule->korisnik.cipovi;
        cs_clientSocket->Socket->SendBuf(&paket,sizeof(paket));
}
void TFormSobe::odjaviKorisnika()
{
        PaketLogout paket;
        strcpy(paket.username,dm_dataModule->korisnik.username);
        paket.cipovi=dm_dataModule->korisnik.cipovi;
        cs_clientSocket->Socket->SendBuf(&paket,sizeof(paket));
}
//---------------------------------------------------------------------------


void __fastcall TFormSobe::bt_osvjeziSobeClick(TObject *Sender)
{
        PaketZahtjevSveSobe pkt;
        cs_clientSocket->Socket->SendBuf(&pkt,sizeof(pkt));
}
//---------------------------------------------------------------------------

void __fastcall TFormSobe::sg_sobeOnSelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
        if (sobeID)
        {
                odabranaSoba = sobeID[ARow];
                strcpy(odabranoIme,sobeImena[ARow]);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormSobe::bt_izaberiSobuClick(TObject *Sender)
{
        if (dm_dataModule->korisnik.cipovi < IZNOS_BIG_BLIND)
        {
                ShowMessage("Nemate dovoljno èipova!");
                return;
        }

        PaketUdjiUSobu pkt(odabranaSoba);
        strcpy(pkt.ime , dm_dataModule->korisnik.ime);
        strcpy(pkt.prezime , dm_dataModule->korisnik.prezime);
        strcpy(pkt.username,dm_dataModule->korisnik.username);
        pkt.cipovi = dm_dataModule->korisnik.cipovi;

        cs_clientSocket->Socket->SendBuf(&pkt,sizeof(pkt));
}
//---------------------------------------------------------------------------

void __fastcall TFormSobe::FormSobeOnShow(TObject *Sender)
{
        l_cipovi->Caption = AnsiString(dm_dataModule->korisnik.cipovi) + " €";
}
//---------------------------------------------------------------------------

