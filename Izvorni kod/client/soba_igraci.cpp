//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "soba_igraci.h"
#include "igra.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSobaIgraci *FormSobaIgraci;
//---------------------------------------------------------------------------
__fastcall TFormSobaIgraci::TFormSobaIgraci(TComponent* Owner)
        : TForm(Owner), ostaliIgraci(20), izabraniIgracZaBrisanje(-1), daLiSamIzbacen(false)
{
}
void __fastcall TFormSobaIgraci::cs_OnRead(TObject* Sender, TCustomWinSocket* Socket)
{
        int velicina=Socket->ReceiveLength();

        Paket* paket;
        paket=(Paket*)malloc(velicina); //paket=(Paket*)malloc(MAX_VELICINA_PAKETA);
        Socket->ReceiveBuf(paket,velicina);

        if (paket->Tip()==pktSviIgraciUSobiResponse)
        {
                PaketSviIgraciUSobiResponse* paketSviIgraci=(PaketSviIgraciUSobiResponse*)paket;

                sg_igraci->RowCount= brojOstalihIgraca = paketSviIgraci->brojIgraca-1;
                sg_igraci->Rows[0]->Text="";

                int j=0;
                ostaliIgraci.resize(brojOstalihIgraca);
                for (int i=0; i<paketSviIgraci->brojIgraca; i++)
                        if (paketSviIgraci->igraci[i].idUSobi != idIgracaUSobi)
                        {
                                sg_igraci->Rows[j]->Text=AnsiString(paketSviIgraci->igraci[i].korisnik.ime) +
                                        " " + paketSviIgraci->igraci[i].korisnik.prezime + " - " +
                                        paketSviIgraci->igraci[i].korisnik.cipovi + " €";

                                ostaliIgraci[j].korisnik = paketSviIgraci->igraci[i].korisnik;
                                /*strcpy(ostaliIgraci[j].korisnik.ime , paketSviIgraci->igraci[i].korisnik.ime);
                                strcpy(ostaliIgraci[j].korisnik.prezime , paketSviIgraci->igraci[i].korisnik.prezime);
                                ostaliIgraci[j].korisnik.cipovi = paketSviIgraci->igraci[i].korisnik.cipovi;
                                */ostaliIgraci[j].idUSobi = paketSviIgraci->igraci[i].idUSobi;
                                j++;
                        }
        }

        else if (paket->Tip() == pktIzlazakIzSobeNotifikacija)
        {
                PaketIzlazakIzSobeNotifikacija* paketNotif = (PaketIzlazakIzSobeNotifikacija*)paket;

                for (std::vector<Korisnik_ID>::iterator it=ostaliIgraci.begin(); it!=ostaliIgraci.end(); it++)
                {
                        if (it->idUSobi == paketNotif->idIgracaUSobi)
                        {
                                ostaliIgraci.erase(it);
                                brojOstalihIgraca--;
                                break;
                        }
                }

                updateujTabeluIgraca();
        }

        else if (paket->Tip() == pktIzbacenIzSobeNotifikacija)
        {
                ShowMessage("Izbaèeni ste iz sobe!");
                daLiSamIzbacen=true;
                this->Close();
        }

        else if (paket->Tip() == pktPocetakIgreNotifikacija)
        {
                pocniIgru();
        }
}
//---------------------------------------------------------------------------
void __fastcall TFormSobaIgraci::FormSobaIgraciOnClose(TObject *Sender,
      TCloseAction &Action)
{
        kreator->Show();
        cs_clientSocket->OnRead=__OnRead;

        brojOstalihIgraca=0;
        ostaliIgraci.resize(0);
        for (std::vector<Korisnik_ID>::iterator it=ostaliIgraci.begin(); it!=ostaliIgraci.end(); it++)
                ostaliIgraci.erase(it);

        if (daLiSamVlasnik)
        {
                PaketZatvoriSobu pkt(idSobe);
                cs_clientSocket->Socket->SendBuf(&pkt,sizeof(pkt));
        }
        else if(!daLiSamIzbacen)
        {
                PaketIzadjiIzSobe pkt(idSobe,idIgracaUSobi);
                cs_clientSocket->Socket->SendBuf(&pkt,sizeof(pkt));
        }
        else daLiSamIzbacen=false;
}
void TFormSobaIgraci::init()
{
        if (daLiSamVlasnik)
        {
                FormSobaIgraci->bt_izbrisiIgraca->Visible=true;
                FormSobaIgraci->bt_zapocniIgru->Visible=true;
        }
        else
        {
                FormSobaIgraci->bt_izbrisiIgraca->Visible=false;
                FormSobaIgraci->bt_zapocniIgru->Visible=false;
        }

        l_ime->Caption=dm_dataModule->korisnik.ime;
        l_prezime->Caption=dm_dataModule->korisnik.prezime;
        l_cipovi->Caption=AnsiString(dm_dataModule->korisnik.cipovi) + " €";
        this->Caption=AnsiString("Poker 2013 - ") + imeSobe;

        PaketSviIgraciUSobi pkt(idSobe);
        cs_clientSocket->Socket->SendBuf(&pkt,sizeof(pkt));
}
//---------------------------------------------------------------------------
void __fastcall TFormSobaIgraci::Button1Click(TObject *Sender)
{
        AnsiString poruka="";
        poruka+=AnsiString("ID Sobe: ") + idSobe + "; ";
        poruka+=AnsiString("Moje ime: ") + dm_dataModule->korisnik.ime + " " + dm_dataModule->korisnik.prezime + "; ";
        poruka+=AnsiString("Moj ID u sobi: ") + idIgracaUSobi + "; ";
        poruka+=AnsiString("Broj ostalih igraca: ") + brojOstalihIgraca + "; ";
        for (int i=0; i<brojOstalihIgraca; i++)
                poruka+=AnsiString("Igrac ID: ") + ostaliIgraci[i].idUSobi +
                        ", igrac ime: " + ostaliIgraci[i].korisnik.ime + "; ";

        ShowMessage(poruka);
}
//---------------------------------------------------------------------------
void __fastcall TFormSobaIgraci::bt_napustiSobuClick(TObject *Sender)
{
        this->Close();        
}
//---------------------------------------------------------------------------

