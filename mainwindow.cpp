#include <iostream>
#include <fstream>
#include "mainwindow.h"
#include "fthelper.h"


//(*InternalHeaders(mainwindow)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/string.h>
//*)
#include <wx/socket.h>
#include <wx/timer.h>
#include <wx/menu.h>
#include "fthelper.h"

#include <fstream>
#include <iterator>
#include <algorithm>

//(*IdInit(mainwindow)
const long mainwindow::ID_BUTTON5 = wxNewId();
const long mainwindow::ID_BUTTON1 = wxNewId();
const long mainwindow::ID_STATICTEXT3 = wxNewId();
const long mainwindow::ID_STATICTEXT2 = wxNewId();
const long mainwindow::ID_STATICTEXT4 = wxNewId();
const long mainwindow::ID_STATICTEXT1 = wxNewId();
const long mainwindow::ID_TEXTCTRL2 = wxNewId();
const long mainwindow::ID_TEXTCTRL4 = wxNewId();
const long mainwindow::ID_TEXTCTRL1 = wxNewId();
const long mainwindow::ID_TEXTCTRL3 = wxNewId();
const long mainwindow::ID_CHECKBOX1 = wxNewId();
const long mainwindow::ID_TEXTCTRL5 = wxNewId();
const long mainwindow::ID_BUTTON2 = wxNewId();
const long mainwindow::ID_BUTTON3 = wxNewId();
const long mainwindow::ID_CHOICE1 = wxNewId();
const long mainwindow::ID_STATICTEXT5 = wxNewId();
const long mainwindow::ID_BUTTON4 = wxNewId();
const long mainwindow::ID_CHECKBOX2 = wxNewId();
const long mainwindow::ID_TEXTCTRL10 = wxNewId();
const long mainwindow::ID_TEXTCTRL8 = wxNewId();
const long mainwindow::ID_TEXTCTRL7 = wxNewId();
const long mainwindow::ID_RADIOBUTTON1 = wxNewId();
const long mainwindow::ID_RADIOBUTTON2 = wxNewId();
const long mainwindow::ID_RADIOBUTTON3 = wxNewId();
const long mainwindow::ID_TEXTCTRL9 = wxNewId();
const long mainwindow::ID_STATICTEXT6 = wxNewId();
const long mainwindow::ID_STATICTEXT7 = wxNewId();
const long mainwindow::ID_RADIOBUTTON4 = wxNewId();
const long mainwindow::ID_RADIOBUTTON5 = wxNewId();
const long mainwindow::ID_STATICTEXT8 = wxNewId();
const long mainwindow::ID_RADIOBUTTON6 = wxNewId();
const long mainwindow::ID_TOGGLEBUTTON1 = wxNewId();
const long mainwindow::ID_LISTCTRL1 = wxNewId();
const long mainwindow::ID_BUTTON7 = wxNewId();
const long mainwindow::ID_BUTTON8 = wxNewId();
const long mainwindow::ID_BUTTON6 = wxNewId();
const long mainwindow::ID_BUTTON9 = wxNewId();
//*)
const long mainwindow::SOCKET_ID = wxNewId();
const long mainwindow::TIMER_ID = wxNewId();

bool                isrunning = false;
bool                decoderunning = false;
bool                qsorunning = false;
wxDatagramSocket    *sock;
char                Software[64] = {0};
char                Globalmode[8] = {0};
uint64_t            Globalfreq = 0;
char                GlobalCall[16] = {0};
char                GlobalQTH[16] = {0};
unsigned char       maxschema;
std::vector<Worked> worked;
wxTimer             *qsotimer;
unsigned long       decodedlines = 0;

BEGIN_EVENT_TABLE(mainwindow,wxFrame)
	//(*EventTable(mainwindow)
	//*)
    EVT_SOCKET(SOCKET_ID,  mainwindow::OnSocketEvent)
    EVT_TIMER(TIMER_ID, mainwindow::OnTimer)
END_EVENT_TABLE()

