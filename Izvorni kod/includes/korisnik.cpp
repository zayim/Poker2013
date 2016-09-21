//---------------------------------------------------------------------------
#pragma hdrstop
#include <string.h>
#include "korisnik.h"

Korisnik::Korisnik(const char ime[10], const char prezime[20],
              const char username[20], const char password[20]) : cipovi(POCETNI_BROJ_CIPOVA)
{
        strcpy(this->ime,ime);
        strcpy(this->prezime,prezime);
        strcpy(this->username,username);
        strcpy(this->password,password);
}
/*Korisnik::Korisnik(const Korisnik &k) :  cipovi(k.cipovi)
{
        strcpy(this->ime,k.ime);
        strcpy(this->prezime,k.prezime);
        strcpy(this->username,k.username);
        strcpy(this->password,k.password);
}
Korisnik& Korisnik::operator =(const Korisnik &k)
{
        if (this==&k) return *this;

        strcpy(this->ime,k.ime);
        strcpy(this->prezime,k.prezime);
        strcpy(this->username,k.username);
        strcpy(this->password,k.password);
        cipovi=k.cipovi;

        return *this;
}*/

//---------------------------------------------------------------------------

#pragma package(smart_init)

