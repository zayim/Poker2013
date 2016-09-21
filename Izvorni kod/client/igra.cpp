//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "igra.h"
#include <algorithm>
#include <ctime>
#include <fstream>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormIgra *FormIgra;
//---------------------------------------------------------------------------
__fastcall TFormIgra::TFormIgra(TComponent* Owner)
        : TForm(Owner)
{
        srand(time(0));
        kartaDefault = new TJPEGImage;
        kartaDefault->LoadFromFile("slike/karte/default.jpg");
}
//---------------------------------------------------------------------------
void __fastcall TFormIgra::FormIgraOnClose(TObject *Sender,
      TCloseAction &Action)
{
        igraci.resize(0);
        potovi.resize(0);
        kreator->Close();
}
int TFormIgra::indexByID(int id)
{
        for (int i=0; i<igraci.size(); i++)
                if (igraci[i]->idUSobi == id) return i;
        return -1;
}
void TFormIgra::init()
{
        // sortiranje igraca prema ID
        for (int i=0; i<igraci.size()-1; i++)
                for (int j=i+1; j<igraci.size(); j++)
                        if (igraci[i]->idUSobi > igraci[j]->idUSobi)
                        {
                                Igrac* tmp=igraci[i];
                                igraci[i]=igraci[j];
                                igraci[j]=tmp;
                        }
        if (mojID==0)
                dodijeliSlucajnoKarte();

        int mojIND=indexByID(mojID);

        //inicijalizacija stanja
        stadij=stdPreFlop;
        igracBigBlind=igraci.size()-1;
        igracZadnjiRaise=igraci.size()-1;
        igracNaPotezu=0;

        //kreiranje prvog pota
        Pot* prviPot=new Pot(igraci.size());
        prviPot->dodajIndeksIgraca(igracBigBlind);
        prviPot->dodajIndeksIgraca(prethodniIndex(igracBigBlind));

        prviPot->uloziIgraca[igracBigBlind] = IZNOS_BIG_BLIND;
        prviPot->uloziIgraca[prethodniIndex(igracBigBlind)] = IZNOS_SMALL_BLIND;
        prviPot->ulogPoIgracu=IZNOS_BIG_BLIND;
        prviPot->naslov=l_pot1Naslov;
        prviPot->iznos=l_pot1;
        l_pot1->Caption=AnsiString(prviPot->vratiIznosPota()) + " €";

        igraci[igracBigBlind]->cipovi-=IZNOS_BIG_BLIND;
        igraci[prethodniIndex(igracBigBlind)]->cipovi-=IZNOS_SMALL_BLIND;

        if (igracBigBlind == mojIND)
                dm_dataModule->korisnik.cipovi-=IZNOS_BIG_BLIND;
        else if (prethodniIndex(igracBigBlind) == mojIND)
                dm_dataModule->korisnik.cipovi-=IZNOS_SMALL_BLIND;

        potovi.push_back(prviPot);

        int ind=indexByID(mojID); // Igraè 0
        igraci[ind]->canvasK1=im_K1igrac0->Picture;
        igraci[ind]->canvasK2=im_K2igrac0->Picture;
        igraci[ind]->slikaPotez=im_potez0;
        igraci[ind]->l_cipovi=l_cipovi0;
        igraci[ind]->l_ulozeno=l_ulozeno0;
        l_ime0->Caption=AnsiString(igraci[ind]->ime) + " " + igraci[ind]->prezime;
        l_cipovi0->Caption=AnsiString(igraci[ind]->cipovi) + " €";
        l_ulozeno0->Caption=AnsiString(izracunajUkupanUlog(ind)) + " €";

        if (igracNaPotezu==ind)
        {
                bt_raise->Enabled=true;
                bt_fold->Enabled=true;
                bt_call->Enabled=true;
                ed_raise->Enabled=true;
                im_potez0->Visible=true;

                if (igraci.size()==2)
                        bt_call->Caption=AnsiString("Prati (") + (IZNOS_BIG_BLIND-IZNOS_SMALL_BLIND) + " €)";
                else
                        bt_call->Caption=AnsiString("Prati (") + IZNOS_BIG_BLIND + " €)";
        }

        if (igraci.size()>1) // Igraè 1
        {
                ind=sljedeciIndex(ind);
                igraci[ind]->canvasK1=im_K1igrac1->Picture;
                igraci[ind]->canvasK2=im_K2igrac1->Picture;
                igraci[ind]->slikaPotez=im_potez1;

                igraci[ind]->l_cipovi=l_cipovi1;
                igraci[ind]->l_ulozeno=l_ulozeno1;

                l_ime1->Caption=AnsiString(igraci[ind]->ime) + " " + igraci[ind]->prezime;
                l_cipovi1->Caption=AnsiString(igraci[ind]->cipovi) + " €";
                l_ulozeno1->Caption=AnsiString(izracunajUkupanUlog(ind)) + " €";

                if (ind==igracNaPotezu) im_potez1->Visible=true;
        }
        else
        {
                im_K1igrac1->Visible=false;
                im_K2igrac1->Visible=false;

                l_ime1->Visible=false;
                l_cipovi1->Visible=false;
                l_ulozeno1->Visible=false;
        }
        if (igraci.size()>2) // Igraè 2
        {
                ind=sljedeciIndex(ind);
                igraci[ind]->canvasK1=im_K1igrac2->Picture;
                igraci[ind]->canvasK2=im_K2igrac2->Picture;
                igraci[ind]->slikaPotez=im_potez2;

                igraci[ind]->l_cipovi=l_cipovi2;
                igraci[ind]->l_ulozeno=l_ulozeno2;

                l_ime2->Caption=AnsiString(igraci[ind]->ime) + " " + igraci[ind]->prezime;
                l_cipovi2->Caption=AnsiString(igraci[ind]->cipovi) + " €";
                l_ulozeno2->Caption=AnsiString(izracunajUkupanUlog(ind)) + " €";

                if (ind==igracNaPotezu) im_potez2->Visible=true;
        }
        else
        {
                im_K1igrac2->Visible=false;
                im_K2igrac2->Visible=false;

                l_ime2->Visible=false;
                l_cipovi2->Visible=false;
                l_ulozeno2->Visible=false;
        }
        if (igraci.size()>3) // Igraè 3
        {
                ind=sljedeciIndex(ind);
                igraci[ind]->canvasK1=im_K1igrac3->Picture;
                igraci[ind]->canvasK2=im_K2igrac3->Picture;
                igraci[ind]->slikaPotez=im_potez3;

                igraci[ind]->l_cipovi=l_cipovi3;
                igraci[ind]->l_ulozeno=l_ulozeno3;

                l_ime3->Caption=AnsiString(igraci[ind]->ime) + " " + igraci[ind]->prezime;
                l_cipovi3->Caption=AnsiString(igraci[ind]->cipovi) + " €";
                l_ulozeno3->Caption=AnsiString(izracunajUkupanUlog(ind)) + " €";

                if (ind==igracNaPotezu) im_potez3->Visible=true;
        }
        else
        {
                im_K1igrac3->Visible=false;
                im_K2igrac3->Visible=false;

                l_ime3->Visible=false;
                l_cipovi3->Visible=false;
                l_ulozeno3->Visible=false;
        }
        if (igraci.size()>4) // Igraè 4
        {
                ind=sljedeciIndex(ind);
                igraci[ind]->canvasK1=im_K1igrac4->Picture;
                igraci[ind]->canvasK2=im_K2igrac4->Picture;
                igraci[ind]->slikaPotez=im_potez4;

                igraci[ind]->l_cipovi=l_cipovi4;
                igraci[ind]->l_ulozeno=l_ulozeno4;

                l_ime4->Caption=AnsiString(igraci[ind]->ime) + " " + igraci[ind]->prezime;
                l_cipovi4->Caption=AnsiString(igraci[ind]->cipovi) + " €";
                l_ulozeno4->Caption=AnsiString(izracunajUkupanUlog(ind)) + " €";

                if (ind==igracNaPotezu) im_potez4->Visible=true;
        }
        else
        {
                im_K1igrac4->Visible=false;
                im_K2igrac4->Visible=false;

                l_ime4->Visible=false;
                l_cipovi4->Visible=false;
                l_ulozeno4->Visible=false;
        }

}
void TFormIgra::dodijeliSlucajnoKarte()
{
        const int brojKarti=52;
        Karta *karte[brojKarti];

        for (int i=1; i<=13; i++)
        {
                karte[(i-1)*4]=new Karta(list,i);
                karte[(i-1)*4 + 1]=new Karta(mak,i);
                karte[(i-1)*4 + 2]=new Karta(srce,i);
                karte[(i-1)*4 + 3]=new Karta(kocka,i);
        }

        std::random_shuffle(karte,karte+brojKarti);

        PaketKarte pkt(igraci.size(),idSobe);

        for (int i=0; i<2*igraci.size()+5; i++)
        {
                pkt.boje[i]=karte[i]->boja;
                pkt.brojevi[i]=karte[i]->broj;
        }

        cs_clientSocket->Socket->SendBuf(&pkt,sizeof(pkt));

        for (int i=0; i<brojKarti; i++)
                delete karte[i];
}
void __fastcall TFormIgra::cs_OnRead(TObject* Sender, TCustomWinSocket* Socket)
{
        int velicina=Socket->ReceiveLength();

        Paket* paket;
        paket=(Paket*)malloc(velicina);
        Socket->ReceiveBuf(paket,velicina);

        if (paket->Tip() == pktIzbacenIzSobeNotifikacija)
        {
                ShowMessage("Izbaèeni ste iz sobe!");
                FormSobaIgraci->daLiSamIzbacen=true;
                this->Close();
        }
        else if (paket->Tip() == pktIzlazakIzSobeNotifikacija)
        {
                int ind__=indexByID(((PaketIzlazakIzSobeNotifikacija*)paket)->idIgracaUSobi);
                if (ind__!=-1)
                {
                        igraci[ ind__ ]->status=izasao;
                        igraci[ ind__ ]->l_cipovi->Caption = "Odustao";
                        updateujIgricu();
                }
        }
        else if (paket->Tip() == pktKarte)
        {
                rasporediKarte((PaketKarte*)paket);
        }
        else if (paket->Tip() == pktPotez)
        {
                switch ( ((PaketPotez*)paket)->tipPoteza)
                {
                        case potezCall:
                                odigrajCall(((PaketPotez*)paket)->idIgacaUSobi);
                        break;

                        case potezFold:
                                odigrajFold(((PaketPotez*)paket)->idIgacaUSobi);
                        break;

                        case potezRaise:
                                odigrajRaise(((PaketPotez*)paket)->idIgacaUSobi,
                                             ((PaketPotez*)paket)->iznosRaise);
                        break;
                }

                updateujIgricu();
        }

        free(paket);
}
void TFormIgra::rasporediKarte(PaketKarte* pkt)
{
        flop1=Karta(  Boja(pkt->boje[0]), pkt->brojevi[0] );
        flop2=Karta(  Boja(pkt->boje[1]), pkt->brojevi[1] );
        flop3=Karta(  Boja(pkt->boje[2]), pkt->brojevi[2] );
        turn=Karta(  Boja(pkt->boje[3]), pkt->brojevi[3] );
        river=Karta(  Boja(pkt->boje[4]), pkt->brojevi[4] );

        int karta=5;
        for (int i=0; i<igraci.size(); i++)
        {
                igraci[i]->karta1 = Karta(  Boja(pkt->boje[karta]), pkt->brojevi[karta] );
                karta++;
                igraci[i]->karta2 = Karta(  Boja(pkt->boje[karta]), pkt->brojevi[karta] );
                karta++;
        }

        igraci[indexByID(mojID)]->iscrtaj();
}
void TFormIgra::iscrtajFlop()
{
        im_flop1->Picture->Assign( flop1.slika );
        im_flop2->Picture->Assign( flop2.slika );
        im_flop3->Picture->Assign( flop3.slika );
}
void TFormIgra::iscrtajTurn()
{
        im_turn->Picture->Assign( turn.slika );
}
void TFormIgra::iscrtajRiver()
{
        im_river->Picture->Assign( river.slika );
}
void TFormIgra::sakrijFlop()
{
        im_flop1->Picture->Assign( kartaDefault );
        im_flop2->Picture->Assign( kartaDefault );
        im_flop3->Picture->Assign( kartaDefault );
}
void TFormIgra::sakrijTurn()
{
        im_turn->Picture->Assign( kartaDefault );
}
void TFormIgra::sakrijRiver()
{
        im_river->Picture->Assign( kartaDefault );
}
void TFormIgra::sakrijOstaleIgrace()
{
        int mojInd=indexByID(mojID);

        for (int i=0; i<mojInd; i++)
        {
                igraci[i]->canvasK1->Assign( kartaDefault );
                igraci[i]->canvasK2->Assign( kartaDefault );
        }
        for (int i=mojInd+1; i<igraci.size(); i++)
        {
                igraci[i]->canvasK1->Assign( kartaDefault );
                igraci[i]->canvasK2->Assign( kartaDefault );
        }
}
int TFormIgra::izracunajUkupanUlog(int ind)
{
        int ulog=0;
        for (int i=0; i<potovi.size(); i++)
                ulog+=potovi[i]->uloziIgraca[ind];
        return ulog;
}
void  TFormIgra::ispisiIgruUDatoteku()
{
        std::fstream zayim("../igra.txt",std::ios::out);

        zayim << "Broj igraca: " << igraci.size() << std::endl;
        zayim << "Moj ID: " << mojID << std::endl;
        zayim << "----------------" << std::endl;

        zayim << "(";
        switch(flop1.boja)
        {
                case list: zayim << "LIST"; break; case mak: zayim << "MAK"; break;
                case srce: zayim << "SRCE"; break; case kocka: zayim << "KOCKA"; break;
        }
        zayim << "," << flop1.broj << ") ";
        zayim << "(";
        switch(flop2.boja)
        {
                case list: zayim << "LIST"; break; case mak: zayim << "MAK"; break;
                case srce: zayim << "SRCE"; break; case kocka: zayim << "KOCKA"; break;
        }
        zayim << "," << flop2.broj << ") ";
        zayim << "(";
        switch(flop3.boja)
        {
                case list: zayim << "LIST"; break; case mak: zayim << "MAK"; break;
                case srce: zayim << "SRCE"; break; case kocka: zayim << "KOCKA"; break;
        }
        zayim << "," << flop3.broj << ") ";
        zayim << "(";
        switch(turn.boja)
        {
                case list: zayim << "LIST"; break; case mak: zayim << "MAK"; break;
                case srce: zayim << "SRCE"; break; case kocka: zayim << "KOCKA"; break;
        }
        zayim << "," << turn.broj << ") ";
        zayim << "(";
        switch(river.boja)
        {
                case list: zayim << "LIST"; break; case mak: zayim << "MAK"; break;
                case srce: zayim << "SRCE"; break; case kocka: zayim << "KOCKA"; break;
        }
        zayim << "," << river.broj << ") ";

        zayim << "----------------" << std::endl;
        for (int i=0; i<igraci.size(); i++)
        {
                zayim << "Index: " << i << std::endl;
                zayim << "ID: " << igraci[i]->idUSobi << std::endl;
                zayim << "Ime: " << igraci[i]->ime << " " << igraci[i]->prezime << std::endl;

                zayim << "Karta 1: ";
                zayim << "(";
                switch(igraci[i]->karta1.boja)
                {
                        case list: zayim << "LIST"; break; case mak: zayim << "MAK"; break;
                        case srce: zayim << "SRCE"; break; case kocka: zayim << "KOCKA"; break;
                }
                zayim << "," << igraci[i]->karta1.broj << ")" << " // " << igraci[i]->karta1.vratiImeFajla().c_str() << std::endl;

                zayim << "Karta 2: ";
                zayim << "(";
                switch(igraci[i]->karta2.boja)
                {
                        case list: zayim << "LIST"; break; case mak: zayim << "MAK"; break;
                        case srce: zayim << "SRCE"; break; case kocka: zayim << "KOCKA"; break;
                }
                zayim << "," << igraci[i]->karta2.broj << ")" << " // " << igraci[i]->karta2.vratiImeFajla().c_str() << std::endl;

                zayim << "----------------" << std::endl;
        }

        zayim << "Big Blind index: " << igracBigBlind << std::endl;
        zayim << "Zadnji raise index: " << igracZadnjiRaise << std::endl;
        zayim << "Na potezu: " << igracNaPotezu << std::endl;

        zayim << "Broj potova: " << potovi.size() << std::endl;
        zayim << "----------------" << std::endl;
        for (int i=0; i<potovi.size(); i++)
        {
                zayim << "POT " << i << std::endl;

                zayim << "Ulog po igracu: " << potovi[i]->ulogPoIgracu << std::endl;
                zayim << "Broj igraca na potu: " << potovi[i]->brojIgracaNaPotu << std::endl;
                zayim << "Igraci na potu (indexi): ";
                for (int j=0; j<potovi[i]->brojIgracaNaPotu; j++)
                        zayim << potovi[i]->indexiIgracaNaPotu[j] << " ";
                zayim << "Broj igraca (u sobi): " << potovi[i]->brojIgraca << std::endl;
                zayim << "Ulozi igraca: ";
                for (int j=0; j<potovi[i]->brojIgraca; j++)
                        zayim << potovi[i]->uloziIgraca[j] << " ";

                zayim << std::endl << "----------------" << std::endl;
        }

        zayim.close();
}
//---------------------------------------------------------------------------
void __fastcall TFormIgra::Button1Click(TObject *Sender)
{
        ispisiIgruUDatoteku();        
}
//---------------------------------------------------------------------------

