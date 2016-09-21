//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("glavna.cpp", FormConnect);
USEFORM("data_module.cpp", dm_dataModule); /* TDataModule: File Type */
USEFORM("igra.cpp", FormIgra);
USEFORM("login.cpp", FormLogin);
USEFORM("registracija.cpp", FormRegistracija);
USEFORM("soba_igraci.cpp", FormSobaIgraci);
USEFORM("sobe.cpp", FormSobe);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->Title = "Poker 2013";
                 Application->CreateForm(__classid(TFormConnect), &FormConnect);
                 Application->CreateForm(__classid(Tdm_dataModule), &dm_dataModule);
                 Application->CreateForm(__classid(TFormIgra), &FormIgra);
                 Application->CreateForm(__classid(TFormLogin), &FormLogin);
                 Application->CreateForm(__classid(TFormRegistracija), &FormRegistracija);
                 Application->CreateForm(__classid(TFormSobaIgraci), &FormSobaIgraci);
                 Application->CreateForm(__classid(TFormSobe), &FormSobe);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        catch (...)
        {
                 try
                 {
                         throw Exception("");
                 }
                 catch (Exception &exception)
                 {
                         Application->ShowException(&exception);
                 }
        }
        return 0;
}
//---------------------------------------------------------------------------
