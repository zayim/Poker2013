object FormGlavna: TFormGlavna
  Left = 244
  Top = 78
  BorderStyle = bsSingle
  Caption = 'Poker 2013 - Server'
  ClientHeight = 551
  ClientWidth = 574
  Color = clBtnFace
  Font.Charset = EASTEUROPE_CHARSET
  Font.Color = clWindowText
  Font.Height = -15
  Font.Name = 'Segoe UI'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 20
  object l_logNaslov: TLabel
    Left = 240
    Top = 24
    Width = 78
    Height = 20
    Caption = 'Informacije:'
  end
  object me_log: TMemo
    Left = 48
    Top = 56
    Width = 481
    Height = 321
    Font.Charset = EASTEUROPE_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Segoe UI'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 0
  end
  object bt_pokreniServer: TButton
    Left = 208
    Top = 416
    Width = 145
    Height = 33
    Caption = 'Pokreni server'
    TabOrder = 1
    OnClick = bt_pokreniServerClick
  end
  object bt_sacuvajLog: TButton
    Left = 208
    Top = 472
    Width = 145
    Height = 33
    Caption = 'Sa'#269'uvaj log'
    TabOrder = 2
    OnClick = bt_sacuvajLogClick
  end
  object ss_serverSocket: TServerSocket
    Active = False
    Port = 4000
    ServerType = stNonBlocking
    OnClientConnect = ss_OnClientConnect
    OnClientDisconnect = ss_OnClientDisconnect
    OnClientRead = ss_OnClientRead
    OnClientError = ss_OnClientError
    Left = 8
    Top = 8
  end
end
