/**********************************************************
		Demonstration Opalis C-Schnittstelle

	Datei		: DEMODXJ.CPP
	OS			: OS/2 2.x ; 3.0
	Compiler	: Borland C++ 1.5 ; 2.0 fÅr OS/2
				  IBM C/C++ 2.01
	Autor		: Raoul Gema

benîtigte Dateien:
* DEMODXJ.CPP		- dieser Quelltext
* DXJAPI.HPP		- C++ -Header
* DEMODXJ.MAK		- MAKE-Datei fÅr BCOS2 und ICC
* DEMODXJB.DEF		- Moduldef. fÅr BCOS2 DEMODXJB.EXE
* DEMODXJI.DEF		-           fÅr ICC   DEMODXJI.EXE

* x:\OPALIS\OPALIS.EXE	- Opalis Datex-J-Decoder 1.2
* x:\OS2\DLL\DXJAPI.DLL	- Opalis C-Schnittstellen DLL
**********************************************************/

// C++ -Header define & include
#include <stdio.h>
#include <iostream.h>
#include <string.h>
#include <fstream.h>
#include <conio.h>

// OS define & include
#include <os2.h>

// Anwendung define & include
#include "DXJAPI.HPP"
#define	LF			'\n'
#define	DECODER		"E:\\OPALIS\\OPALIS.EXE"
#define	DATEINAME	"DEMO001.TXT"
#define SENDEAUSZUG	"*38900#^3^6^3^#^999999912345^2^1^"
#define SENDEUEBERW	"*38900#^3^6^3^#^9999999^12345^3^1^#^"

#ifdef __BORLANDC__
	#define	FLUSH
#else
	#define	FLUSH		cout.flush()
#endif
// *** globale Variablen **********************************
char	text[256];
char	seite[1024];

// *** Ausgabe von Fehlermeldungen als Text ***************
USHORT
Meldung(USHORT fehlernr) {
	if (fehlernr) {
		cout << "\n*** DXJAPI.DLL-Fehler : "
			<< fehlernr << "***\n";
		DxjGetErrorString(fehlernr, (PSZ)text, sizeof(text));
		cout << text; FLUSH;
	}
	return fehlernr;
}

// ** Senden von "text" mit Auswertung von Sonderzeichen **
void
Senden(PSZ text) {
UINT	x = strlen((const char*)text);
// char sz[2] = "\0\0";								// BCOS2
char	sz[2]; sz[0] = '\0'; sz[1] = '\0';
	for (UINT i = 0; i < x; i++) {
		sz[0] = text[i];
		cout << sz; FLUSH;
		switch (sz[0]) {
			case '^':	DxjWaitDCT(15); 			break;
			case '*':   DxjWriteText((PSZ)"\\*");   break;
			case '#':   DxjWriteText((PSZ)"\\#");   break;
			default:    DxjWriteText((PSZ)sz);
		}
	}
}

void Dialog1();
void Dialog2();
// *** main()**********************************************
int main(int /*argc*/, char* argv[]) {
try {
	cout << "\n*** Demo Opalis-DLL-API ***\n"; FLUSH;
	DxjInitializeDLL();					// DLL-Init
	if (!Meldung(DxjOpen((PSZ)DECODER,	// Decoder laden
		  (PSZ)argv[0], FALSE))) {      // FALSE = sichtbar
		cout << "\n<Taste> um die Verbindung aufzubauen"; FLUSH;
		getch();
		Meldung(DxjConnect());			// Anwahl

		Dialog2();						// und Action ...

		cout << "n<Taste> um die Verbindung zu beenden"; FLUSH;
		getch();
		Meldung(DxjDisconnect());   	// TschÅ· ...
		Meldung(DxjTerminate());
		Meldung(DxjClose());			// Decoder freigeben
	}
	DxjTerminateDLL();              	// DLL freigeben
	cout << "\n*** OK ***"; FLUSH;
	return 0;                       	// ... und weg.
} catch (...) {
	return -1;
}
}

/*** AusfÅhren des DATEX-J -Dialoges am Beispiel
 des îffentlichen Testkontos des Commerzbank
 Auszug einlesen							 		*/

void Dialog1() {
	// Testkonto Commerzbank - Auszug lesen
	cout << "\nTestkonto Commerzbank aufrufen";
	cout << "\nsenden :"; FLUSH;
	Senden((PSZ)SENDEAUSZUG);				 	// BTX-Talk
ofstream	datei(DATEINAME);
	if (datei) {
		cout << "\nSeite einlesen\n"; FLUSH;	// Seite lesen
		Meldung(DxjReadText(0, 0, 39, 24,
					(PSZ)seite, sizeof(seite)));
		datei << seite << LF;   				// Seite speichern
		datei.flush();
		datei.close();
	}
}

void Dialog2() {
	// Testkonto Commerzbank - EinzelÅberweisung
	cout << "\nTestkonto Commerzbank : EinzelÅberweisung";
	cout << "\nsenden :"; FLUSH;
	Senden((PSZ)SENDEUEBERW);				 	// BTX-Talk
ofstream	datei(DATEINAME);
	if (datei) {
		// Formular-Seite Erfassung Einzel-öberweisung

		// BLZ ist vorgegeben
		// Institut wird automatisch eingefÅgt
		Senden((PSZ)"123456#^Otto Schubert#^100#^25^");
		Senden((PSZ)"Rnr.: 123/95#^vom : 12.01.95#^#^");
		// Anzeige zur PrÅfung
		Senden((PSZ)"#^");
		// BestÑtigung mit TAN und PIN
		Senden((PSZ)"123456^98765^#^");
		cout << "\nSeiteninhalt einlesen\n"; FLUSH;
		Meldung(DxjReadText(0, 0, 39, 23, (PSZ)seite, sizeof(seite)));
		datei << seite << LF;

		// 9 senden Ende   # weitere öberweisungen
		Senden((PSZ)"9^*9#");

		datei.flush();
		datei.close();
	}
}
// *** EOF ************************************************

