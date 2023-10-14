#include <iostream>
#include <fstream>
#include <wx/wx.h>
#include "wx/socket.h"
#include "mainwindow.h"
#include "fthelper.h"

class MyApp : public wxApp
{
public:
    bool OnInit() override;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    mainwindow  *mw = new mainwindow ();

    mw->Show();

    return true;
}