mainwindow::mainwindow()
{
	BuildContent();

    Worked  w;

    std::ifstream infile ("worked.dat", std::fstream::binary);
    while (infile.read (reinterpret_cast<char *>(&w), sizeof (w)))  {
        worked.push_back(w);
        //std::cout << "Imported QSO : " << w.dxcall << " Frequency : " << w.freq << " Mode " << w.mode << std::endl;
    }
    infile.close ();
    log->AppendText(wxString::Format(wxT("Loaded %lu QSO from worked.dat\n"), worked.size()));
    wantedlist->LoadFile("wantedprefs.txt");
    ignorelist->LoadFile("ignorelist.txt");
    wantedloc->LoadFile("wantedlocs.txt");
}
void mainwindow::BuildContent()
{ wxWindow  *parent = NULL;
	//(*Initialize(mainwindow)
	Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
	SetClientSize(wxSize(965,780));
	Button2 = new wxButton(this, ID_BUTTON5, _("START"), wxPoint(770,8), wxSize(190,33), 0, wxDefaultValidator, _T("ID_BUTTON5"));
	Button2->SetDefault();
	startstop = new wxButton(this, ID_BUTTON1, _("START"), wxPoint(770,8), wxSize(190,33), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	startstop->SetDefault();
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Listen IP"), wxPoint(8,8), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Forward IP"), wxPoint(328,8), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Listen port"), wxPoint(200,8), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Forward port"), wxPoint(536,8), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	listenip = new wxTextCtrl(this, ID_TEXTCTRL2, _("127.0.0.1"), wxPoint(64,8), wxSize(130,24), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	listenport = new wxTextCtrl(this, ID_TEXTCTRL4, _("2237"), wxPoint(272,8), wxSize(50,24), 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	sendip = new wxTextCtrl(this, ID_TEXTCTRL1, _("127.0.0.1"), wxPoint(400,8), wxSize(130,24), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	sendport = new wxTextCtrl(this, ID_TEXTCTRL3, _("2277"), wxPoint(624,8), wxSize(50,24), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	fwen = new wxCheckBox(this, ID_CHECKBOX1, _("Enabled"), wxPoint(680,8), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	fwen->SetValue(false);
	log = new wxTextCtrl(this, ID_TEXTCTRL5, wxEmptyString, wxPoint(8,488), wxSize(952,232), wxTE_MULTILINE|wxTE_READONLY|wxTE_DONTWRAP, wxDefaultValidator, _T("ID_TEXTCTRL5"));
	wxFont logFont(10,wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Nimbus Roman"),wxFONTENCODING_DEFAULT);
	log->SetFont(logFont);
	decodingact = new wxButton(this, ID_BUTTON2, _("Decoding"), wxPoint(768,48), wxSize(95,33), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	decodingact->Disable();
	txon = new wxButton(this, ID_BUTTON3, _("TX On"), wxPoint(864,48), wxSize(95,33), 0, wxDefaultValidator, _T("ID_BUTTON3"));
	txon->Disable();
	txtimeout = new wxChoice(this, ID_CHOICE1, wxPoint(760,224), wxSize(56,32), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
	txtimeout->Append(_("1"));
	txtimeout->SetSelection( txtimeout->Append(_("2")) );
	txtimeout->Append(_("3"));
	txtimeout->Append(_("4"));
	txtimeout->Append(_("5"));
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("TX timeout in min."), wxPoint(632,232), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	Button1 = new wxButton(this, ID_BUTTON4, _("Clear decodes"), wxPoint(8,448), wxSize(100,33), 0, wxDefaultValidator, _T("ID_BUTTON4"));
	onlycq = new wxCheckBox(this, ID_CHECKBOX2, _("Show only CQ decodes"), wxPoint(224,456), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
	onlycq->SetValue(true);
	wantedloc = new wxTextCtrl(this, ID_TEXTCTRL10, wxEmptyString, wxPoint(544,72), wxSize(60,328), wxTE_MULTILINE, wxDefaultValidator, _T("ID_TEXTCTRL10"));
	ignorelist = new wxTextCtrl(this, ID_TEXTCTRL8, wxEmptyString, wxPoint(472,72), wxSize(60,328), wxTE_MULTILINE, wxDefaultValidator, _T("ID_TEXTCTRL8"));
	wantedlist = new wxTextCtrl(this, ID_TEXTCTRL7, wxEmptyString, wxPoint(400,72), wxSize(60,328), wxTE_MULTILINE, wxDefaultValidator, _T("ID_TEXTCTRL7"));
	usewantedlist = new wxRadioButton(this, ID_RADIOBUTTON1, _("Use wanted list"), wxPoint(608,120), wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON1"));
	useignorelist = new wxRadioButton(this, ID_RADIOBUTTON2, _("Use ignore list"), wxPoint(608,144), wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON2"));
	usemindistance = new wxRadioButton(this, ID_RADIOBUTTON3, _("Min. distance km. :"), wxPoint(608,192), wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON3"));
	usemindistance->Disable();
	mindistance = new wxTextCtrl(this, ID_TEXTCTRL9, _("700"), wxPoint(760,192), wxSize(97,24), 0, wxDefaultValidator, _T("ID_TEXTCTRL9"));
	mindistance->Disable();
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Wanted DX"), wxPoint(400,48), wxSize(72,19), 0, _T("ID_STATICTEXT6"));
	wxFont StaticText6Font(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
	StaticText6->SetFont(StaticText6Font);
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Ignored DX"), wxPoint(472,48), wxSize(72,19), 0, _T("ID_STATICTEXT7"));
	wxFont StaticText7Font(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
	StaticText7->SetFont(StaticText7Font);
	usecq = new wxRadioButton(this, ID_RADIOBUTTON4, _("All CQ\'s"), wxPoint(608,72), wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON4"));
	usecq->SetValue(true);
	usecq73 = new wxRadioButton(this, ID_RADIOBUTTON5, _("CQ && 73"), wxPoint(608,96), wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON5"));
	usecq73->Disable();
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("Wanted LOC"), wxPoint(544,48), wxSize(72,19), 0, _T("ID_STATICTEXT8"));
	wxFont StaticText8Font(8,wxFONTFAMILY_SWISS,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL,false,_T("Sans"),wxFONTENCODING_DEFAULT);
	StaticText8->SetFont(StaticText8Font);
	usewantedloc = new wxRadioButton(this, ID_RADIOBUTTON6, _("Use wanted LOC"), wxPoint(608,168), wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON6"));
	pause = new wxToggleButton(this, ID_TOGGLEBUTTON1, _("Pause"), wxPoint(768,88), wxSize(192,33), 0, wxDefaultValidator, _T("ID_TOGGLEBUTTON1"));
	decodesList = new wxListCtrl(this, ID_LISTCTRL1, wxPoint(8,48), wxSize(384,390), wxLC_REPORT|wxLC_NO_HEADER, wxDefaultValidator, _T("ID_LISTCTRL1"));
	wxFont decodesListFont(10,wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Nimbus Roman"),wxFONTENCODING_DEFAULT);
	decodesList->SetFont(decodesListFont);
	Button4 = new wxButton(this, ID_BUTTON7, _("Load"), wxPoint(400,408), wxSize(64,33), 0, wxDefaultValidator, _T("ID_BUTTON7"));
	Button5 = new wxButton(this, ID_BUTTON8, _("Load"), wxPoint(544,408), wxSize(64,33), 0, wxDefaultValidator, _T("ID_BUTTON8"));
	Button3 = new wxButton(this, ID_BUTTON6, _("Load"), wxPoint(472,408), wxSize(64,33), 0, wxDefaultValidator, _T("ID_BUTTON6"));
	Button6 = new wxButton(this, ID_BUTTON9, _("Clear log "), wxPoint(112,448), wxSize(100,33), 0, wxDefaultValidator, _T("ID_BUTTON9"));
	FileDialog1 = new wxFileDialog(this, _("Select wanted DX file"), _("."), wxEmptyString, _("*.txt"), wxFD_DEFAULT_STYLE, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));
	FileDialog2 = new wxFileDialog(this, _("Select ADI file"), wxEmptyString, wxEmptyString, _("*.adi*"), wxFD_DEFAULT_STYLE, wxDefaultPosition, wxDefaultSize, _T("wxFileDialog"));

	Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&mainwindow::OnButton1Click);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&mainwindow::OnButton1Click);
	Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&mainwindow::OnButton1Click1);
	Connect(ID_RADIOBUTTON3,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&mainwindow::OnRadioButton3Select);
	Connect(ID_TOGGLEBUTTON1,wxEVT_COMMAND_TOGGLEBUTTON_CLICKED,(wxObjectEventFunction)&mainwindow::OnpauseToggle);
	Connect(ID_BUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&mainwindow::OnButton4Click);
	Connect(ID_BUTTON8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&mainwindow::OnButton5Click);
	Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&mainwindow::OnButton3Click);
	Connect(ID_BUTTON9,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&mainwindow::OnButton6Click);
	//*)

	decodesList->AppendColumn ("------", wxLIST_FORMAT_RIGHT);
	decodesList->AppendColumn ("dB", wxLIST_FORMAT_RIGHT);
	decodesList->AppendColumn ("", wxLIST_FORMAT_RIGHT);
	decodesList->AppendColumn ("", wxLIST_FORMAT_RIGHT);
	decodesList->AppendColumn ("", wxLIST_FORMAT_LEFT);
	decodesList->AppendColumn ("Loc", wxLIST_FORMAT_RIGHT);
	decodesList->SetColumnWidth(0, 55);
	decodesList->SetColumnWidth(1, 35);
    decodesList->SetColumnWidth(2, 35);
    decodesList->SetColumnWidth(3, 35);

    std::cout.rdbuf(log);
#ifdef __WXMSW__
    decodesList->SetColumnWidth(4, 145); // msg
#else
    decodesList->SetColumnWidth(4, 165); // msg
#endif
    decodesList->SetColumnWidth(5, 50); // Grid

    SetLabel ("FT helper v1.02");
    CreateStatusBar ();
    SetStatusText ("FT helper ready.");
    memset (Software, 0, sizeof (Software));
    qsotimer = new wxTimer();
    qsotimer->SetOwner(this, TIMER_ID);

  	pMenuBar = new wxMenuBar;
    pImportMenu = new wxMenu;
    pImportMenu->Append(wxID_OPEN, _T("&Import ADIF to worked.dat"));
    pImportMenu->Append(wxID_EXIT, _T("&Quit"));
    pAboutMenu = new wxMenu;
    pAboutMenu->Append(wxID_ABOUT, _T("&About"));
    pMenuBar->Append(pImportMenu, wxT("&File"));
    pMenuBar->Append(pAboutMenu, wxT("&Help"));
    SetMenuBar(pMenuBar);
    Bind(wxEVT_MENU, &mainwindow::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &mainwindow::OnImportADIF, this, wxID_OPEN);
    pMenuBar->Show();
}

mainwindow::~mainwindow()
{
	//(*Destroy(mainwindow)
	//*)
    std::cout << "Logged QSO's : " << worked.size();std::cout << std::endl;
    std::cout << "Saving ..." << std::endl;
    std::cout << "Saving wantedprefs.txt\n";
    wantedlist->SaveFile("wantedprefs.txt");
    ignorelist->SaveFile("ignorelist.txt");
    wantedloc->SaveFile("wantedlocs.txt");

    std::ofstream output ("worked.dat", std::fstream::trunc | std::fstream::binary);
    output.write (reinterpret_cast<const char *>(&worked[0]), worked.size() * sizeof (Worked));
    output.close ();
    delete qsotimer;
}

typedef wxIPV4address IPaddress;

void mainwindow::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("Inspired by lu9dce/ultron project i wrote my own\nAny ideas/bugs reports are welcome\nLY5VP, Egis. 73!\negislev@yandex.com",
                 "FTHelper", wxOK | wxICON_INFORMATION);
}

void mainwindow::OnButton1Click(wxCommandEvent& event)
{
    if  (isrunning)
    {
        isrunning = false;
        qsorunning = false;
        qsotimer->Stop ();
        listenip->Enable();
        listenport->Enable();
        sendip->Enable();
        sendport->Enable();
        fwen->Enable();
        startstop->SetLabel("START");
        startstop->Update ();
        sock->Destroy();
        startstop->SetBackgroundColour("");
        log->AppendText(wxString::Format(wxT("Listener stoped.\n")));
    } else
    {
        IPaddress addr;
        addr.Hostname(listenip->GetValue());
        auto str = addr.IPAddress();
        addr.Service(atoi(listenport->GetValue()));
        log->AppendText(wxString::Format(wxT("Starting listener service at %s:%u \n"),addr.IPAddress(), addr.Service()));
        sock = new wxDatagramSocket(addr);
        if  (!sock->IsOk())
        {
            log->AppendText(wxString::Format(wxT("Can not start listener service.\n")));
            return;
        }
        isrunning = true;
        listenip->Disable();
        listenport->Disable();
        sendip->Disable();
        sendport->Disable();
        fwen->Disable();
        sock->SetEventHandler(*this, SOCKET_ID);
        sock->SetNotify(wxSOCKET_INPUT | wxSOCKET_CONNECTION | wxSOCKET_LOST);
        sock->Notify(true);
        startstop->SetLabel("STOP");
        startstop->Update ();
    }
}

void mainwindow::OnSocketEvent(wxSocketEvent& event)
{
    switch (event.GetSocketEvent())
    {
        case wxSOCKET_INPUT:
            processPacket ();
            break;
        default:
            break;
    }
}

void mainwindow::OnMenu (wxCommandEvent& event)
{
log->AppendText("On menu test\n");

}


void mainwindow::OnTimer(wxTimerEvent& event)
{
    if  (qsorunning)
    {
        log->AppendText("Stop calling.\n");
        qsorunning = false;
    }
    qsotimer->Stop();
}

void mainwindow::OnImportADIF(wxCommandEvent& event)
{
    std::ifstream   in;
    char            str[1024];

    if  ( FileDialog2->ShowModal() == wxID_OK )
	{
		wxString path;
		path.append( FileDialog2->GetDirectory() );
#ifdef __WXMSW__
    path.append ("\\");
#else
    path.append ("//");
#endif
		path.append( FileDialog2->GetFilename() );
		std::cout  << path << "\n";
        in.open(path, std::ios_base::in);
        while ( !in.eof() )
        {
            in.getline (str, 1024);
            //std::cout << str << std::endl;
                    }
        in.close();
	}
}

int mainwindow::dxdistance (char *s)
{
    double mylon, mylat, myflon, myflat, myslon, myslat, mysublon, mysublat;
    double dxlon, dxlat, dxflon, dxflat, dxslon, dxslat, dxsublon, dxsublat;

    mysublat = mysublon = dxsublat = dxsublon = 0;

    if  (strlen (s) < 4 || strlen (GlobalQTH) < 4)
        return (0);

    myflon = toupper (GlobalQTH[0]) - 'A';myflat = toupper (GlobalQTH[1]) - 'A';
    myslon = GlobalQTH[2] - '0';myslat = GlobalQTH[3] - '0';
    if  (strlen (GlobalQTH) >= 4)
    {
        mysublon = toupper (GlobalQTH[4]) - 'A';mysublon += (float)0.5;mysublon /= (float)12;
        mysublat = toupper (GlobalQTH[5]) - 'A';mysublat += (float)0.5;mysublat /= (float)24;
    }
    mylon = myflon * 20 + myslon * 2 + mysublon;
    mylat = myflat * 10 + myslat + mysublat;
    mylon -= 180;mylat -= 90;

    dxflon = toupper (s[0]) - 'A';dxflat = toupper (s[1]) - 'A';
    dxslon = s[2] - '0';dxslat = s[3] - '0';
    if  (strlen (s) >= 4)
    {
        dxsublon = toupper (s[4]) - 'A';dxsublon += (float)0.5;dxsublon /= (float)12;
        dxsublat = toupper (s[5]) - 'A';dxsublat += (float)0.5;dxsublat /= (float)24;
    }
    dxlon = dxflon * 20 + dxslon * 2 + dxsublon;
    dxlat = dxflat * 10 + dxslat + dxsublat;
    dxlon -= 180;dxlat -= 90;

    std::cout << mylat << " " << mylon << "     " << dxlat << " " << dxlon << "\n";

    double r = 6371; // km
    double p = 3.14159265358 / 180;
    double a = 0.5 - std::cos ((dxlat - mylat) * p) / 2
                + std::cos(mylat * p) * std::cos(dxlat * p) *
                  (1 - std::cos((dxlon - mylon) * p)) / 2;

    double retval =  2 * r * std::asin (std::sqrt(a));
    std::cout << "Distance : " << retval << "\n";

    return (0);
}


bool    qsobefore (Worked *w)
{
    for (unsigned int i = 0; i < worked.size(); ++i) {
        if  (!strcmp ((char *)worked[i].dxcall,(char *) w->dxcall))  {
            return true;
        }
    }
    return false;
}

void    saverecord2file (Worked *w)
{
    std::ofstream output ("worked.dat", std::ios_base::app | std::fstream::binary);
    output.write (reinterpret_cast<const char *>(w), sizeof (Worked));
    output.close ();
}

bool    mainwindow::search4wanteddx (Worked *w)
{
    std::cout << "Searching for wanted DX [" << w->dxcall << "] ...\n";
    for (int i = 0;i < wantedlist->GetNumberOfLines();i++)
    {
        if  (strlen (wantedlist->GetLineText(i)))
        {
            if  (!strncasecmp (w->dxcall, wantedlist->GetLineText(i), strlen (wantedlist->GetLineText(i))))
            {
                std::cout << "Found wanted DX [" << w->dxcall << "] / [" << wantedlist->GetLineText(i) << "]\n";
                return true;
            }
        }
    }

    return false;
}

bool    mainwindow::search4ignoreddx (Worked *w)
{
    std::cout << "Searching for ignored DX [" << w->dxcall << "] ...\n";
    for (int i = 0;i < ignorelist->GetNumberOfLines();i++)
    {
        if  (strlen (ignorelist->GetLineText(i)))
        {
            if  (!strncasecmp (w->dxcall, ignorelist->GetLineText(i), strlen (ignorelist->GetLineText(i))))
            {
                std::cout << "Found ignored DX [" << w->dxcall << "] / [" << ignorelist->GetLineText(i) << "]\n";
                return true;
            }
        }
    }

    return false;
}

bool    mainwindow::search4wantedloc (unsigned char *l)
{
    std::cout << "Searching for wanted LOC [" << l << "] ...\n";
    for (int i = 0;i < wantedloc->GetNumberOfLines();i++)
    {
        if  (strlen (wantedloc->GetLineText(i)))
        {
            if  (!strncasecmp ((char *)l, wantedloc->GetLineText(i), strlen (wantedloc->GetLineText(i))))
            {
                std::cout << "Found wanted DX [" << l << "] / [" << wantedloc->GetLineText(i) << "]\n";
                return true;
            }
        }
    }

    return false;
}

void    mainwindow::addlinetolist (int32_t snr,int vals, bool worked,uint32_t tt, unsigned char *s1, unsigned char *s2, unsigned char *s3, unsigned char *s4)
{
    char    str[1024];

    tt /= 1000;
    sprintf (str,"%02u%02u%02u", tt / 60 / 60, (tt / 60) % 60, tt % 60);
    if  (s1[0] == 'C' && s1[1] == 'Q')   {
        decodesList->InsertItem (decodedlines, 1);
        decodesList->SetItem (decodedlines, 0, str);
        decodesList->SetItem (decodedlines, 1, wxString::Format(wxT("%i"), (char)snr));
        decodesList->SetItem (decodedlines, 2, s1);
        if  (vals == 4) // CQ DX AA1AAA AA11
        {
            decodesList->SetItem (decodedlines, 3, s2);
            decodesList->SetItem (decodedlines, 4, s3);
            decodesList->SetItem (decodedlines, 5, s4);
        } else // CQ AA1AAA AA11
        {
            decodesList->SetItem (decodedlines, 4, s2);
            decodesList->SetItem (decodedlines, 5, s3);
        }
        decodesList->EnsureVisible (decodedlines);
        if  (worked)
            decodesList->SetItemBackgroundColour(decodedlines, *wxCYAN);
        else
            if  (pause->GetValue())
                decodesList->SetItemBackgroundColour(decodedlines, *wxYELLOW);
            else
                decodesList->SetItemBackgroundColour(decodedlines, *wxGREEN);
        decodedlines++;
    } else // no CQ
    {
        decodesList->InsertItem (decodedlines, 1);
        decodesList->SetItem (decodedlines, 0, str);
        decodesList->SetItem (decodedlines, 1, wxString::Format(wxT("%i"), (char)snr));
        sprintf (str,"%s %s %s",s1,s2,s3);
        decodesList->SetItem (decodedlines, 4, str);
        decodesList->EnsureVisible (decodedlines);
        decodedlines++;
        }
}

void mainwindow::processPacket (void)
{
    unsigned char   buf[1024];
    char            str[1024];
    unsigned char   *ptr,len;
    uint32_t        magic;
    unsigned char   schema, mType;
    Worked          w;

    auto dataread = sock->Read(buf,1024).LastReadCount();
    if (dataread > 16)
    {
        magic = *(uint32_t *)buf;
        schema = buf[7];schema = schema; // no warning
        mType = buf[11];
        if  (magic != 0xdacbbcad) // !!! bytes order BE/LE !!!
        {
            log->AppendText("Bad packet magic !\n");
            return;
        }
        switch  (mType)
        {
            case    0: // heartbeat / software version
                    char    soft[32],version[32];

                    memset (soft, 0, sizeof (soft));memset (version, 0, sizeof (version));
                    ptr = buf + 12;
                    len = *(ptr + 3);ptr += 4;
                    memcpy (soft, ptr, len);
                    ptr += 4;
                    maxschema = *(ptr + 3);ptr += 4;
                    len = *(ptr + 3); ptr += 4;
                    memcpy (version, ptr, len);
                    sprintf (Software,"%s %s",soft,version);
                    startstop->SetBackgroundColour(*wxGREEN);
                    break;
            case    1: // status
                    uint64_t    freq;
                    char        mode[32],dxcall[32],report[32],txmode[32];
                    char        txenable, txing;

                    memset (soft, 0, sizeof (soft));
                    memset (mode, 0, sizeof (mode));
                    memset (dxcall, 0, sizeof (dxcall));
                    memset (report, 0, sizeof (report));
                    memset (txmode, 0, sizeof (txmode));
                    memset (GlobalCall, 0, sizeof (GlobalCall));
                    memset (GlobalQTH, 0, sizeof (GlobalQTH));
                    ptr = buf + 12;
                    len = *(ptr +3);ptr += 4;
                    memcpy (soft, ptr, len);
                    ptr += len;
                    memcpy (str,ptr,32);
                    freq = (uint64_t)ptr[0] << 54 | (uint64_t)ptr[1] << 48 | (uint64_t)ptr[2] << 40 | (uint64_t)ptr[3] << 32 | (uint64_t)ptr[4] << 24 | (uint64_t)ptr[5] << 16 | (uint64_t)ptr[6] << 8 | (uint64_t)ptr[7];
                    Globalfreq = freq;
                    ptr += 8;
                    len = *(ptr + 3);ptr += 4;
                    memcpy (mode, ptr, len);
                    ptr += len;
                    memcpy (str,ptr,64);
                    len = *(ptr + 3);ptr += 4;
                    memcpy (dxcall, ptr, len);
                    ptr += len;
                    len = *(ptr + 3);ptr += 4;
                    memcpy (report, ptr, len);
                    ptr += len;
                    len = *(ptr + 3);ptr += 4;
                    memcpy (mode, ptr, len); strcpy (Globalmode, mode);
                    ptr += len;
                    txenable = *ptr;ptr++;txenable = txenable; // for warning
                    txing = *ptr;ptr++;
                    decoderunning = *ptr != 0?true:false;
                    ptr += 9;
                    len = *(ptr + 3);ptr += 4;
                    memcpy (GlobalCall, ptr, len);
                    ptr += len;
                    len = *(ptr + 3);ptr += 4;
                    memcpy (GlobalQTH, ptr, len);
                    if  (decoderunning)
                    {
                        decodingact->SetBackgroundColour(*wxYELLOW);
                    }
                    else
                        decodingact->SetBackgroundColour("");
                    if  (txing)
                        txon->SetBackgroundColour(*wxRED);
                    else
                        txon->SetBackgroundColour("");
                    sprintf (str,"Status : %s [ID %s] Tuned frequency %.3f Mhz %s My call : %s My QTH : %s             Total registered QSO's : %lu\n", Software, soft, freq / 1e6, mode, GlobalCall, GlobalQTH, (unsigned long)worked.size());
                    SetStatusText(str);
                    break;
            case    2: // decode
                    char            newdecode;
                    char            msg[64];
                    uint32_t        decodetime,snr,deltafreq;
                    uint64_t        deltatime;
                    unsigned char   s1[16],s2[16],s3[16],s4[16];
                    unsigned int    c;
                    unsigned char   replymsg[1024];

                    memset (mode, 0, sizeof (mode));
                    memset (msg, 0, sizeof (msg));
                    memset (str, 0, sizeof (str));
                    ptr = buf + 12;
                    len = *(ptr + 3);ptr+=4;
                    memcpy (soft, ptr, len);ptr += len;
                    newdecode = *(ptr);ptr++;newdecode = newdecode;// for warning
                    decodetime = (uint32_t)ptr[0] << 24 | (uint32_t)ptr[1] << 16 | (uint32_t)ptr[2] << 8 | (uint32_t)ptr[3];decodetime = decodetime; // for warning
                    memset (str,0,sizeof (str));
                    ptr += 4;
                    snr = (uint32_t)ptr[3];snr = snr; // for warning
                    ptr += 4;
                    deltatime = (uint64_t)ptr[0] << 54 | (uint64_t)ptr[1] << 48 | (uint64_t)ptr[2] << 40 | (uint64_t)ptr[3] << 32 | (uint64_t)ptr[4] << 24 | (uint64_t)ptr[5] << 16 | (uint64_t)ptr[6] << 8 | (uint64_t)ptr[7];
                    deltatime = deltatime; // for warning
                    ptr += 8;
                    deltafreq =  0;deltafreq = deltafreq; // skipped for now, for warning
                    ptr += 4;
                    //memcpy (str,ptr,64);
                    len = *(ptr + 3);ptr += 4;
                    memcpy (mode, ptr, len);
                    ptr += len;
                    len = *(ptr + 3);ptr += 4;
                    memcpy (msg, ptr, len);
                    ptr += len;
                    // next ignored
                    c = sscanf (msg,"%s %s %s %s", s1, s2, s3, s4);
                    if  (msg[0] == 'C' && msg[1] == 'Q')
                    {
                        strcpy ((char *)w.dxcall, (char *)s2);
                        w.freq = Globalfreq;
                        strcpy ((char *)w.mode, Globalmode);
                        if  (qsobefore(&w) || c == 4)   // ignoring CQ DX, QSO before etc...
                            addlinetolist (snr, c, true, decodetime, s1, s2, s3, s4);
                        else
                        {

                            int distance = dxdistance ((char *)s3);
                            addlinetolist (snr, c, false, decodetime, s1, s2, s3, s4);
                            if  (!qsorunning && strlen ((char *)w.dxcall) > 3 && !pause->GetValue() && ((usewantedlist->GetValue() && search4wanteddx (&w)) || usecq->GetValue() ||
                                                                                                        (useignorelist->GetValue() && !search4ignoreddx(&w)) ||
                                                                                                        (usewantedloc->GetValue() && search4wantedloc(s3)))) // check for CQ DX etc...
                            {
                                qsorunning = true;
                                log->AppendText(wxString::Format(wxT("Calling [ %s ] ...\n"),w.dxcall));
                                qsotimer->StartOnce ((txtimeout->GetSelection() + 1) * 60000);
                                memset (replymsg, 0, sizeof (replymsg));
// in future make packet from 0 !
                                memcpy (replymsg, buf, 12);
                                replymsg[11] = 4; // Reply message
                                memcpy (replymsg + 12, buf + 12, 8);
                                memcpy (replymsg + 20, buf + 21, dataread - 22); // skip New Qtime .....
                                sock->Write(replymsg, dataread - 3); // - off air/low confidence/new
                            }


                        }
                        switch  (c) {
                                case 2: // CQ AA1AAA possible
                                    break;
                                case 3: // CQ AA1AAA AA11
                                    sprintf (str,"%4i\t%s\t\t%-25s\t\t%-40s\n",(char)snr, s1,s2,s3);
                                    break;
                                case 4: // CQ AA AA1AA AA11
                                    sprintf (str, "%4i\t%s\t%s\t%-25s\t\t%-40s\n",(char)snr, s1,s2,s3,s4) ;
                                    break;
                                default:
                                    sprintf (str, "%4i\t %s\n",(int)snr, msg) ;
                                    log->AppendText(wxString::Format(wxT("Bad CQ message [%s]/[%s]\n"), msg ,str));
                                    break;
                        }
                    } else {
                        if  (!onlycq->IsChecked())
                        {
                            addlinetolist (snr, c, true, decodetime, s1, s2, s3, s4);
                        }
                    }
                    //decodes->AppendText(str);
                    //decodes->SetBackgroundColour (*wxWHITE);
                    break;
            case    3: // got clear messages window
                    log->AppendText("Got \"Clear\" message\n");
                    decodesList->ClearAll ();
                    decodedlines = 0;
	decodesList->AppendColumn ("------", wxLIST_FORMAT_RIGHT);
	decodesList->AppendColumn ("dB", wxLIST_FORMAT_RIGHT);
	decodesList->AppendColumn ("", wxLIST_FORMAT_RIGHT);
	decodesList->AppendColumn ("", wxLIST_FORMAT_RIGHT);
	decodesList->AppendColumn ("", wxLIST_FORMAT_LEFT);
	decodesList->AppendColumn ("Loc", wxLIST_FORMAT_RIGHT);
	decodesList->SetColumnWidth(0, 55);
	decodesList->SetColumnWidth(1, 35);
    decodesList->SetColumnWidth(2, 35);
    decodesList->SetColumnWidth(3, 35);
#ifdef __WXMSW__
    decodesList->SetColumnWidth(4, 145); // msg
#else
    decodesList->SetColumnWidth(4, 165); // msg
#endif
    decodesList->SetColumnWidth(5, 50); // Grid


                    break;
            case    5: // logged QSO
                    Worked  w;

                    memset (&w, 0, sizeof (w));
                    memset (soft, 0, sizeof (soft));
                    ptr = buf + 12;
                    len = *(ptr + 3);ptr+=4;
                    memcpy (soft, ptr, len);ptr += len;
                    ptr += 13; // skip datetime decode
                    len = *(ptr + 3);ptr += 4;
                    memcpy (w.dxcall, ptr, len);
                    ptr += len;
                    len = *(ptr + 3);ptr += 4;
                    memcpy (msg, ptr, len); // grid
                    ptr += len;
                    w.freq = (uint64_t)ptr[0] << 54 | (uint64_t)ptr[1] << 48 | (uint64_t)ptr[2] << 40 | (uint64_t)ptr[3] << 32 | (uint64_t)ptr[4] << 24 | (uint64_t)ptr[5] << 16 | (uint64_t)ptr[6] << 8 | (uint64_t)ptr[7];
                    ptr += 8;
                    len = *(ptr + 3);ptr += 4;
                    memcpy (w.mode, ptr, len); // mode
                    ptr += len;
                    worked.push_back (w); // no check for dupes ?
                    saverecord2file (&w);
                    sprintf (str,"Logged QSO : Call %s Frequency %.3f Mode %s\n", w.dxcall, w.freq / 1e6, w.mode);
                    log->AppendText(str);
                    qsotimer->Stop();
                    qsorunning = false;
                    break;
            case    6: // close
                    break;
            case    10: // WSPR decode
                    break;
            case    12: // ADIF
                    break;
            default:
//                log->AppendText ("Unknown message type\n");
                break;
        }
    } else
    {
        log->AppendText("Got too short packet\n");
    }


}

void mainwindow::OnButton1Click1(wxCommandEvent& event)
{
                       decodesList->ClearAll ();
                    decodedlines = 0;
	decodesList->AppendColumn ("------", wxLIST_FORMAT_RIGHT);
	decodesList->AppendColumn ("dB", wxLIST_FORMAT_RIGHT);
	decodesList->AppendColumn ("", wxLIST_FORMAT_RIGHT);
	decodesList->AppendColumn ("", wxLIST_FORMAT_RIGHT);
	decodesList->AppendColumn ("", wxLIST_FORMAT_LEFT);
	decodesList->AppendColumn ("Loc", wxLIST_FORMAT_RIGHT);
	decodesList->SetColumnWidth(0, 55);
	decodesList->SetColumnWidth(1, 35);
    decodesList->SetColumnWidth(2, 35);
    decodesList->SetColumnWidth(3, 35);
#ifdef __WXMSW__
    decodesList->SetColumnWidth(4, 145); // msg
#else
    decodesList->SetColumnWidth(4, 165); // msg
#endif
    decodesList->SetColumnWidth(5, 50); // Grid

}


void mainwindow::OnRadioButton3Select(wxCommandEvent& event)
{
}

void mainwindow::OnpauseClick(wxCommandEvent& event)
{
}

void mainwindow::OnButton4Click(wxCommandEvent& event)
{
    if  ( FileDialog1->ShowModal() == wxID_OK )
	{
		wxString path;
		path.append( FileDialog1->GetDirectory() );
#ifdef __WXMSW__
    path.append ("\\");
#else
    path.append ("//");
#endif
		path.append( FileDialog1->GetFilename() );
        wantedlist->LoadFile(path);
		log->AppendText(path);
	}
}

void mainwindow::OnButton3Click(wxCommandEvent& event)
{
    if  ( FileDialog1->ShowModal() == wxID_OK )
	{
		wxString path;
		path.append( FileDialog1->GetDirectory() );
#ifdef __WXMSW__
    path.append ("\\");
#else
    path.append ("//");
#endif
		path.append( FileDialog1->GetFilename() );
        ignorelist->LoadFile(path);
		log->AppendText(path);
	}
}

void mainwindow::OnButton5Click(wxCommandEvent& event)
{
    if  ( FileDialog1->ShowModal() == wxID_OK )
	{
		wxString path;
		path.append( FileDialog1->GetDirectory() );
#ifdef __WXMSW__
    path.append ("\\");
#else
    path.append ("//");
#endif
		path.append( FileDialog1->GetFilename() );
        wantedloc->LoadFile(path);
		log->AppendText(path);
	}
}

void mainwindow::OnpauseToggle(wxCommandEvent& event)
{
    qsorunning = false;
    qsotimer->Stop();
}

void mainwindow::OnButton6Click(wxCommandEvent& event)
{
    log->Clear();
}

