//---------------------------------------------------------------------------


#pragma hdrstop

#include "logika.h"
#include <algorithm>

Igrac::Igrac(const Korisnik& k, int id)
{
        strcpy(this->ime , k.ime);
        strcpy(this->prezime, k.prezime);
        cipovi=k.cipovi;
        idUSobi=id;

        status=igra;
}
void Igrac::iscrtaj()
{
        canvasK1->Assign(karta1.slika);
        canvasK2->Assign(karta2.slika);
}
Pot::Pot(int n)
{
        brojIgracaNaPotu=0;
        brojIgraca=n;
        for (int i=0; i<20; i++)
                uloziIgraca[i]=0;
        for (int i=0; i<20; i++)
                indexiIgracaNaPotu[i]=-1;

        ulogPoIgracu=0;
}
void Pot::dodajIndeksIgraca(int ind)
{
        for (int i=0; i<brojIgracaNaPotu; i++)
                if (indexiIgracaNaPotu[i]==ind)
                        return;

        indexiIgracaNaPotu[brojIgracaNaPotu++]=ind;
}
bool Pot::ukloniIndeksIgraca(int ind)
{
        for (int i=0; i<brojIgracaNaPotu; i++)
                if (indexiIgracaNaPotu[i]==ind)
                {
                        for (int j=i; j<brojIgracaNaPotu-1; j++)
                                std::swap(indexiIgracaNaPotu[j],indexiIgracaNaPotu[j+1]);
                        brojIgracaNaPotu--;
                        return true;
                }
        return false;
}
void Pot::izbrisiIgracaPremaIndexu(int ind)
{
        for (int i=ind; i<brojIgraca-1; i++)
                std::swap(uloziIgraca[i],uloziIgraca[i+1]);
        brojIgraca--;

        for (int i=0; i<brojIgracaNaPotu; i++)
                if (indexiIgracaNaPotu[i]==ind)
                {
                        for (int j=i; j<brojIgracaNaPotu-1; j++)
                                std::swap(uloziIgraca[j],uloziIgraca[j+1]);
                        brojIgracaNaPotu--;
                        break;
                }
}
int Pot::vratiIznosPota()
{
        int iznos=0;
        for (int i=0; i<brojIgraca; i++)
                iznos+=uloziIgraca[i];

        return iznos;
}
AnsiString Karta::vratiImeFajla()
{
        AnsiString fileName="slike/karte/";
        if (broj<11) fileName+=AnsiString(broj)+"_";
        else if (broj==11) fileName+="J_";
        else if (broj==12) fileName+="Q_";
        else if (broj==13) fileName+="K_";
        switch(boja)
        {
                case list:  fileName+="list.jpg";  break;
                case mak:   fileName+="mak.jpg";   break;
                case srce:  fileName+="srce.jpg";  break;
                case kocka: fileName+="kocka.jpg"; break;
        }

        return fileName;
}
Karta::Karta(Boja b, int br) : boja(b), broj(br)
{
        slika = new TJPEGImage;
        slika->LoadFromFile(vratiImeFajla());
}

int SkupKarti::kartaToBitIndex(const __Karta &k)
{
        return int(k.boja) * 13 + k.broj - 1;
}
SkupKarti::__Karta SkupKarti::bitIndexToKarta(int bitInd)
{
        __Karta tmp;
        tmp.boja = Boja(bitInd/13);
        tmp.broj = 1+ (bitInd%13);

        return tmp;
}

