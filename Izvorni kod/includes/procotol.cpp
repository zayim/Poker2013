//---------------------------------------------------------------------------


#pragma hdrstop

#include "procotol.h"

PaketRegistracija::PaketRegistracija(const Korisnik &k) : Paket(pktRegistracija)
{
        strcpy(this->ime,k.ime);
        strcpy(this->prezime,k.prezime);
        strcpy(this->username,k.username);
        strcpy(this->password,k.password);
}

PaketLogin::PaketLogin(const Korisnik &k) : Paket(pktLogin)
{
        strcpy(this->username,k.username);
        strcpy(this->password,k.password);
}
PaketKorisnikResponse::PaketKorisnikResponse(const char im[10], const char pr[20],
                        const char un[20], int cip) : Paket(pktKorisnikResponse), cipovi(cip)
{
        strcpy(ime,im); strcpy(prezime,pr); strcpy(username,un);
        status=true;
}


//---------------------------------------------------------------------------

#pragma package(smart_init)
