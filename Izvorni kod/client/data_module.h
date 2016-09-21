//---------------------------------------------------------------------------

#ifndef data_moduleH
#define data_moduleH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ScktComp.hpp>
#include "../includes/korisnik.h"
//---------------------------------------------------------------------------
class Tdm_dataModule : public TDataModule
{
__published:	// IDE-managed Components
private:	// User declarations
public:		// User declarations
        __fastcall Tdm_dataModule(TComponent* Owner);
        Korisnik korisnik;
};
//---------------------------------------------------------------------------
extern PACKAGE Tdm_dataModule *dm_dataModule;
//---------------------------------------------------------------------------
#endif
