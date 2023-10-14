#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//(*Headers(mainwindow)
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/frame.h>
#include <wx/radiobut.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
//*)
#include <wx/socket.h>
#include <wx/timer.h>
#include "fthelper.h"

class mainwindow: public wxFrame
{
	public:

		mainwindow();
		virtual ~mainwindow();
        void OnTimer(wxTimerEvent& event);
        void OnMenu(wxCommandEvent& event);

        wxMenuBar *pMenuBar;
        wxMenu *pImportMenu;
        wxMenu *pAboutMenu;


		//(*Declarations(mainwindow)
		wxButton* Button1;
		wxButton* Button2;
		wxButton* decodingact;
		wxButton* startstop;
		wxButton* txon;
		wxCheckBox* fwen;
		wxCheckBox* onlycq;
		wxChoice* txtimeout;
		wxRadioButton* RadioButton2;
		wxRadioButton* usecq73;
		wxRadioButton* usecq;
		wxRadioButton* usemindistance;
		wxRadioButton* usewantedlist;
		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxStaticText* StaticText7;
		wxTextCtrl* decodes;
		wxTextCtrl* ignorelist;
		wxTextCtrl* listenip;
		wxTextCtrl* listenport;
		wxTextCtrl* log;
		wxTextCtrl* mindistance;
		wxTextCtrl* sendip;
		wxTextCtrl* sendport;
		wxTextCtrl* wantedlist;
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
		static const long ID_TEXTCTRL6;
		static const long ID_BUTTON2;
		static const long ID_BUTTON3;
		static const long ID_CHOICE1;
		static const long ID_STATICTEXT5;
		static const long ID_BUTTON4;
		static const long ID_CHECKBOX2;
		static const long ID_TEXTCTRL8;
		static const long ID_TEXTCTRL7;
		static const long ID_RADIOBUTTON1;
		static const long ID_RADIOBUTTON2;
		static const long ID_RADIOBUTTON3;
		static const long ID_TEXTCTRL9;
		static const long ID_STATICTEXT6;
		static const long ID_STATICTEXT7;
		static const long ID_RADIOBUTTON4;
		static const long ID_RADIOBUTTON5;
		//*)
		static const long SOCKET_ID;
		static const long TIMER_ID;

	private:

		//(*Handlers(mainwindow)
		void OnButton1Click(wxCommandEvent& event);
		void OnlogText(wxCommandEvent& event);
		void OnButton1Click1(wxCommandEvent& event);
		void OnRadioButton3Select(wxCommandEvent& event);
		//*)
        void OnSocketEvent(wxSocketEvent& event);
	protected:

		void BuildContent();
		void processPacket();

		DECLARE_EVENT_TABLE()
};

#endif
