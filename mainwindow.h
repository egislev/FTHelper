#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//(*Headers(mainwindow)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/filedlg.h>
#include <wx/frame.h>
#include <wx/listctrl.h>
#include <wx/radiobut.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/tglbtn.h>
//*)
#include <string.h>
#include <wx/socket.h>
#include <wx/timer.h>
#include <wx/msgdlg.h>
#include "fthelper.h"

class mainwindow: public wxFrame
{
	public:

		mainwindow();
		virtual ~mainwindow();
        void OnTimer(wxTimerEvent& event);
        void OnMenu(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnImportADIF(wxCommandEvent& event);

        wxMenuBar *pMenuBar;
        wxMenu *pImportMenu;
        wxMenu *pAboutMenu;


		//(*Declarations(mainwindow)
		wxButton* Button1;
		wxButton* Button2;
		wxButton* Button3;
		wxButton* Button4;
		wxButton* Button5;
		wxButton* Button6;
		wxButton* decodingact;
		wxButton* startstop;
		wxButton* txon;
		wxCheckBox* fwen;
		wxCheckBox* onlycq;
		wxChoice* txtimeout;
		wxFileDialog* FileDialog1;
		wxFileDialog* FileDialog2;
		wxListCtrl* decodesList;
		wxRadioButton* usecq;
		wxRadioButton* useignorelist;
		wxRadioButton* usemindistance;
		wxRadioButton* usewantedlist;
		wxRadioButton* usewantedloc;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxStaticText* StaticText7;
		wxStaticText* StaticText8;
		wxTextCtrl* ignorelist;
		wxTextCtrl* listenip;
		wxTextCtrl* listenport;
		wxTextCtrl* log;
		wxTextCtrl* mindistance;
		wxTextCtrl* sendip;
		wxTextCtrl* sendport;
		wxTextCtrl* wantedlist;
		wxTextCtrl* wantedloc;
		wxToggleButton* pause;
		//*)

	protected:

		//(*Identifiers(mainwindow)
		static const long ID_BUTTON5;
		static const long ID_BUTTON1;
		static const long ID_STATICTEXT3;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT4;
		static const long ID_STATICTEXT1;
		static const long ID_TEXTCTRL2;
		static const long ID_TEXTCTRL4;
		static const long ID_TEXTCTRL1;
		static const long ID_TEXTCTRL3;
		static const long ID_CHECKBOX1;
		static const long ID_TEXTCTRL5;
		static const long ID_BUTTON2;
		static const long ID_BUTTON3;
		static const long ID_CHOICE1;
		static const long ID_STATICTEXT5;
		static const long ID_BUTTON4;
		static const long ID_CHECKBOX2;
		static const long ID_TEXTCTRL10;
		static const long ID_TEXTCTRL8;
		static const long ID_TEXTCTRL7;
		static const long ID_RADIOBUTTON1;
		static const long ID_RADIOBUTTON2;
		static const long ID_RADIOBUTTON3;
		static const long ID_TEXTCTRL9;
		static const long ID_STATICTEXT6;
		static const long ID_STATICTEXT7;
		static const long ID_RADIOBUTTON4;
		static const long ID_STATICTEXT8;
		static const long ID_RADIOBUTTON6;
		static const long ID_TOGGLEBUTTON1;
		static const long ID_LISTCTRL1;
		static const long ID_BUTTON7;
		static const long ID_BUTTON8;
		static const long ID_BUTTON6;
		static const long ID_BUTTON9;
		//*)
		static const long SOCKET_ID;
		static const long TIMER_ID;

	private:

		//(*Handlers(mainwindow)
		void OnButton1Click(wxCommandEvent& event);
		void OnlogText(wxCommandEvent& event);
		void OnButton1Click1(wxCommandEvent& event);
		void OnRadioButton3Select(wxCommandEvent& event);
		void OnpauseClick(wxCommandEvent& event);
		void OnButton4Click(wxCommandEvent& event);
		void OnButton3Click(wxCommandEvent& event);
		void OnwantedlocText(wxCommandEvent& event);
		void OnButton5Click(wxCommandEvent& event);
		void OnpauseToggle(wxCommandEvent& event);
		void OnwantedlistText(wxCommandEvent& event);
		void OnButton6Click(wxCommandEvent& event);
		//*)
        void OnSocketEvent(wxSocketEvent& event);
        void addlinetolist (int32_t, int, bool, uint32_t, unsigned char *, unsigned char *, unsigned char *, unsigned char *);
        bool search4wanteddx (Worked *);
        bool search4ignoreddx (Worked *);
        bool search4wantedloc (unsigned char *);
        int dxdistance (char *);
	protected:

		void BuildContent();
		void processPacket();

		DECLARE_EVENT_TABLE()
};

#endif
