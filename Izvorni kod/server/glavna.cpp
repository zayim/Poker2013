//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#define paketReg ((PaketRegistracija*)paket)
#define paketLogin ((PaketLogin*)paket)
#define paketLogout ((PaketLogout*)paket)
#define paketKorZahtjev ((PaketKorisnikZahtjev*)paket)
#define paketNapraviSobu ((PaketNapraviSobu*)paket)
#define paketZahtjevSveSobe ((PaketZahtjevSveSobe*)paket)
#define paketZatvoriSobu ((PaketZatvoriSobu*)paket)
#define paketUdjiUSobu ((PaketUdjiUSobu*)paket)
#define paketSviIgraciUSobi ((PaketSviIgraciUSobi*)paket)
#define paketIzadjiIzSobe ((PaketIzadjiIzSobe*)paket)
#define paketIzbacenIzSobe ((PaketIzbacenIzSobe*)paket)
#define paketZapocniIgru ((PaketZapocniIgruUSobi*)paket)
#define paketKarte ((PaketKarte*)paket)
#define paketPotez ((PaketPotez*)paket)

#include "glavna.h"
#include "../includes/konstante.h"
#include <fstream>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormGlavna *FormGlavna;
//---------------------------------------------------------------------------
__fastcall TFormGlavna::TFormGlavna(TComponent* Owner)
        : TForm(Owner), nextID(0)
{
        fstream datoteka("sleep_time.txt",ios::in);
        datoteka >> sleepTime__;
        datoteka.close();
}
AnsiString TFormGlavna::trenutnoVrijeme()
{
        return Time().CurrentTime().TimeString();
}
vector<Soba*>::iterator TFormGlavna::nadjiSobuPremaIDu(int idSobe)
{
        for (vector<Soba*>::iterator it=sobe.begin(); it!=sobe.end(); it++)
                if ((*it)->id == idSobe) return it;
        return sobe.end();
}
//---------------------------------------------------------------------------
void __fastcall TFormGlavna::bt_pokreniServerClick(TObject *Sender)
{
        if (!ss_serverSocket->Active)
        {
                ss_serverSocket->Active=true;
                me_log->Lines->Add(trenutnoVrijeme() + " | Server pokrenut!");

                bt_pokreniServer->Caption="Zaustavi server";
        }
        else
        {
                for (int i=0; i<ss_serverSocket->Socket->ActiveConnections; i++)
                        ss_serverSocket->Socket->Connections[i]->Close();
                ss_serverSocket->Active=false;

                odjaviSveKorisnike();

                me_log->Lines->Add(trenutnoVrijeme() + " | Server zaustavljen!");
                bt_pokreniServer->Caption="Pokreni server";
        }
}
void TFormGlavna::odjaviSveKorisnike()
{
        for (vector<AnsiString>::iterator it=logovaniKorisnici.begin(); it!=logovaniKorisnici.end(); it++)
        {
                me_log->Lines->Add(trenutnoVrijeme() + " | Odjavljujem korisnika: " + *it);

                char imeDatoteke[255];
                strcpy(imeDatoteke,"korisnici/");
                strcat(imeDatoteke,it->c_str());
                strcat(imeDatoteke,"_login_status.txt");

                ifstream file(imeDatoteke);
                if (file)
                {
                        fstream datoteka(imeDatoteke,ios::out);
                        datoteka << 0 << endl;
                        datoteka.close();

                }
        }
}
//---------------------------------------------------------------------------
void __fastcall TFormGlavna::ss_OnClientConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
        me_log->Lines->Add(trenutnoVrijeme() + " | Konektovan klijent: " + Socket->RemoteHost);

        PaketKonekcijaResponse pkt;
        pkt.status=true;
        Socket->SendBuf(&pkt,sizeof(pkt));
}
//---------------------------------------------------------------------------
void __fastcall TFormGlavna::ss_OnClientDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
        me_log->Lines->Add(trenutnoVrijeme() + " | Diskonektovan klijent: " + Socket->RemoteHost);
}
//---------------------------------------------------------------------------

