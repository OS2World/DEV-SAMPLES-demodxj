/* 			Demo Datex-J Opalis 1.2 OS/2
	Compiler	: BCOS2 2.0, ICC 2.1, WATCOM 10.0a
	Datei		: DXJW.CPP
				  Datex-J Funktionen in Threads
*/

// Anwendung define & include
#include "WIN.HPP"
#include "DXJAPI.HPP"	// Opalis Datex-J API

#define TAN					"9999999"
#define PIN					"12345"
#define SENDEUEBERW1		"*38900#^11^43^#^"
#define SENDEUEBERW2		"^3^1^#^"

// #define _TEST_		// fÅr Versuche nicht online gehen

// *** globale Variablen ************************
char	dxjtext[256];
BOOL	online = FALSE;

// *** Ausgabe von Fehlermeldungen als Text *****
USHORT Meldung(USHORT fehlernr) {
	if (fehlernr) {
		DxjGetErrorString(fehlernr, (PSZ)dxjtext,
			sizeof(dxjtext));
		InsertList2(dxjtext);
	}
	return fehlernr;
}

// *** Anwahl-Thread ****************************
void _THREADLINK_
GoOnline(void* param) {

char	decoder[128];
	try {
		InsertList2("* Start Anwahl *");
		DxjInitializeDLL();
		PrfQueryProfileString(HINI_USERPROFILE,
			(PSZ)"ABRBTX", (PSZ)"ABRBASEDIR",
			(PSZ)"", (PSZ)decoder, sizeof(decoder));
		strcat(decoder, "OPALIS.EXE");
		// Decoder laden ; FALSE = sichtbar
		if (!Meldung(DxjOpen((PSZ)decoder,
				(PSZ)"WIN1B", /*TRUE*/ FALSE))) {
#ifdef _TEST_			
			online = TRUE;// nach Test-Ende entfernen
#endif
		} else {
			InsertList2("* Fehler : Decoder laden *");
		}
#ifndef _TEST_
		if (!Meldung(DxjConnect(TRUE))) {
			InsertList2("* Verbindungsaufbau Ok *");
			online = TRUE;
		}
#endif
		// beim Express-Senden - öberweisen starten
		if ((param) && (online)) {
			WinPostMsg((HWND)param, WM_COMMAND,
				(MPARAM)IDM_UEBERWSEND, 0);
			DosSleep(200);
		}
	} catch (...) {
	}
	_endthread();
}

// *** Abwahl-Thread ****************************
void _THREADLINK_
GoOffline(void* /*param*/) {
	try {
		InsertList2("* Start Abwahl *");
#ifndef _TEST_
		if (!Meldung(DxjDisconnect())) {
			InsertList2("* Verbindung wurde beendet *");
		}
#endif
		Meldung(DxjTerminate());
		Meldung(DxjClose());
		DxjTerminateDLL();
	} catch (...) {
	}
	InsertList2("* Abwahl OK *");
	online = FALSE;
	_endthread();
}

// *** Senden von "sendtext" mit
// Auswertung von Sonderzeichen ***
void Senden(PSZ sendtext) {
#ifndef _TEST_
char	sz[2];
		sz[0] = '\0'; sz[1] = '\0';
UINT	x = strlen((const char*)sendtext);
	for (UINT i = 0; i < x; i++) {
		sz[0] = sendtext[i];
		switch (sz[0]) {
			case '^':	DxjWaitDCT(15);
							break;
			case '*':	DxjWriteText((PSZ)"\\*");
							break;
			case '#':	DxjWriteText((PSZ)"\\#");
							break;
			default:		DxjWriteText((PSZ)sz);
		}
	}
#endif
}

// *** eine öberweisung senden ******************
void Send() {
	for (int x = 0; x < 7; x++) {
		WinPostMsg(hliste1, LM_QUERYITEMTEXT,
			MPFROM2SHORT(0, sizeof(text)), text);
		DosSleep(100);
		WinPostMsg(hliste2, LM_INSERTITEM,
			MPFROMSHORT(LIT_END), text);
		DosSleep(100);
		WinPostMsg(hliste1, LM_DELETEITEM,
			MPFROMSHORT(0), 0);
		DosSleep(100);
		SetData(x);
	}
	SaveInProtokoll();

	// *** Reihenfolge an Bank anpassen ***
	// BLZ ist vorgegeben
	// Institut wird automatisch eingefÅgt
	Senden((PSZ)ueberw.kontonummer);
	Senden((PSZ)"#^");
	Senden((PSZ)ueberw.name);
	Senden((PSZ)"#^");
	Senden((PSZ)ueberw.betragdm);
	Senden((PSZ)"#^");
	if (strlen(ueberw.betragpf) == 1)
		Senden((PSZ)"0");
	Senden((PSZ)ueberw.betragpf);
	Senden((PSZ)"^");
	Senden((PSZ)ueberw.zweck1);
	Senden((PSZ)"#^");
	Senden((PSZ)ueberw.zweck2);
	Senden((PSZ)"#^#^");
	// Anzeige zur PrÅfung
	Senden((PSZ)"#^");
	// BestÑtigung mit TAN und PIN
	Senden((PSZ)TAN);
	Senden((PSZ)"^");
	Senden((PSZ)PIN);
	Senden((PSZ)"^#^");
}

// *** Dialog-Thread ****************************
// alle aktuellen öberweisungen senden
void _THREADLINK_
UeberwSend(void* param) {
	WinPostMsg(hliste2, LM_DELETEALL, 0, 0);
	DosSleep(100);

	InsertList2((express) ?
		"* Start Express-Senden *" :
		"* Beginn öberweisung-Senden *");
	datafile.open(PROTOKOLLFILE,
		ios::out | ios::app);
	if (!datafile) {
		Beep();
		InsertList2("Fehler beim Speichern"
			" der Protokoll-Daten");
	}
	Senden((PSZ)SENDEUEBERW1);
	Senden((PSZ)TAN);
	Senden((PSZ)PIN);
	Senden((PSZ)SENDEUEBERW2);

	for (int nr = 0; nr < ueberwanz; nr++) {
		WinPostMsg(hliste1, LM_QUERYITEMTEXT,
			MPFROM2SHORT(0, sizeof(text)), text);
		DosSleep(100);
		InsertList2(text);

		WinPostMsg(hliste1, LM_DELETEITEM,
			MPFROMSHORT(0), 0);
		DosSleep(100);
		// eine einzelne öberweisung senden
		Send();
		// "#" weitere öberweisung ; "9" Ende
		Senden((nr + 1 < ueberwanz) ?
			(PSZ)"#^" : (PSZ)"9^");
	}
	// *** Ende Bank-Dialog ***
	Senden((PSZ)"*9#");

	ueberwanz = 0;
	if (datafile) {
		datafile.flush();
		datafile.close();
	}
	InsertList2((express) ?
		"* Stop Express-Senden *" :
		"* öberweisung ist beendet *");
	// nach Express-Senden auflegen
	if (param) {
		DosSleep(3000);
		WinPostMsg((HWND)param, WM_COMMAND,
			(MPARAM)IDM_OFFLINE, 0);
	}
	_endthread();
}

