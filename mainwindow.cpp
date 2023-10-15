#include <iostream>
#include <fstream>
#include "mainwindow.h"
#include "fthelper.h"


//(*InternalHeaders(mainwindow)
#include <wx/font.h>
#include <wx/intl.h>
#include <wx/settings.h>
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
const long mainwindow::ID_STATICBOX1 = wxNewId();
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
const long mainwindow::ID_TEXTCTRL6 = wxNewId();
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
unsigned char       maxschema;
std::vector<Worked> worked;
wxTimer             *qsotimer;

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
	StaticBox1 = new wxStaticBox(this, ID_STATICBOX1, _("Options"), wxPoint(608,128), wxSize(352,312), 0, _T("ID_STATICBOX1"));
	Button2 = new wxButton(this, ID_BUTTON5, _("START"), wxPoint(770,8), wxSize(190,33), 0, wxDefaultValidator, _T("ID_BUTTON5"));
	Button2->SetDefault();
	startstop = new wxButton(this, ID_BUTTON1, _("START"), wxPoint(770,8), wxSize(190,33), 0, wxDefaultValidator, _T("ID_BUTTON1"));
	startstop->SetDefault();
	StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("Listen IP"), wxPoint(8,16), wxDefaultSize, 0, _T("ID_STATICTEXT3"));
	StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("Forward IP"), wxPoint(328,16), wxDefaultSize, 0, _T("ID_STATICTEXT2"));
	StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("Listen port"), wxPoint(200,16), wxDefaultSize, 0, _T("ID_STATICTEXT4"));
	StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("Forward port"), wxPoint(536,16), wxDefaultSize, 0, _T("ID_STATICTEXT1"));
	listenip = new wxTextCtrl(this, ID_TEXTCTRL2, _("127.0.0.1"), wxPoint(64,8), wxSize(130,32), 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
	listenport = new wxTextCtrl(this, ID_TEXTCTRL4, _("2237"), wxPoint(272,8), wxSize(50,32), 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
	sendip = new wxTextCtrl(this, ID_TEXTCTRL1, _("127.0.0.1"), wxPoint(400,8), wxSize(130,32), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
	sendport = new wxTextCtrl(this, ID_TEXTCTRL3, _("2277"), wxPoint(624,8), wxSize(50,32), 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
	fwen = new wxCheckBox(this, ID_CHECKBOX1, _("Enabled"), wxPoint(680,16), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX1"));
	fwen->SetValue(false);
	log = new wxTextCtrl(this, ID_TEXTCTRL5, wxEmptyString, wxPoint(8,488), wxSize(952,232), wxTE_MULTILINE|wxTE_READONLY|wxTE_DONTWRAP|wxBORDER_DOUBLE, wxDefaultValidator, _T("ID_TEXTCTRL5"));
	wxFont logFont(10,wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,_T("Nimbus Roman"),wxFONTENCODING_DEFAULT);
	log->SetFont(logFont);
	decodes = new wxTextCtrl(this, ID_TEXTCTRL6, wxEmptyString, wxPoint(8,48), wxSize(312,392), wxTE_MULTILINE|wxTE_READONLY|wxTE_DONTWRAP|wxBORDER_SIMPLE|wxBORDER_DOUBLE, wxDefaultValidator, _T("ID_TEXTCTRL6"));
	wxFont decodesFont = wxSystemSettings::GetFont(wxSYS_OEM_FIXED_FONT);
	if ( !decodesFont.Ok() ) decodesFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
	decodesFont.SetPointSize(10);
	decodesFont.SetStyle(wxFONTSTYLE_NORMAL);
	decodesFont.SetWeight(wxFONTWEIGHT_BOLD);
	decodesFont.SetUnderlined(false);
	decodesFont.SetFaceName(_T("Nimbus Roman"));
	decodes->SetFont(decodesFont);
	decodingact = new wxButton(this, ID_BUTTON2, _("Decoding"), wxPoint(768,48), wxSize(95,33), 0, wxDefaultValidator, _T("ID_BUTTON2"));
	decodingact->Disable();
	txon = new wxButton(this, ID_BUTTON3, _("TX On"), wxPoint(864,48), wxSize(95,33), 0, wxDefaultValidator, _T("ID_BUTTON3"));
	txon->Disable();
	txtimeout = new wxChoice(this, ID_CHOICE1, wxPoint(760,304), wxSize(56,32), 0, 0, 0, wxDefaultValidator, _T("ID_CHOICE1"));
	txtimeout->Append(_("1"));
	txtimeout->SetSelection( txtimeout->Append(_("2")) );
	txtimeout->Append(_("3"));
	txtimeout->Append(_("4"));
	txtimeout->Append(_("5"));
	StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("TX timeout in min."), wxPoint(624,312), wxDefaultSize, 0, _T("ID_STATICTEXT5"));
	Button1 = new wxButton(this, ID_BUTTON4, _("Clear decodes"), wxPoint(8,448), wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
	onlycq = new wxCheckBox(this, ID_CHECKBOX2, _("Show only CQ decodes"), wxPoint(152,456), wxDefaultSize, 0, wxDefaultValidator, _T("ID_CHECKBOX2"));
	onlycq->SetValue(true);
	wantedloc = new wxTextCtrl(this, ID_TEXTCTRL10, wxEmptyString, wxPoint(520,72), wxSize(80,368), wxTE_MULTILINE, wxDefaultValidator, _T("ID_TEXTCTRL10"));
	ignorelist = new wxTextCtrl(this, ID_TEXTCTRL8, wxEmptyString, wxPoint(432,72), wxSize(80,368), wxTE_MULTILINE, wxDefaultValidator, _T("ID_TEXTCTRL8"));
	wantedlist = new wxTextCtrl(this, ID_TEXTCTRL7, wxEmptyString, wxPoint(344,72), wxSize(80,368), wxTE_MULTILINE, wxDefaultValidator, _T("ID_TEXTCTRL7"));
	usewantedlist = new wxRadioButton(this, ID_RADIOBUTTON1, _("Use wanted list"), wxPoint(616,200), wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON1"));
	RadioButton2 = new wxRadioButton(this, ID_RADIOBUTTON2, _("Use ignore list"), wxPoint(616,224), wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON2"));
	usemindistance = new wxRadioButton(this, ID_RADIOBUTTON3, _("Min. distance km. :"), wxPoint(616,272), wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON3"));
	mindistance = new wxTextCtrl(this, ID_TEXTCTRL9, _("700"), wxPoint(768,264), wxSize(48,32), 0, wxDefaultValidator, _T("ID_TEXTCTRL9"));
	StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("Wanted pref."), wxPoint(344,48), wxDefaultSize, 0, _T("ID_STATICTEXT6"));
	StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("Ignored pref."), wxPoint(432,48), wxDefaultSize, 0, _T("ID_STATICTEXT7"));
	usecq = new wxRadioButton(this, ID_RADIOBUTTON4, _("All CQ\'s"), wxPoint(616,152), wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON4"));
	usecq->SetValue(true);
	usecq73 = new wxRadioButton(this, ID_RADIOBUTTON5, _("CQ && 73"), wxPoint(616,176), wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON5"));
	StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("Wanted LOC"), wxPoint(520,48), wxDefaultSize, 0, _T("ID_STATICTEXT8"));
	RadioButton1 = new wxRadioButton(this, ID_RADIOBUTTON6, _("Use wanted LOC"), wxPoint(616,248), wxDefaultSize, 0, wxDefaultValidator, _T("ID_RADIOBUTTON6"));
	pause = new wxToggleButton(this, ID_TOGGLEBUTTON1, _("Pause"), wxPoint(768,88), wxSize(192,33), 0, wxDefaultValidator, _T("ID_TOGGLEBUTTON1"));

	Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&mainwindow::OnButton1Click);
	Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&mainwindow::OnButton1Click);
	Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&mainwindow::OnButton1Click1);
	Connect(ID_RADIOBUTTON3,wxEVT_COMMAND_RADIOBUTTON_SELECTED,(wxObjectEventFunction)&mainwindow::OnRadioButton3Select);
	//*)
    SetLabel ("FT helper version 1.00");
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

#if wxUSE_IPV6
    typedef wxIPV6address IPaddress;
#else
    typedef wxIPV4address IPaddress;
#endif


void mainwindow::OnButton1Click(wxCommandEvent& event)
{
    if  (isrunning)
    {
        isrunning = false;
        listenip->Enable();
        listenport->Enable();
        sendip->Enable();
        sendport->Enable();
        fwen->Enable();
        startstop->SetLabel("START");
        log->AppendText("Stoping listener.\n");
        sock->Destroy();
        startstop->SetBackgroundColour("");
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
        decodes->AppendText("Waiting for decodes ...\n");
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
log->AppendText("asdasdas\n");


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


bool    qsobefore (Worked *w)
{
    //std::cout << "qsobefore : " << w->dxcall << " " << w->freq << " " << w->mode << std::endl;
    for (unsigned int i = 0; i < worked.size(); ++i) {
        if  (!strcmp (worked[i].dxcall, w->dxcall))  {
            //std::cout << "qsobefore: FOUND " << " call " << w->dxcall << " freq / freq " << w->freq << " / " << worked[i].freq << " mode " << w->mode << std::endl;
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
                    if  (decoderunning)
                    {
                        decodingact->SetBackgroundColour(*wxYELLOW);
                        decodes->AppendText("------------------------------------------------------------------------------\n");
                        //decodes->Clear();
                    }
                    else
                        decodingact->SetBackgroundColour("");
                    if  (txing)
                        txon->SetBackgroundColour(*wxRED);
                    else
                        txon->SetBackgroundColour("");
                    sprintf (str,"Status : %s [ID %s] Tuned frequency %.3f Mhz %s    Total registered QSO's : %lu\n", Software, soft, freq / 1e6, mode, worked.size());
                    SetStatusText(str);
                    break;
            case    2: // decode
                    char            newdecode;
                    char            msg[64];
                    uint32_t        decodetime,snr,deltafreq;
                    uint64_t        deltatime;
                    char            s1[16],s2[16],s3[16],s4[16];
                    unsigned int    c;
                    unsigned char   replymsg[1024];

                    memset (mode, 0, sizeof (mode));
                    memset (msg, 0, sizeof (msg));
                    memset (str, 0, sizeof (str));
                    ptr = buf + 12;
                    len = *(ptr + 3);ptr+=4;
                    memcpy (soft, ptr, len);ptr += len;
                    newdecode = *(ptr);ptr++;newdecode = newdecode;// for warning
                    decodetime = (uint32_t)ptr[3] << 24 | (uint32_t)ptr[2] << 16 | (uint32_t)ptr[1] << 8 | (uint32_t)ptr[0];decodetime = decodetime; // for warning
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
                        strcpy (w.dxcall, s2);
                        w.freq = Globalfreq;
                        strcpy (w.mode, Globalmode);
                        if  (qsobefore(&w) || c == 4)   // ignoring CQ DX etc...
                            decodes->SetBackgroundColour (*wxCYAN);
                        else
                        {
                            decodes->SetBackgroundColour (*wxGREEN);
                            if  (!qsorunning && strlen (w.dxcall) > 3 && !pause->GetValue()) // check for CQ DX etc...
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
                            sprintf (str,"%4i\t\t\t%-17s\n",(char)snr, msg);
                        }
                    }
                    decodes->AppendText(str);
                    decodes->SetBackgroundColour (*wxWHITE);
                    break;
            case    3: // got clear messages window
                    log->AppendText("Got \"Clear\" message\n");
                    decodes->Clear();
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
    decodes->Clear();
}


void mainwindow::OnRadioButton3Select(wxCommandEvent& event)
{
}

void mainwindow::OnpauseClick(wxCommandEvent& event)
{
}