void __fastcall TFormGlavna::ss_OnClientRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
        int velicina=Socket->ReceiveLength();

        Paket* paket;
        paket = (Paket*)malloc(velicina);
        Socket->ReceiveBuf(paket,velicina);

        switch(paket->Tip())
        {
                case pktRegistracija:
                        registrujKorisnika(paketReg,Socket);
                break;

                case pktLogin:
                        prijaviKorisnika(paketLogin,Socket);
                break;

                case pktLogout:
                        odjaviKorisnika(paketLogout,Socket);
                break;

                case pktKorisnikZahtjev:
                        obradiKorisnikZahtjev(paketKorZahtjev,Socket);
                break;

                case pktNapraviSobu:
                        obradiNapraviSobu(paketNapraviSobu,Socket);
                break;

                case pktZahtjevSveSobe:
                        obradiZahtjevSveSobe(paketZahtjevSveSobe,Socket);
                break;

                case pktZatvoriSobu:
                        obradiZatvoriSobu(paketZatvoriSobu,Socket);
                break;

                case pktUdjiUSobu:
                        obradiUdjiUSobu(paketUdjiUSobu,Socket);
                break;

                case pktSviIgraciUSobi:
                        obradiSviIgraciUSobi(paketSviIgraciUSobi,Socket);
                break;

                case pktIzadjiIzSobe:
                        obradiIzadjiIzSobe(paketIzadjiIzSobe,Socket);
                break;

                case pktIzbacenIzSobe:
                        obradiIzbacenIzSobe(paketIzbacenIzSobe,Socket);
                break;

                case pktZapocniIgruUSobi:
                        obradiZapocniIgruUSobi(paketZapocniIgru,Socket);
                break;

                case pktKarte:
                        obradiKarte(paketKarte,Socket);
                break;

                case pktPotez:
                        obradiPotez(paketPotez,Socket);
                break;
        }

        free(paket);        
}
//---------------------------------------------------------------------------
void TFormGlavna::registrujKorisnika(PaketRegistracija* paket, TCustomWinSocket* korisnik)
{
        me_log->Lines->Add(trenutnoVrijeme() + " | Zahtjev za registraciju: " + paket->username);

        char imeDatoteke[255];
        strcpy(imeDatoteke,"korisnici/");
        strcat(imeDatoteke,paket->username);
        strcat(imeDatoteke,".txt");

        bool ok=false;

        ifstream file(imeDatoteke);
        if (!file)
        {
                fstream datoteka(imeDatoteke,ios::out);
                datoteka << paket->password << endl;
                datoteka << paket->username << endl;
                datoteka << paket->ime << endl;
                datoteka << paket->prezime << endl;
                datoteka << Korisnik::POCETNI_BROJ_CIPOVA << endl;
                datoteka.close();

                strcpy(imeDatoteke,"korisnici/");
                strcat(imeDatoteke,paket->username);
                strcat(imeDatoteke,"_login_status.txt");
                datoteka.open(imeDatoteke,ios::out);
                datoteka << 0 << endl;
                datoteka.close();

                ok=true;
        }

        PaketRegistracijaResponse p(ok);
        korisnik->SendBuf(&p,sizeof(p));

        me_log->Lines->Add(trenutnoVrijeme() + " | Zahtjev za registraciju (" +
                        paket->username + ") obraðen, status: " + (ok ? "Y" : "N") );
}
void TFormGlavna::prijaviKorisnika(PaketLogin* paket, TCustomWinSocket* korisnik)
{
        me_log->Lines->Add(trenutnoVrijeme() + " | Zahtjev za login: " + paket->username);

        char imeDatoteke[255];
        strcpy(imeDatoteke,"korisnici/");
        strcat(imeDatoteke,paket->username);
        strcat(imeDatoteke,".txt");

        int ok=PaketLoginResponse::LOGIN_POGRESAN;

        ifstream file(imeDatoteke);
        if (file)
        {
                fstream datoteka(imeDatoteke,ios::in);

                char pw[20];
                datoteka >> pw;
                datoteka.close();
                if (!strcmp(pw,paket->password))
                {
                        strcpy(imeDatoteke,"korisnici/");
                        strcat(imeDatoteke,paket->username);
                        strcat(imeDatoteke,"_login_status.txt");

                        datoteka.open(imeDatoteke,ios::in);
                        int loginStatus;
                        datoteka >> loginStatus;
                        datoteka.close();

                        if (loginStatus==0)
                        {
                                datoteka.open(imeDatoteke,ios::out);
                                datoteka << 1;
                                datoteka.close();
                                ok=PaketLoginResponse::LOGIN_USPJESAN;
                                logovaniKorisnici.push_back(AnsiString(paket->username));
                        }
                        else ok=PaketLoginResponse::VEC_LOGOVAN;
                }
        }

        PaketLoginResponse p(ok);
        korisnik->SendBuf(&p,sizeof(p));

        me_log->Lines->Add(trenutnoVrijeme() + " | Zahtjev za login (" +
                        paket->username + ") obraðen, status: " +
                        (ok==PaketLoginResponse::LOGIN_USPJESAN ? "OK" :
                                (ok == PaketLoginResponse::VEC_LOGOVAN ? "LOGGED_IN" : "BAD")) );
}
void TFormGlavna::odjaviKorisnika(PaketLogout* paket, TCustomWinSocket* korisnik)
{
        me_log->Lines->Add(trenutnoVrijeme() + " | Zahtjev za logout: " + paket->username
                + " (" + paket->cipovi + ")");

        int cipovi=paket->cipovi;

        char imeDatoteke[255];
        strcpy(imeDatoteke,"korisnici/");
        strcat(imeDatoteke,paket->username);
        strcat(imeDatoteke,"_login_status.txt");

        ifstream file(imeDatoteke);
        if (file)
        {
                file.close();
                fstream datoteka(imeDatoteke,ios::out);
                datoteka << 0 << endl;
                datoteka.close();

                for (vector<AnsiString>::iterator it=logovaniKorisnici.begin(); it!=logovaniKorisnici.end(); it++)
                        if ( !strcmp(it->c_str(),paket->username) )
                        {
                                logovaniKorisnici.erase(it);
                                break;
                        }
        }

        strcpy(imeDatoteke,"korisnici/");
        strcat(imeDatoteke,paket->username);
        strcat(imeDatoteke,".txt");

        ifstream file2(imeDatoteke);
        if (file2)
        {
                file2.close();
                char un[20],pw[20],im[10],pr[20];
                fstream datoteka(imeDatoteke,ios::in);
                datoteka >> un >> pw >> im >> pr;
                datoteka.close();

                datoteka.open(imeDatoteke,ios::out);
                datoteka << un << endl << pw << endl << im << endl << pr << endl << cipovi << endl;
                datoteka.close();
        }
}
void TFormGlavna::obradiKorisnikZahtjev(PaketKorisnikZahtjev* paket, TCustomWinSocket* korisnik)
{
        me_log->Lines->Add(trenutnoVrijeme() + " | Zahtjev za korisnika: " + paket->username);

        char imeDatoteke[255];
        strcpy(imeDatoteke,"korisnici/");
        strcat(imeDatoteke,paket->username);
        strcat(imeDatoteke,".txt");

        PaketKorisnikResponse p(false);

        ifstream file(imeDatoteke);
        if (file)
        {
                fstream datoteka(imeDatoteke,ios::in);
                p.status=true;

                datoteka >> p.username >> p.username >> p.ime >> p.prezime >> p.cipovi;
        }

        korisnik->SendBuf(&p,sizeof(p));

        me_log->Lines->Add(trenutnoVrijeme() + " | Zahtjev za korisnika (" +
                        paket->username + ") obraðen, status: " + (p.status ? "Y" : "N") );
}