void __fastcall TFormSobaIgraci::bt_osvjeziClick(TObject *Sender)
{
        PaketSviIgraciUSobi pkt(idSobe);
        cs_clientSocket->Socket->SendBuf(&pkt,sizeof(pkt));        
}
void TFormSobaIgraci::updateujTabeluIgraca()
{
        sg_igraci->RowCount=brojOstalihIgraca;
        sg_igraci->Rows[0]->Text="";

        int j=0;
        for (int i=0; i<brojOstalihIgraca; i++)
                if (ostaliIgraci[i].idUSobi!=idIgracaUSobi)
                {
                        sg_igraci->Rows[j++]->Text=AnsiString(ostaliIgraci[i].korisnik.ime) +
                                        " " + ostaliIgraci[i].korisnik.prezime + " - " +
                                        ostaliIgraci[i].korisnik.cipovi + " €";
                }
}
//---------------------------------------------------------------------------

void __fastcall TFormSobaIgraci::sg_igraciOnSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
        izabraniIgracZaBrisanje = ostaliIgraci[ARow].idUSobi;        
}
//---------------------------------------------------------------------------

void __fastcall TFormSobaIgraci::bt_izbrisiIgracaClick(TObject *Sender)
{
        PaketIzbacenIzSobe pkt(idSobe, izabraniIgracZaBrisanje);
        cs_clientSocket->Socket->SendBuf(&pkt,sizeof(pkt));
}

//---------------------------------------------------------------------------

void __fastcall TFormSobaIgraci::bt_zapocniIgruClick(TObject *Sender)
{
        if (!ostaliIgraci.size())
        {
                ShowMessage("Nema dovoljno igraèa u sobi!");
                return;
        }

        PaketZapocniIgruUSobi pkt(idSobe);
        cs_clientSocket->Socket->SendBuf(&pkt,sizeof(pkt));
}
void TFormSobaIgraci::pocniIgru()
{
        FormIgra->kreator=this;
        FormIgra->cs_clientSocket=cs_clientSocket;
        FormIgra->__OnRead=cs_clientSocket->OnRead;
        FormIgra->cs_clientSocket->OnRead=FormIgra->cs_OnRead;

        FormIgra->dodajIgraca(dm_dataModule->korisnik,idIgracaUSobi);
        FormIgra->mojID=idIgracaUSobi;
        FormIgra->idSobe=idSobe;

        for (int i=0; i<ostaliIgraci.size(); i++)
                FormIgra->dodajIgraca(ostaliIgraci[i].korisnik,ostaliIgraci[i].idUSobi);

        FormIgra->init();
        FormIgra->Show();
        this->Hide();
}
//---------------------------------------------------------------------------

void __fastcall TFormSobaIgraci::TFormSobaIgraciOnShow(TObject *Sender)
{
        std::vector<Korisnik_ID>::iterator it=ostaliIgraci.begin();
        while (!ostaliIgraci.empty())
                it=ostaliIgraci.erase(it);

        ostaliIgraci.resize(0);

        l_cipovi->Caption = AnsiString(dm_dataModule->korisnik.cipovi) + " €";
}
//---------------------------------------------------------------------------