void __fastcall TFormIgra::bt_callClick(TObject *Sender)
{
        PaketPotez pkt(potezCall);
        pkt.idSobe=idSobe;
        pkt.idIgacaUSobi=mojID;
        cs_clientSocket->Socket->SendBuf(&pkt,sizeof(pkt));
}
//---------------------------------------------------------------------------

void __fastcall TFormIgra::bt_foldClick(TObject *Sender)
{
        PaketPotez pkt(potezFold);
        pkt.idSobe=idSobe;
        pkt.idIgacaUSobi=mojID;
        cs_clientSocket->Socket->SendBuf(&pkt,sizeof(pkt));
}
//---------------------------------------------------------------------------

void __fastcall TFormIgra::bt_raiseClick(TObject *Sender)
{
        PaketPotez pkt(potezRaise);
        pkt.idSobe=idSobe;
        pkt.idIgacaUSobi=mojID;

        try
        {
                pkt.iznosRaise=ed_raise->Text.ToInt();
                int mojIND=indexByID(mojID);
                int iznosZaCall=0;
                for (int i=0; i<potovi.size(); i++)
                        iznosZaCall+=potovi[i]->ulogPoIgracu - potovi[i]->uloziIgraca[mojIND];

                if (pkt.iznosRaise <= iznosZaCall)
                {
                        ShowMessage("Morate uložiti veæi iznos!");
                        return;
                }
                else if (pkt.iznosRaise > igraci[mojIND]->cipovi)
                {
                        ShowMessage("Nemate dovoljno èipova!");
                        return;
                }


                cs_clientSocket->Socket->SendBuf(&pkt,sizeof(pkt));

        }
        catch(...)
        {
                ShowMessage("Unesite broj!");
        }


}
bool TFormIgra::provjeriValidnostRaiseIznosa(int iznos)
{
        return true;
}
void TFormIgra::odigrajCall(int id__)
{
        //sortiranje potova prema "koliko treba novca uloziti id__ da bi bio u potu"
        // nazovimo to udaljenost - d

        std::fstream nedo("../igra__potovi.txt",std::ios::out);

        int ind__=indexByID(id__);
        int *d=new int[potovi.size()];

        nedo << "CHECK " << ind__ << std::endl;
        nedo << "Broj potova: " << potovi.size() << std::endl;

        for (int i=0; i<potovi.size(); i++){
                d[i]=potovi[i]->ulogPoIgracu-potovi[i]->uloziIgraca[ind__];
                nedo << "d[" << i << "]=" << d[i] << "; ";
        }
        nedo << std::endl;

        for (int i=0; i<potovi.size()-1; i++)
                for (int j=i+1; j<potovi.size(); j++)
                {
                        if (d[i] > d[j] || (d[i]==d[j] && potovi[i]->uloziIgraca[ind__]==0) )
                        {
                                Pot* tmp=potovi[i];
                                potovi[i]=potovi[j];
                                potovi[j]=tmp;
                                std::swap(d[i],d[j]);
                        }
                }
        nedo << "------------" << std::endl;
        // prolazimo kroz sve potove i gledamo moze li igrac uci u njih
        for (int i=0; i<potovi.size(); i++)
        {
                nedo << "Pot " << i << std::endl;
                // ako moze, ubacimo ga u pot
                if (igraci[ind__]->cipovi >= d[i])
                {
                        nedo << "Moze" << std::endl;
                        nedo << "-" << d[i] << std::endl;
                        nedo << "- potovi(i)->ulozi(ind) (before): " <<  potovi[i]->uloziIgraca[ind__] << std::endl;
                        potovi[i]->uloziIgraca[ind__]+=d[i];
                        igraci[ind__]->cipovi-=d[i];
                        potovi[i]->dodajIndeksIgraca(ind__);
                        nedo << "- potovi(i)->ulozi(ind) (after): " <<  potovi[i]->uloziIgraca[ind__] << std::endl;

                        if (ind__ == indexByID(mojID))
                                dm_dataModule->korisnik.cipovi-=d[i];

                }
                // ako ne moze, onda: ako je vec bio u potu, izbacimo ga
                else
                {
                        nedo << "Ne moze" << std::endl;
                        potovi[i]->ukloniIndeksIgraca(ind__);

                        nedo << "Indeksi u trenutnom potu: ";
                        for (int i_=0; i_<potovi[i]->brojIgracaNaPotu; i_++)
                        nedo << "[" << i_ << "]=" << potovi[i]->indexiIgracaNaPotu[i_] << " ";
                        nedo << std::endl;

                        nedo << "---igraci[ind]->cipovi:" << igraci[ind__]->cipovi << std::endl;
                        nedo << "---potoci(i)->uloziIgraca(ind):" << potovi[i]->uloziIgraca[ind__] << std::endl;

                        Pot *tmp=new Pot(igraci.size());

                        tmp->ulogPoIgracu=igraci[ind__]->cipovi + potovi[i]->uloziIgraca[ind__];
                        tmp->uloziIgraca[ind__]=tmp->ulogPoIgracu;
                        tmp->dodajIndeksIgraca(ind__);
                        potovi[i]->ulogPoIgracu-=potovi[i]->uloziIgraca[ind__]+igraci[ind__]->cipovi;
                        potovi[i]->uloziIgraca[ind__]=0;
                        igraci[ind__]->cipovi=0;
                        igraci[ind__]->l_cipovi->Caption = "0 €";
                        igraci[ind__]->status=allIn;
                        if (ind__ == indexByID(mojID))
                                dm_dataModule->korisnik.cipovi=0;

                        nedo << "---tmp->Ulog po igracu: " << tmp->ulogPoIgracu << std::endl;
                        nedo << "---potovi(i)->Ulog po I: " << potovi[i]->ulogPoIgracu << std::endl;

                        for (int j=0; j<potovi[i]->brojIgracaNaPotu; j++)
                        {
                                if (potovi[i]->uloziIgraca[ potovi[i]->indexiIgracaNaPotu[j] ] > tmp->ulogPoIgracu)
                                {
                                        potovi[i]->uloziIgraca[potovi[i]->indexiIgracaNaPotu[j]]-=tmp->ulogPoIgracu;
                                        tmp->uloziIgraca[potovi[i]->indexiIgracaNaPotu[j]]=tmp->ulogPoIgracu;
                                        tmp->dodajIndeksIgraca(potovi[i]->indexiIgracaNaPotu[j]);
                                }
                                else
                                {
                                        tmp->uloziIgraca[potovi[i]->indexiIgracaNaPotu[j]]=potovi[i]->uloziIgraca[potovi[i]->indexiIgracaNaPotu[j]];
                                        potovi[i]->uloziIgraca[potovi[i]->indexiIgracaNaPotu[j]]=0;
                                        tmp->dodajIndeksIgraca(potovi[i]->indexiIgracaNaPotu[j]);
                                        if (potovi[i]->ukloniIndeksIgraca(potovi[i]->indexiIgracaNaPotu[j])) j--;   /////////////
                                }
                        }

                        potovi.push_back(tmp);
                        switch(potovi.size())
                        {
                                case 2: tmp->naslov=l_pot2Naslov; tmp->iznos=l_pot2; break;
                                case 3: tmp->naslov=l_pot3Naslov; tmp->iznos=l_pot3; break;
                                case 4: tmp->naslov=l_pot4Naslov; tmp->iznos=l_pot4; break;
                                case 5: tmp->naslov=l_pot5Naslov; tmp->iznos=l_pot5; break;
                        }
                        break;
                }
        }

        int ukupanUlog__=0;
        for (int i=0; i<potovi.size(); i++)
        {
                potovi[i]->naslov->Caption=AnsiString("POT ") + (i+1);
                potovi[i]->iznos->Caption=AnsiString(potovi[i]->vratiIznosPota()) + " €";

                potovi[i]->naslov->Visible=true;
                potovi[i]->iznos->Visible=true;

                ukupanUlog__+=potovi[i]->uloziIgraca[ind__];
        }

        igraci[ind__]->l_cipovi->Caption = AnsiString(igraci[ind__]->cipovi) + " €";
        igraci[ind__]->l_ulozeno->Caption = AnsiString(ukupanUlog__) + " €";

        if (igraci[ind__]->cipovi==0)
                igraci[ind__]->status=allIn;

        delete[] d;
}
void TFormIgra::odigrajFold(int id__)
{
        int ind__=indexByID(id__);

        igraci[ind__]->status=odustao;
        igraci[ind__]->l_ulozeno->Caption="Odustao";

        for (int i=0; i<potovi.size(); i++)
                potovi[i]->ukloniIndeksIgraca(ind__);

        if (ind__ == igracZadnjiRaise)
        {
                while (igraci[igracZadnjiRaise]->status != igra)
                        igracZadnjiRaise=prethodniIndex(igracZadnjiRaise);
        }
}
void TFormIgra::odigrajRaise(int id__, int iznos__)
{
        int ind__=indexByID(id__);

        int indexPotaSaNajmanjeIgraca=0;
        for (int i=1; i<potovi.size(); i++)
        {
                if (potovi[i]->brojIgracaNaPotu < potovi[indexPotaSaNajmanjeIgraca]->brojIgracaNaPotu)
                        indexPotaSaNajmanjeIgraca=i;
                else if (potovi[i]->brojIgracaNaPotu == potovi[indexPotaSaNajmanjeIgraca]->brojIgracaNaPotu)
                {
                        int di=potovi[i]->ulogPoIgracu - potovi[i]->uloziIgraca[ind__];
                        int dmax=potovi[indexPotaSaNajmanjeIgraca]->ulogPoIgracu - potovi[indexPotaSaNajmanjeIgraca]->uloziIgraca[ind__];

                        if (di>dmax)
                                indexPotaSaNajmanjeIgraca=i;
                }
        }

        int d=potovi[indexPotaSaNajmanjeIgraca]->ulogPoIgracu - potovi[indexPotaSaNajmanjeIgraca]->uloziIgraca[ind__];
        potovi[indexPotaSaNajmanjeIgraca]->uloziIgraca[ind__]+=iznos__;
        potovi[indexPotaSaNajmanjeIgraca]->ulogPoIgracu+=iznos__-d;
        potovi[indexPotaSaNajmanjeIgraca]->iznos->Caption=
                AnsiString(potovi[indexPotaSaNajmanjeIgraca]->vratiIznosPota()) + " €";

        igraci[ind__]->cipovi-=iznos__;
        if (id__==mojID)
                dm_dataModule->korisnik.cipovi-=iznos__;
        igraci[ind__]->l_cipovi->Caption=AnsiString(igraci[ind__]->cipovi) + " €";

        int ukupanUlog__=0;
        for (int i=0; i<potovi.size(); i++)
                ukupanUlog__+=potovi[i]->uloziIgraca[ind__];
        igraci[ind__]->l_ulozeno->Caption=AnsiString(ukupanUlog__) + " €";

        igracZadnjiRaise=ind__;

        if (igraci[ind__]->cipovi==0)
                igraci[ind__]->status=allIn;

}
void TFormIgra::updateujIgricu()
{
        igraci[igracNaPotezu]->slikaPotez->Visible=false;

        if (igracNaPotezu==indexByID(mojID))
        {
                bt_raise->Enabled=false;
                bt_fold->Enabled=false;
                bt_call->Enabled=false;
                ed_raise->Enabled=false;
                im_potez0->Visible=false;
        }

        int tmp=igracNaPotezu;
        do
        {
                igracNaPotezu=sljedeciIndex(igracNaPotezu);
        } while (igraci[igracNaPotezu]->status!=igra && igracNaPotezu!=tmp);

        if (igracNaPotezu==tmp)
        {
                zavrsiPartiju();
                stadij=stdPreFlop;
                return;
        }

        else if (igracNaPotezu==igracZadnjiRaise)
        {
                //završi jedan krug
                switch (stadij)
                {
                        case stdPreFlop:
                        iscrtajFlop();
                        stadij=stdFlop;
                        break;

                        case stdFlop:
                        iscrtajTurn();
                        stadij=stdTurn;
                        break;

                        case stdTurn:
                        iscrtajRiver();
                        stadij=stdRiver;
                        break;

                        case stdRiver:
                        zavrsiPartiju();
                        stadij=stdPreFlop;
                        return;
                        break;
                }
        }

        int brojIgracaKojiTrebajuIgrati=0,indexOnogKojiTrebaIgrati=0;
        for (int i=0; i<igraci.size(); i++)
                if (igraci[i]->status==igra)
                {
                        brojIgracaKojiTrebajuIgrati++;
                        indexOnogKojiTrebaIgrati=i;
                }
        if (brojIgracaKojiTrebajuIgrati==0)
        {
                zavrsiPartiju();
                stadij=stdPreFlop;
                return;
        }
        else if (brojIgracaKojiTrebajuIgrati==1)
        {
                int d=0;
                for (int i=0; i<potovi.size(); i++)
                        d+=potovi[i]->ulogPoIgracu - potovi[i]->uloziIgraca[indexOnogKojiTrebaIgrati];

                if (d==0)
                {
                        zavrsiPartiju();
                        stadij=stdPreFlop;
                        return;
                }
        }

        igraci[igracNaPotezu]->slikaPotez->Visible=true;
        if (igracNaPotezu==indexByID(mojID))
        {
                bt_raise->Enabled=true;
                bt_fold->Enabled=true;
                bt_call->Enabled=true;
                ed_raise->Enabled=true;
                im_potez0->Visible=true;

                int iznosZaCall=0;
                for (int i=0; i<potovi.size(); i++)
                        iznosZaCall+=potovi[i]->ulogPoIgracu - potovi[i]->uloziIgraca[igracNaPotezu];
                if (iznosZaCall >= igraci[igracNaPotezu]->cipovi)
                {
                        iznosZaCall=igraci[igracNaPotezu]->cipovi;
                        bt_raise->Enabled=false;
                        bt_call->Caption=AnsiString("Uloži sve (") + iznosZaCall + " €)";
                }

                else if (iznosZaCall)
                        bt_call->Caption=AnsiString("Prati (") + iznosZaCall + " €)";
                else
                        bt_call->Caption="Èekaj";
        }
}
void TFormIgra::zavrsiPartiju()
{
        l_pobjednik->Visible=true;
        l_pobjednikNaslov->Visible=true;

        int mojIND=indexByID(mojID);
        if (igraci[mojIND]->status!=odustao)
        {
                SkupKarti mojSkup(flop1,flop2,flop3,turn,river,
                                  igraci[mojIND]->karta1,igraci[mojIND]->karta2);
                l_handName->Caption=mojSkup.vratiImeHanda();
                l_handName->Visible=true;
                l_handName->Repaint();
        }


        for (int i=0; i<igraci.size(); i++)
                if (igraci[i]->status == igra || igraci[i]->status == allIn)
                        igraci[i]->iscrtaj();

        iscrtajFlop(); iscrtajTurn(); iscrtajRiver();

        for (int i=0; i<potovi.size(); i++)
        {
                l_pobjednikNaslov->Caption=AnsiString("POT ") + (i+1) + " - pobjednik";
                int brojPobjednika=1;
                int indeksiPobjednika[20];

                int pom=0;
                while(igraci[potovi[i]->indexiIgracaNaPotu[pom]]->status==odustao ||
                            igraci[potovi[i]->indexiIgracaNaPotu[pom]]->status==izasao) pom++;
                indeksiPobjednika[0]=potovi[i]->indexiIgracaNaPotu[pom];

                SkupKarti pobjednickiSkupKarti(flop1,flop2,flop3,turn,river,
                        igraci[potovi[i]->indexiIgracaNaPotu[pom]]->karta1,
                        igraci[potovi[i]->indexiIgracaNaPotu[pom]]->karta2);

                for (int j=pom+1; j<potovi[i]->brojIgracaNaPotu; j++)
                {
                        if (igraci[potovi[i]->indexiIgracaNaPotu[j]]->status==igra ||
                            igraci[potovi[i]->indexiIgracaNaPotu[j]]->status==allIn)
                        {
                        SkupKarti tmp(flop1,flop2,flop3,turn,river,
                        igraci[potovi[i]->indexiIgracaNaPotu[j]]->karta1,
                        igraci[potovi[i]->indexiIgracaNaPotu[j]]->karta2);

                        if (tmp > pobjednickiSkupKarti)
                        {
                                pobjednickiSkupKarti=tmp;
                                brojPobjednika=1;
                                indeksiPobjednika[0]=potovi[i]->indexiIgracaNaPotu[j];
                        }
                        else if (tmp==pobjednickiSkupKarti)
                                indeksiPobjednika[brojPobjednika++]=potovi[i]->indexiIgracaNaPotu[j];
                        }
                }

                int dobitak=potovi[i]->vratiIznosPota() / brojPobjednika;
                for (int j=0; j<brojPobjednika; j++)
                {
                        igraci[ indeksiPobjednika[j] ]->cipovi+=dobitak;
                        if (indeksiPobjednika[j] == mojIND)
                                dm_dataModule->korisnik.cipovi+=dobitak;
                        l_pobjednik->Caption=AnsiString("(") + (j+1) + ") " + igraci[ indeksiPobjednika[j] ]->ime + " "
                                + igraci[ indeksiPobjednika[j] ]->prezime + " - " + dobitak + " €";
                        igraci[indeksiPobjednika[j]]->l_cipovi->Caption =
                                AnsiString(igraci[indeksiPobjednika[j]]->cipovi) + " €";

                        l_pobjednik->Repaint();
                        l_pobjednikNaslov->Repaint();
                        igraci[indeksiPobjednika[j]]->l_cipovi->Repaint();
                        Sleep(3500);
                }
        }

        for (int i=0; i<igraci.size(); i++)
                igraci[i]->l_cipovi->Caption = AnsiString(igraci[i]->cipovi) + " €";

        Sleep(2000);

        l_pobjednik->Visible=false;
        l_pobjednikNaslov->Visible=false;
        l_handName->Visible=false;
        l_handName->Repaint();
        l_pobjednik->Repaint();
        l_pobjednikNaslov->Repaint();

        if ( igraci[mojIND]->cipovi < IZNOS_BIG_BLIND)
        {
                this->Close();
                ShowMessage("Nemate dovoljno cipova!");
                return;
        }

        // inicijalizacija nove partije

        sakrijFlop(); sakrijTurn(); sakrijRiver();
        for (std::vector<Igrac*>::iterator it=igraci.begin(); it!=igraci.end(); it++)
        {
                (*it)->canvasK1->Assign( kartaDefault );
                (*it)->canvasK2->Assign( kartaDefault );

                (*it)->slikaPotez->Visible=false;
                (*it)->l_ulozeno->Caption="0 €";

                if ((*it)->status!=izasao && (*it)->cipovi>=IZNOS_BIG_BLIND)
                        (*it)->status=igra;
                else
                {
                        int index=std::distance(igraci.begin(), it);
                        if (igracZadnjiRaise > index)
                                igracZadnjiRaise=prethodniIndex(igracZadnjiRaise);
                        (*it)->slikaPotez->Visible=false;
                        (*it)->l_ulozeno->Visible=false;
                        (*it)->l_cipovi->Caption = "Odustao";
                        it=igraci.erase(it);
                        if (it!=igraci.begin()) it--;
                        if (igraci.size() < 2)
                        {
                                ShowMessage("Nedovoljno igraèa u sobi!");
                                this->Close();
                                return;
                        }
                }
        }

        if (mojID==0)
                dodijeliSlucajnoKarte();

        stadij=stdPreFlop;
        for (int i=0; i<potovi.size(); i++)
        {
                potovi[i]->naslov->Caption=""; potovi[i]->naslov->Visible=false;
                potovi[i]->iznos->Caption=""; potovi[i]->iznos->Visible=false;
        }
        potovi.resize(0);

        igracBigBlind=sljedeciIndex(igracBigBlind);
        igracZadnjiRaise=igracBigBlind;
        igracNaPotezu=sljedeciIndex(igracBigBlind);
        igraci[igracNaPotezu]->slikaPotez->Visible=true;

        mojIND=indexByID(mojID);
        if (igracNaPotezu == mojIND)
        {
                bt_raise->Enabled=true;
                bt_fold->Enabled=true;
                bt_call->Enabled=true;
                ed_raise->Enabled=true;
                im_potez0->Visible=true;

                if (igraci.size()==2)
                        bt_call->Caption=AnsiString("Prati (") + (IZNOS_BIG_BLIND-IZNOS_SMALL_BLIND) + " €)";
                else
                        bt_call->Caption=AnsiString("Prati (") + IZNOS_BIG_BLIND + " €)";
        }

        //kreiranje prvog pota
        Pot* prviPot=new Pot(igraci.size());
        prviPot->dodajIndeksIgraca(igracBigBlind);
        prviPot->dodajIndeksIgraca(prethodniIndex(igracBigBlind));

        prviPot->uloziIgraca[igracBigBlind] = IZNOS_BIG_BLIND;
        prviPot->uloziIgraca[prethodniIndex(igracBigBlind)] = IZNOS_SMALL_BLIND;
        prviPot->ulogPoIgracu=IZNOS_BIG_BLIND;
        prviPot->naslov=l_pot1Naslov;
        prviPot->iznos=l_pot1;
        prviPot->naslov->Caption="POT 1";
        prviPot->iznos->Caption=AnsiString(prviPot->vratiIznosPota()) + " €";
        prviPot->iznos->Visible=true;
        prviPot->naslov->Visible=true;

        igraci[igracBigBlind]->cipovi-=IZNOS_BIG_BLIND;
        igraci[prethodniIndex(igracBigBlind)]->cipovi-=IZNOS_SMALL_BLIND;

        if (igracBigBlind == mojIND)
                dm_dataModule->korisnik.cipovi-=IZNOS_BIG_BLIND;
        else if (prethodniIndex(igracBigBlind) == mojIND)
                dm_dataModule->korisnik.cipovi-=IZNOS_SMALL_BLIND;

        potovi.push_back(prviPot);

        igraci[igracBigBlind]->l_ulozeno->Caption=AnsiString(IZNOS_BIG_BLIND) + " €";
        igraci[prethodniIndex(igracBigBlind)]->l_ulozeno->Caption=AnsiString(IZNOS_SMALL_BLIND) + " €";
        igraci[igracBigBlind]->l_cipovi->Caption = AnsiString(igraci[igracBigBlind]->cipovi) + " €";
        igraci[prethodniIndex(igracBigBlind)]->l_cipovi->Caption =
                AnsiString(igraci[prethodniIndex(igracBigBlind)]->cipovi) + " €";
}
void TFormIgra::foo()
{
        SkupKarti::__Karta k[7];
        int bj[7],br[7];
        char zarez;

        std::fstream in("../testIN.txt",std::ios::in);
        for (int i=0; i<7; i++)
        {
                in >> bj[i] >> zarez >> br[i];
                k[i].boja=Boja(bj[i]);
                k[i].broj=br[i];
        }
        SkupKarti sk(k[0],k[1],k[2],k[3],k[4],k[5],k[6]);

        for (int i=0; i<7; i++)
        {
                in >> bj[i] >> zarez >> br[i];
                k[i].boja=Boja(bj[i]);
                k[i].broj=br[i];
        }
        SkupKarti sk2(k[0],k[1],k[2],k[3],k[4],k[5],k[6]);
        in.close();

        std::fstream out("../testOUT.txt",std::ios::out);
        for (int i=0; i<4; i++)
        {
                for (int j=0; j<13; j++)
                        out << sk.bits[i*13+j];
                out << "  ";
        }
        out << std::endl;
        for (int i=0; i<4; i++)
        {
                for (int j=0; j<13; j++)
                        out << sk2.bits[i*13+j];
                out << "  ";
        }
        out << std::endl;
        out << sk.vratiImeHanda().c_str() << std::endl;
        for (int i=0; i<5; i++)
                out << int(sk.hand.karte[i].boja) << "," << sk.hand.karte[i].broj << " ";
        out << std::endl;

        out << sk2.vratiImeHanda().c_str() << std::endl;
        for (int i=0; i<5; i++)
                out << int(sk2.hand.karte[i].boja) << "," << sk2.hand.karte[i].broj << " ";
        out << std::endl;

        if (sk > sk2) out << "Veci prvi!" << std::endl;
        else if (sk==sk2) out << "Isti su!" << std::endl;
        else if (sk2 > sk) out << "Veci drugi!" << std::endl;
        else out << "Nista!" << std::endl;

        out.close();
}
//---------------------------------------------------------------------------