void TFormGlavna::obradiNapraviSobu(PaketNapraviSobu* paket, TCustomWinSocket* korisnik)
{
        me_log->Lines->Add(trenutnoVrijeme() + " | Zahtjev za napravi sobu: " + paket->ime);

        Soba* tmp=new Soba(nextID++,paket->ime);
        tmp->dodajIgraca(korisnik,paket->usernameKorisnika ,paket->imeKorisnika,paket->prezimeKorisnika,paket->cipoviKorisnika,me_log);

        sobe.push_back(tmp);

        PaketNapraviSobuResponse pkt(tmp->id);
        korisnik->SendBuf(&pkt,sizeof(pkt));

        me_log->Lines->Add(trenutnoVrijeme() + " | Zahtjev za napravi sobu (" + paket->ime +
                        ") obradjen, ID: " + tmp->id);
}

void TFormGlavna::obradiZahtjevSveSobe(PaketZahtjevSveSobe* paket, TCustomWinSocket* korisnik)
{
        me_log->Lines->Add(trenutnoVrijeme() + " | Zahtjev za sve sobe: " + korisnik->RemoteHost);

        int brojSoba=sobe.size();

        PaketSveSobeResponse pkt(brojSoba);
        int j=0;
        for (int i=0; i<brojSoba; i++)
        {
                if (!sobe[i]->igraUToku && sobe[i]->brojIgraca!=sobe[i]->MAX_BROJ_IGRACA)
                {
                        pkt.sobe[j].id = sobe[i]->id;
                        pkt.sobe[j].brojIgraca = sobe[i]->brojIgraca;
                        strcpy(pkt.sobe[j].ime , sobe[i]->ime);
                        j++;
                }
        }

        int vel=korisnik->SendBuf(&pkt,sizeof(pkt));

        me_log->Lines->Add(trenutnoVrijeme() + " | Poslane sobe (" + j + ") klijentu: "
                        + korisnik->RemoteHost + " // SIZE: " + vel);
}

