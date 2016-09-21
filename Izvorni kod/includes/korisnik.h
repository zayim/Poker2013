//---------------------------------------------------------------------------

#ifndef korisnikH
#define korisnikH
#include <string>

class Korisnik
{
        public:
        char ime[10];
        char prezime[20];
        char username[20];
        char password[20];
        int cipovi;
        static const int POCETNI_BROJ_CIPOVA=100000;

        Korisnik() : cipovi(POCETNI_BROJ_CIPOVA){}
        Korisnik(const char ime[10], const char prezime[20],
              const char username[20], const char password[20]);
        //Korisnik(const Korisnik &k);
        //Korisnik& operator =(const Korisnik &);

        Korisnik(const Korisnik &k) :  cipovi(k.cipovi)
        {
                strcpy(this->ime,k.ime);
                strcpy(this->prezime,k.prezime);
                strcpy(this->username,k.username);
                strcpy(this->password,k.password);
        }

        Korisnik& operator =(const Korisnik &k)
        {
                if (this==&k) return *this;

                strcpy(this->ime,k.ime);
                strcpy(this->prezime,k.prezime);
                strcpy(this->username,k.username);
                strcpy(this->password,k.password);
                cipovi=k.cipovi;

                return *this;
        }
};
//---------------------------------------------------------------------------
#endif