SkupKarti::SkupKarti(const __Karta &k1, const __Karta &k2, const __Karta &k3, const __Karta &k4,
                  const __Karta &k5, const __Karta &k6, const __Karta &k7)
{
        karte[0]=k1;
        karte[1]=k2;
        karte[2]=k3;
        karte[3]=k4;
        karte[4]=k5;
        karte[5]=k6;
        karte[6]=k7;

        for (int i=0; i<52; i++)
                bits[i]=false;

        for (int i=0; i<7; i++)
                bits[ kartaToBitIndex(karte[i]) ] = true;

        konstruisiHand();
}
SkupKarti::SkupKarti(const Karta &k1, const Karta &k2, const Karta &k3, const Karta &k4,
                  const Karta &k5, const Karta &k6, const Karta &k7)
{
        karte[0]=__Karta(k1.boja,k1.broj);
        karte[1]=__Karta(k2.boja,k2.broj);
        karte[2]=__Karta(k3.boja,k3.broj);
        karte[3]=__Karta(k4.boja,k4.broj);
        karte[4]=__Karta(k5.boja,k5.broj);
        karte[5]=__Karta(k6.boja,k6.broj);
        karte[6]=__Karta(k7.boja,k7.broj);

        for (int i=0; i<52; i++)
                bits[i]=false;

        for (int i=0; i<7; i++)
                bits[ kartaToBitIndex(karte[i]) ] = true;

        konstruisiHand();
}
void SkupKarti::konstruisiHand()
{
        if (provjeriRoyalFlush()) return;
        else if (provjeriStraightFlush()) return;
        else if (provjeriPoker()) return;
        else if (provjeriFullHouse()) return;
        else if (provjeriFlush()) return;
        else if (provjeriStraight()) return;
        else if (provjeriTriling()) return;
        else provjeriPairs();
}
int prioritet(int broj)
{
        if (broj>1 && broj<14) return broj;
        if (broj==1) return 14;
        return 0;
}
bool SkupKarti::provjeriRoyalFlush()
{
        for (int i=0; i<4; i++)
        {
                int zbir=bits[ 13*i ];
                for (int j=13*i+9; j<13*i+12; j++)
                        zbir+=bits[ j ];

                if (zbir==5)
                {
                        for (int j=0; j<4; j++)
                        {
                                hand.karte[j]=bitIndexToKarta(13*i+9 + j);
                                bits[13*i+9+j]=false;
                        }
                        hand.karte[4]=bitIndexToKarta(13*i);
                        bits[13*i]=false;

                        hand.tip = thRoyalFlush;
                        return true;
                }
        }

        return false;
}
bool SkupKarti::provjeriStraightFlush()
{
        for (int i=0; i<4; i++)
        {
                for (int j=13*i+8; j>=13*i; j--)
                {
                        int zbir=0;
                        for (int k=0; k<5; k++)
                                zbir+=bits[j+k];
                        if (zbir==5)
                        {
                                for (int k=4; k>=0; k--)
                                {
                                        hand.karte[4-k] = bitIndexToKarta(j+k);
                                        bits[j+k]=false;
                                }
                                hand.tip = thStraightFlush;
                                return true;
                        }
                }
        }

        return false;
}
bool SkupKarti::provjeriPoker()
{
        for (int i=0; i<13; i++)
        {
                int zbir=0;
                for (int j=0; j<4; j++)
                        zbir+=bits[i+13*j];

                if (zbir==4)
                {
                        hand.tip=thPoker;
                        for (int j=0; j<4; j++)
                        {
                                hand.karte[j] = bitIndexToKarta(i+13*j);
                                bits[i+13*j]=false;
                        }

                        if (hand.karte[0].broj!=1)
                        {
                                for (int z=0; z<4; z++)
                                if (bits[13*z])
                                {
                                        hand.karte[4]=bitIndexToKarta(13*z);
                                        return true;
                                }
                        }

                        for (int j=12; j>=0; j--)
                        for (int k=0; k<4; k++)
                                if (bits[k*13 + j])
                                {
                                        hand.karte[4]=bitIndexToKarta(k*13+j);
                                        return true;
                                }

                }
        }

        return false;
}
bool SkupKarti::provjeriFullHouse()
{
        for (int i=12; i>=0; i--)
        {
                int zbir=0;
                for (int j=0; j<4; j++)
                        zbir+=bits[i+13*j];

                int broj3=(i%13);
                if (zbir==3)
                {
                        for (int j=12; j>=0; j--)
                        if (j!=broj3)
                        {
                                int zbir__=0;
                                for (int k=0; k<4; k++)
                                zbir__+=bits[k*13+j];

                                if (zbir__==2)
                                {
                                        for (int k=0; k<3; k++)
                                        {
                                                hand.karte[k]=bitIndexToKarta(k*13+broj3);
                                                bits[k*13+broj3]=false;
                                        }
                                                hand.karte[3]=bitIndexToKarta(j);
                                                bits[j]=false;
                                                hand.karte[4]=bitIndexToKarta(13+j);
                                                bits[13+j]=false;

                                        hand.tip=thFullHouse;
                                        return true;
                                }
                        }
                }
        }
        return false;
}
bool SkupKarti::provjeriFlush()
{
        for (int i=0; i<4; i++)
        {
                int zbir=0;
                for (int j=12; j>=0; j--)
                        zbir+=bits[i*13 + j];

                if (zbir>=5)
                {
                        int k=0,j=12;

                        while (k!=5 && j>=0)
                        {
                                if (bits[i*13 + j])
                                        hand.karte[k++]=bitIndexToKarta(i*13+j);
                                j--;
                        }
                        hand.tip=thFlush;

                        if (hand.karte[4].broj==1)
                        {
                                __Karta tmp=hand.karte[4];
                                hand.karte[4]=hand.karte[3];
                                hand.karte[3]=hand.karte[2];
                                hand.karte[2]=hand.karte[1];
                                hand.karte[1]=hand.karte[0];
                                hand.karte[0]=tmp;
                        }

                        // dodati kod sta ako imamo 6 ili 7 iste boje, a jedna od njih je 1

                        return true;
                }
        }
        return false;
}
bool SkupKarti::provjeriStraight()
{
        for (int i=8; i>=0; i--)
        {
                int zbir=0;
                for (int j=i; j<i+5; j++)
                {
                        int zbir__=0;
                        for (int k=0; k<4; k++)
                                zbir__+=bits[ k*13 + j ];

                        if (!zbir__) break;
                        else zbir++;
                }

                if (zbir==5)
                {
                        for (int j=i; j<i+5; j++)
                                hand.karte[j-i] = bitIndexToKarta(i+4-j);
                        hand.tip=thStraight;
                        return true;
                }
        }

        return false;
}
bool SkupKarti::provjeriTriling()
{
        for (int i=12; i>=0; i--)
        {
                int zbir=0;
                for (int j=0; j<4; j++)
                        zbir+=bits[ j*13 + i ];

                if (zbir==3)
                {
                        hand.tip=thTriling;
                        int k=0;
                        for (int j=0; j<4; j++)
                                if(bits[ j*13 + i ])
                                {
                                        hand.karte[k++]=bitIndexToKarta(j*13+i);
                                        bits[j*13+i]=false;
                                }

                        for (int j=12; j>=0; j--)
                                for (int l=0; l<4; l++)
                                        if (bits[l*13+j])
                                        {
                                                hand.karte[k++]=bitIndexToKarta(l*13+j);
                                                bits[l*13+j]=false;
                                                if (k==5)
                                                {
                                                        if (hand.karte[4].broj==1)
                                                        {
                                                                __Karta tmp=hand.karte[4];;
                                                                hand.karte[4]=hand.karte[3];
                                                                hand.karte[3]=tmp;
                                                        }
                                                        return true;
                                                }
                                        }
                }
        }

        return false;
}
void SkupKarti::provjeriPairs()
{
        for (int i=0; i<6; i++)
                for (int j=i+1; j<7; j++)
                        if (karte[i].broj < karte[j].broj)
                        {
                                __Karta tmp=karte[i];
                                karte[i]=karte[j];
                                karte[j]=tmp;
                        }

        for (int i=0; i<6; i++)
                if (karte[i].broj == karte[i+1].broj)
                {
                        hand.karte[0]=karte[i];
                        hand.karte[1]=karte[i+1];

                        for (int j=i+2; j<6; j++)
                                if (karte[j].broj == karte[j+1].broj)
                                {
                                        hand.tip=thTwoPairs;
                                        hand.karte[2]=karte[j];
                                        hand.karte[3]=karte[j+1];

                                        if (karte[6].broj==1)
                                        {
                                                hand.karte[4]=karte[6];
                                                return;
                                        }

                                        for (int k=0; k<7; k++)
                                                if (k!=i && k!=i+1 && k!=j && k!=j+1)
                                                {
                                                        hand.karte[4]=karte[k];
                                                        return;
                                                }
                                }
                        int br=2, k=0;
                        hand.tip=thOnePair;
                        if (karte[6].broj==1)
                        {
                                hand.karte[2]=karte[6];
                                br=3;
                        }

                        while (br!=5 && k<7)
                        {
                                if (k!=i && k!=i+1)
                                        hand.karte[br++]=karte[k];
                                k++;
                        }
                        return;
                }

        if (karte[6].broj==1)
        {
                hand.karte[0]=karte[6];
                for (int i=0; i<4; i++)
                        hand.karte[i+1]=karte[i];
        }
        else
        {
                for (int i=0; i<5; i++)
                        hand.karte[i]=karte[i];
        }
        hand.tip=thHighCard;
}
/*bool SkupKarti::provjeriOnePair()
{
        1+2;
}
bool SkupKarti::provjeriHighCard()
{
        1+2;
}*/
bool SkupKarti::__Hand::operator>(const __Hand& h)
{
        if ( int(tip) < int(h.tip) ) return false;
        if ( int(tip) > int(h.tip) ) return true;

        for (int i=0; i<5; i++)
                if (karte[i].broj != h.karte[i].broj)
                {
                        if (karte[i].broj==1 || karte[i].broj > h.karte[i].broj)
                                return true;
                        else return false;
                }
                
        return false;
}
bool SkupKarti::__Hand::operator==(const __Hand& h)
{
        if (tip!=h.tip) return false;

        for (int i=0; i<5; i++)
                if (karte[i].broj != h.karte[i].broj)
                        return false;

        return true;
}
AnsiString SkupKarti::vratiImeHanda()
{
        switch(hand.tip)
        {
                case thRoyalFlush: return AnsiString("Royal flush");
                case thStraightFlush: return AnsiString("Skala u boji");
                case thPoker: return AnsiString("Poker");
                case thFullHouse: return AnsiString("Full house");
                case thFlush: return AnsiString("Boja");
                case thStraight: return AnsiString("Skala");
                case thTriling: return AnsiString("Triling");
                case thTwoPairs: return AnsiString("Dva para");
                case thOnePair: return AnsiString("Jedan par");
                case thHighCard: return AnsiString("Jedna karta");
                default: return AnsiString("");
        }
}
//---------------------------------------------------------------------------

#pragma package(smart_init)