void TFormGlavna::obradiZatvoriSobu(PaketZatvoriSobu* paket, TCustomWinSocket* korisnik)
{
        me_log->Lines->Add(trenutnoVrijeme() + " | Zahtjev za zatvaranje sobe: " + paket->idSobe);

        vector<Soba*>::iterator it=nadjiSobuPremaIDu(paket->idSobe);

        if (it==sobe.end())
        {
                me_log->Lines->Add(trenutnoVrijeme() + " | Zahtjev za zatvaranje sobe (" +
                        paket->idSobe + ") obraðen, status: N");
                return;
        }

        (*it)->obavijestiSveIgraceOZatvaranju();
        delete *it;
        sobe.erase(it);
        me_log->Lines->Add(trenutnoVrijeme() + " | Zahtjev za zatvaranje sobe (" +
                        paket->idSobe + ") obraðen, status: Y");
}

void TFormGlavna::obradiUdjiUSobu(PaketUdjiUSobu* paket, TCustomWinSocket* korisnik)
{
        me_log->Lines->Add(trenutnoVrijeme() + " | Zahtjev za ulazak u  sobu (" +
                paket->idSobe + "): " + paket->ime + " " + paket->prezime + " - " + paket->cipovi);

        PaketUdjiUSobuResponse pkt;
        vector<Soba*>::iterator it=nadjiSobuPremaIDu(paket->idSobe);
        if (it==sobe.end())
                pkt.dodijeljeniID=-1;
        else
                pkt.dodijeljeniID=(*it)->dodajIgraca(korisnik,paket->username, paket->ime,paket->prezime,paket->cipovi,me_log);

        korisnik->SendBuf(&pkt,sizeof(pkt));

        if(pkt.dodijeljeniID!=-1)
                (*it)->posaljiSvimaSpisakIgraca();

        me_log->Lines->Add(trenutnoVrijeme() + " | Zahtjev za ulazak u  sobu (" +
                paket->idSobe + "): " + paket->ime + " " + paket->prezime + " - " + paket->cipovi +
                "; obraðen, dodijeljeni ID: " + pkt.dodijeljeniID);
}

void TFormGlavna::obradiSviIgraciUSobi(PaketSviIgraciUSobi* paket, TCustomWinSocket* korisnik)
{
        me_log->Lines->Add(trenutnoVrijeme() + " | Zahtjev svi igraci u sobi: " +
                paket->idSobe);

        PaketSviIgraciUSobiResponse pkt;

        vector<Soba*>::iterator it=nadjiSobuPremaIDu(paket->idSobe);
        if (it==sobe.end())
                pkt.brojIgraca=-1;
        else
        {
                pkt.brojIgraca=(*it)->brojIgraca;
                for (int i=0; i<pkt.brojIgraca; i++)
                {
                        strcpy(pkt.igraci[i].korisnik.ime , (*it)->igraci[i]->ime);
                        strcpy(pkt.igraci[i].korisnik.prezime, (*it)->igraci[i]->prezime);
                        pkt.igraci[i].korisnik.cipovi = (*it)->igraci[i]->cipovi;
                        pkt.igraci[i].idUSobi = (*it)->igraci[i]->id;
                }
        }

        korisnik->SendBuf(&pkt,sizeof(pkt));
        me_log->Lines->Add(trenutnoVrijeme() + " | Zahtjev svi igraci u sobi (" +
                paket->idSobe + ") obraðen, broj igraèa: " + pkt.brojIgraca);
}

