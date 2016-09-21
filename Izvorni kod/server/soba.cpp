//---------------------------------------------------------------------------


#pragma hdrstop
#include "procotol.h"
#include "soba.h"

Soba::Soba(unsigned short id,const char ime[]) :
        id(id), brojIgraca(0), /*igraci(new IgracSocket*[MAX_BROJ_IGRACA]),*/ nextID(0), igraUToku(false)
{
        strcpy(this->ime,ime);
}

Soba::~Soba()
{
        1+2;//delete[] igraci; igraci=0;
}
AnsiString crtice()
{
        return AnsiString("-----");
}
int Soba::dodajIgraca(TCustomWinSocket *igrac, const char UN[20], const char IME[10], const char PR[20], int cip, TMemo* log)
{
        log->Lines->Add(crtice() + "Dodavanje (Soba:" + id + ", Igrac:" + IME + " " + PR +")");
        log->Lines->Add(crtice() + "Broj igraca u sobi: " + brojIgraca + " ; igraci.size()=" + igraci.size());
        if (brojIgraca==MAX_BROJ_IGRACA) return -1;
        igraci.push_back(new IgracSocket(nextID++,igrac,UN,IME,PR,cip));
        brojIgraca++;
        log->Lines->Add(crtice() + "Nakon dodavanja: Broj igraca u sobi: " + brojIgraca + " ; igraci.size()=" + igraci.size());
        //igraci[brojIgraca++]= new IgracSocket(nextID++,igrac,UN,IME,PR,cip) ;
        return nextID-1;
}

void Soba::posaljiSvimaSpisakIgraca()
{
        PaketSviIgraciUSobiResponse pkt;
        pkt.brojIgraca=brojIgraca;
        for (int i=0; i<pkt.brojIgraca; i++)
        {
                strcpy(pkt.igraci[i].korisnik.ime , igraci[i]->ime);
                strcpy(pkt.igraci[i].korisnik.prezime, igraci[i]->prezime);
                pkt.igraci[i].korisnik.cipovi = igraci[i]->cipovi;
                pkt.igraci[i].idUSobi = igraci[i]->id;
        }


        for (int i=0; i<brojIgraca; i++)
                igraci[i]->igrac->SendBuf(&pkt,sizeof(pkt));
}

void Soba::izbrisiIgracaPremaID(int idIgracaKojiIzlazi, TMemo* log)
{
        log->Lines->Add(crtice() + "Brisanje (Soba:" + id + ", Igrac:" + idIgracaKojiIzlazi+")");
        log->Lines->Add(crtice() + "Broj igraca u sobi: " + brojIgraca + " ; igraci.size()=" + igraci.size());
        for (std::vector<IgracSocket*>::iterator it=igraci.begin(); it!=igraci.end(); it++)
        {
                log->Lines->Add(crtice() + "ID: " + (*it)->id + " - Ime: " + (*it)->ime + (*it)->prezime);
                if ((*it)->id == idIgracaKojiIzlazi)
                {
                        log->Lines->Add(crtice() + "BRIŠEMO, ID: " + (*it)->id);
                        delete *it;
                        igraci.erase(it);
                        brojIgraca--;
                        log->Lines->Add(crtice() + "Nakon brisanja: "+ "Broj igraca u sobi: " +
                                brojIgraca + " ; igraci.size()=" + igraci.size() );
                        return;
                }
        }
}
void Soba::izbrisiIgracaPremaUsernameu(char usernameIgracaKojiIzlazi[20])
{
        for (std::vector<IgracSocket*>::iterator it=igraci.begin(); it!=igraci.end(); it++)
                if (!strcmp((*it)->ime , usernameIgracaKojiIzlazi))
                {
                        delete *it;
                        igraci.erase(it);
                        brojIgraca--;
                        return;
                }
}
void Soba::obavijestiSveIgraceOBrisanju(int idIgracaUSobi)
{
        PaketIzlazakIzSobeNotifikacija pkt(idIgracaUSobi);
        for (int i=0; i<brojIgraca; i++)
                igraci[i]->igrac->SendBuf(&pkt,sizeof(pkt));
}
TCustomWinSocket* Soba::vratiSoketIgracaPremaID(int idIgraca)
{
        for (int i=0; i<brojIgraca; i++)
                if (igraci[i]->id == idIgraca)
                        return igraci[i]->igrac;

        return NULL;
}
void Soba::obavijestiSveIgraceOZatvaranju()
{
        PaketIzbacenIzSobeNotifikacija pkt;
        for (int i=1; i<brojIgraca; i++)
                igraci[i]->igrac->SendBuf(&pkt,sizeof(pkt));
}
void Soba::obavijestiSveIgraceOPocetku()
{
        PaketPocetakIgreNotifikacija pkt;
        for (int i=0; i<brojIgraca; i++)
                igraci[i]->igrac->SendBuf(&pkt,sizeof(pkt));
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
