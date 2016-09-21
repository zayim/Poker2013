//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "data_module.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tdm_dataModule *dm_dataModule;
//---------------------------------------------------------------------------
__fastcall Tdm_dataModule::Tdm_dataModule(TComponent* Owner)
        : TDataModule(Owner)
{
}
//---------------------------------------------------------------------------