void TFormGlavna::obradiIzadjiIzSobe(PaketIzadjiIzSobe* paket, TCustomWinSocket* korisnik)
{
        me_log->Lines->Add(trenutnoVrijeme() + " | Zahtjev izlazak iz sobe: SobaID - " +
                paket->idSobe + "; IgracID - " + paket->idIgracaUSobi);

        AnsiString poruka="";

        vector<Soba*>::iterator it=nadjiSobuPremaIDu(paket->idSobe);
        (*it)->izbrisiIgracaPremaID(paket->idIgracaUSobi,me_log);
        (*it)->obavijestiSveIgraceOBrisanju(paket->idIgracaUSobi);
}

void TFormGlavna::obradiIzbacenIzSobe(PaketIzbacenIzSobe* paket, TCustomWinSocket* korisnik)
{
        me_log->Lines->Add(trenutnoVrijeme() + " | Zahtjev izbaci nasilno: SobaID - " +
                paket->idSobe + "; IgracID - " + paket->idIgracaUSobi);

        AnsiString poruka="";

        vector<Soba*>::iterator it=nadjiSobuPremaIDu(paket->idSobe);
        TCustomWinSocket* obrisaniKorisnik = (*it)->vratiSoketIgracaPremaID(paket->idIgracaUSobi);

        (*it)->izbrisiIgracaPremaID(paket->idIgracaUSobi,me_log);
        (*it)->obavijestiSveIgraceOBrisanju(paket->idIgracaUSobi);

        PaketIzbacenIzSobeNotifikacija pktNot;
        if (obrisaniKorisnik) obrisaniKorisnik->SendBuf(&pktNot,sizeof(pktNot));
}

void TFormGlavna::obradiZapocniIgruUSobi(PaketZapocniIgruUSobi* paket, TCustomWinSocket* korisnik)
{
        me_log->Lines->Add(trenutnoVrijeme() + " | Zahtjev za poèetak igre: " + paket->idSobe);

        vector<Soba*>::iterator it=nadjiSobuPremaIDu(paket->idSobe);

        if (it!=sobe.end())
        {
                (*it)->igraUToku=true;
                (*it)->obavijestiSveIgraceOPocetku();
        }

        me_log->Lines->Add(trenutnoVrijeme() + " | Zahtjev za poèetak igre (" + paket->idSobe + ") obraðen");
}
void TFormGlavna::obradiKarte(PaketKarte* paket, TCustomWinSocket* korisnik)
{
        me_log->Lines->Add(trenutnoVrijeme() + " | Stigle karte za sobu: " + paket->idSobe
                        + " (n: " + paket->brojIgraca + ")");

        vector<Soba*>::iterator it=nadjiSobuPremaIDu(paket->idSobe);
        for (int i=0; i<(*it)->brojIgraca; i++)
                (*it)->igraci[i]->igrac->SendBuf(paket,sizeof(*paket));

        me_log->Lines->Add(trenutnoVrijeme() + " | Poslane karte svima u sobi: " + paket->idSobe
                        + " (n: " + paket->brojIgraca + ") // SIZE: " + sizeof(*paket));
}
void TFormGlavna::obradiPotez(PaketPotez* paket, TCustomWinSocket* korisnik)
{
        me_log->Lines->Add(trenutnoVrijeme() + " | Stigle potez svima: ID Sobe - " + paket->idSobe
                        + " ; ID Igraca " + paket->idIgacaUSobi + " ; Potez - " +
                        (paket->tipPoteza == potezCall ? " CALL " : (
                                paket->tipPoteza==potezFold ? " FOLD " : " RAISE "  )) );

        vector<Soba*>::iterator it=nadjiSobuPremaIDu(paket->idSobe);
        for (int i=0; i<(*it)->brojIgraca; i++)
                (*it)->igraci[i]->igrac->SendBuf(paket,sizeof(*paket));
}
void __fastcall TFormGlavna::ss_OnClientError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
        me_log->Lines->Add(trenutnoVrijeme() + " | Greska, klijent: " + Socket->RemoteHost
                        + " | Kod greske: " + ErrorCode);
        ErrorCode=0;
}

//---------------------------------------------------------------------------

void __fastcall TFormGlavna::bt_sacuvajLogClick(TObject *Sender)
{
        fstream logFile("log.txt",ios::out);
        logFile << me_log->Text.c_str() << endl;
        logFile.close();        
}
//---------------------------------------------------------------------------

