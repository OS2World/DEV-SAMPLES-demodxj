/*			Demo Datex-J Opalis 1.2 OS/2
	Compiler	: BCOS2 2.0, ICC 2.1, WATCOM 10.0a
	Datei		: WIN.CPP
				  OberflÑche und Datei-Bearbeitung
*/

#include "WIN.HPP"
#include "DXJW.HPP"

// *** globale Variablen ************************
Ueberw	ueberw;
HWND		hwndframe = NULL,
			hwndclient = NULL,
			hliste1 = NULL,
			hliste2 = NULL,
			hwndtemp;
PSZ		fehlerfeldname;
int		ueberwanz = 0;
BOOL		express = FALSE;
char		text[40];
PSZ		szclassname = "Rahmenfenster",
			szmaintitle = "Demo Opalis Datex-J/2",
			kontennamen = "Test-Konto Commerzbank";
fstream	datafile;

void		InsertUeberwDlg();
MRESULT	EXPENTRY Windowproc(HM12);
MRESULT	EXPENTRY UeberwDlgproc(HM12);

// *** main *************************************
int main() {
HAB	hab;
HMQ	hmq;
QMSG	qmsg;
ULONG	clientstyleflags = FCF_TITLEBAR	|
	FCF_SYSMENU		| FCF_SIZEBORDER		|
	FCF_MINMAX		| FCF_SHELLPOSITION	|
	FCF_TASKLIST	| FCF_MENU				|
	FCF_ICON;

	hab = WinInitialize(0);
	hmq = WinCreateMsgQueue(hab, 0);

	WinRegisterClass(hab, szclassname, Windowproc,
		CS_SIZEREDRAW, 0);

	hwndframe = WinCreateStdWindow(HWND_DESKTOP,
		WS_VISIBLE, &clientstyleflags,
		szclassname, szmaintitle,
		WS_VISIBLE, 0, ID_PM1, &hwndclient);

	while (WinGetMsg(hab, &qmsg, 0, 0, 0))
		WinDispatchMsg(hab, &qmsg);

	WinDestroyWindow(hwndframe);
	WinDestroyMsgQueue(hmq);
	WinTerminate(hab);

	return 0;
}

// *** Hilfsfunktionen **************************
BOOL Beep() {
	DosBeep(300, 35);
	DosBeep(600, 35);
	DosBeep(900, 35);
	return FALSE;
}

void InsertList1(PSZ text) {
	WinSendMsg(hliste1, LM_INSERTITEM,
		MPFROMSHORT(LIT_END), text);
}

void InsertList2(PSZ text) {
	WinPostMsg(hliste2, LM_INSERTITEM,
		MPFROMSHORT(LIT_END), text);
	DosSleep(50);
}

MRESULT WMCreate(HM12) {
	WinDefWindowProc(USE_HM12);
	hliste1 = WinCreateWindow(hwnd, WC_LISTBOX,
		"", WS_VISIBLE | LS_NOADJUSTPOS |
		LS_HORZSCROLL, 0, 0, 0, 0, hwnd, HWND_TOP,
		ID_LISTE1, NULL, NULL);
	hliste2 = WinCreateWindow(hwnd, WC_LISTBOX,
		"", WS_VISIBLE | LS_NOADJUSTPOS |
		LS_HORZSCROLL, 0, 0, 0, 0, hwnd, HWND_TOP,
		ID_LISTE2, NULL, NULL);

	char fontNameSize[] = "10.Helv";
	WinSetPresParam(hliste1, PP_FONTNAMESIZE,
		sizeof(fontNameSize), (void*)fontNameSize);
	WinSetPresParam(hliste2, PP_FONTNAMESIZE,
		sizeof(fontNameSize), (void*)fontNameSize);

	return (MRESULT)FALSE;
}

MRESULT WMSize(HWND, ULONG, MPARAM, MPARAM mp2) {
int cxclient = SHORT1FROMMP(mp2);
int cyclient = SHORT2FROMMP(mp2);
	int x = cxclient / 2;
	WinSetWindowPos(hliste1, HWND_TOP, 0, 0, x,
		cyclient, SWP_SIZE | SWP_MOVE | SWP_SHOW);
	WinSetWindowPos(hliste2, HWND_TOP, x, 0, x,
		cyclient, SWP_SIZE | SWP_MOVE | SWP_SHOW);
	return (MRESULT)FALSE;
}

void SetData(int index) {
PSZ	p = text;
	while (*p == ' ') p++;
PSZ	e = p + strlen(p);
	while ((*e == ' ') || (*e == '\0')) {
		if (e == p) break;
		*e = '\0';
		e--;
	}
	switch(index) {
		case 0:	strcpy(ueberw.name, p);
					break;
		case 1:	strcpy(ueberw.blz, "50040000");
					break;
		case 2:	strcpy(ueberw.kontonummer, p);
					break;
		case 3:	strcpy(ueberw.betragdm,		p);
					break;
		case 4:	strcpy(ueberw.betragpf,		p);
					break;
		case 5:	strcpy(ueberw.zweck1,		p);
					break;
		case 6:	strcpy(ueberw.zweck2,		p);
	}
}

// Daten laden und anfÅgen oder Åberschreiben
void LoadFile(BOOL clear) {
	datafile.open(DATAFILENAME, ios::in);
	if (datafile) {
		if (clear) {
			WinSendMsg(hliste1, LM_DELETEALL,
				0 , 0);
			ueberwanz = 0;
		}
		while (!datafile.eof()) {
			for (int x = 0; x < 7; x++) {
				datafile.get(text, sizeof(text),
					(x < 6) ? ',' : '\n');
				datafile.get();
				SetData(x);
			}
			if (strlen(ueberw.name))
				InsertUeberwDlg();
		}
		datafile.close();
	} else {
		WinMessageBox(HWND_DESKTOP, hwndclient,
			"Fehler beim Laden der Daten",
			"Daten laden", 1, MB_MOVEABLE);
	}
}

void GetData(int index) {
	WinSendMsg(hliste1, LM_QUERYITEMTEXT,
		MPFROM2SHORT(index, sizeof(text)), text);
}

void SaveFile() {
	datafile.open(DATAFILENAME, ios::out);
	if (datafile) {
		int offset = 0;
		for (int nr = 0; nr < ueberwanz; nr++) {
			int index = 1 + offset;
			for (int x = 0; x < 7; x++) {
				GetData(index++);
				datafile << text
					<< ((x < 6) ? ", " : "\n");
			}
			offset += 8;
		}
		datafile.flush();
		datafile.close();
	} else {
		WinMessageBox(HWND_DESKTOP, hwndclient,
			"Fehler beim Speichern der Daten",
			"Daten speichern", 1, MB_MOVEABLE);
	}
}

void SaveInProtokoll() {
	if (!datafile) return;
	datafile << "*** öberweisung *******************************"
				<< endl;
#ifndef __IBMCPP__
	dosdate_t	day;         
	dostime_t	time;
	_dos_getdate(&day);
	_dos_gettime(&time);
	datafile << "vom          : " << setfill('0')
				<< setw(2) << (int)day.day << "."
				<< setw(2) << (int)day.month << "."
				<< setw(2) << (int)day.year << " "
				<< setw(2) << (int)time.hour
				<< ":" << setw(2) << (int)time.minute
				<< ":" << setw(2) << (int)time.second
				<< endl;
#endif
	datafile
		<< "von Konto    : Testkonto COMMERZBANK"
		<< endl << endl
		<< "EmpfÑnger    : " << ueberw.name << endl
		<< "BLZ          : " << ueberw.blz << endl
		<< "Bankname     : COMMERZBANK FRANKFURT"
		<< endl
		<< "Konto-Nummer : " << ueberw.kontonummer
		<< endl
		<< "Betrag DM,Pf : " << ueberw.betragdm
		<< ",";
	if (strlen(ueberw.betragpf) == 1)
		datafile << "0";
	datafile	<< ueberw.betragpf << endl
		<< "Zweck        : " << ueberw.zweck1
		<< endl << "             : "
		<< ueberw.zweck2 << endl << endl;
}

MRESULT WMCommand(HWND hwnd, ULONG,
	MPARAM mp1, MPARAM) {
	switch (SHORT1FROMMP(mp1)) {
		case IDM_LOAD: {
		BOOL clear = FALSE;
			if (ueberwanz) {
				if (WinMessageBox(
					HWND_DESKTOP, hwnd,
					"mîchten Sie die neuen Daten\n"
					"an die vorhandenen anfÅgen ?",
					"Daten laden", 1,
					MB_YESNO | MB_QUERY | MB_MOVEABLE)
					!= MBID_YES) clear = TRUE;
			}
			LoadFile(clear);
			break;
		}
		case IDM_SAVE: {
			if (!ueberwanz) return (MRESULT)Beep();
			SaveFile();
			break;
		}

		case IDM_DATANEW:
			WinDlgBox(HWND_DESKTOP, hwnd,
				UeberwDlgproc, 0,
				IDD_UEBERWEISUNG, NULL);
			WinInvalidateRect(hwnd, 0, 0);
			break;

		case IDM_DATACLEARLAST:
			if (!ueberwanz) return (MRESULT)Beep();
			if (WinMessageBox(HWND_DESKTOP, hwnd,
				"mîchten Sie die letzten "
				"Eingabedaten lîschen ?",
				"Eingabedaten lîschen", 1,
				MB_YESNO | MB_QUERY | MB_MOVEABLE |
				MB_DEFBUTTON2) == MBID_YES) {
					int index = (ueberwanz - 1) * 8;
					for (int i = 0; i < 8; i++)
						WinSendMsg(hliste1,
							LM_DELETEITEM,
							MPFROMSHORT(index), 0);
					ueberwanz--;
				}
			break;

		case IDM_DATACLEARALL:
			WinSendMsg(hliste2, LM_DELETEALL, 0 , 0);
			if (!ueberwanz) return (MRESULT)Beep();
			if (WinMessageBox(HWND_DESKTOP, hwnd,
				"mîchten Sie alle "
				"Eingabedaten lîschen ?",
				"Eingabedaten lîschen", 1,
				MB_YESNO | MB_QUERY | MB_MOVEABLE |
					MB_DEFBUTTON2) == MBID_YES) {
					WinSendMsg(hliste1, LM_DELETEALL,
						0 , 0);
					ueberwanz = 0;
				}
			break;

#ifdef __BORLANDC__
   #define STARTTHREAD(function, arglist)       \
      _beginthread(function, 4096, arglist)
#else
   #define STARTTHREAD(function, arglist)       \
      _beginthread(function, NULL, 4096, arglist)
#endif

		case IDM_ONLINE:
			if (online) return (MRESULT)Beep();
			// *** Anwahl-Thread *******************
			express = FALSE;
			STARTTHREAD(GoOnline, NULL);
			break;

		case IDM_OFFLINE:
			if (!online) return (MRESULT)Beep();
			// *** Abwahl-Thread *******************
			STARTTHREAD(GoOffline, NULL);
			express = FALSE;
			break;

		case IDM_UEBERWSEND: {
			if (!ueberwanz) return (MRESULT)Beep();
			if (!online) {
				// *** Online gehen *****************
				express = TRUE;
				STARTTHREAD(GoOnline, (void*)hwnd);
				break;
			}
			// *** Dialog-Thread *******************
			STARTTHREAD(UeberwSend,
				(express) ? (void*)hwnd : NULL);
			break;
		}
		case IDM_EXIT:
			WinSendMsg (hwnd, WM_CLOSE, 0L, 0L);
			break;
	}
	return (MRESULT)FALSE;
}

MRESULT EXPENTRY Windowproc(HM12) {
	switch(msg) {
		case WM_CREATE:
			return WMCreate(USE_HM12);
		case WM_SIZE:
			return WMSize(USE_HM12);
		case WM_COMMAND:
			return WMCommand(USE_HM12);
		case WM_CLOSE:
			WinPostMsg(hwnd, WM_QUIT, 0, 0);
			return FALSE;
	}
	return WinDefWindowProc(hwnd, msg, mp1, mp2);
}

void SetControlText(ULONG ID,
	PSZ Data, ULONG max) {
HWND h = WinWindowFromID(hwndtemp, ID);
	WinSendMsg(h, EM_SETTEXTLIMIT,
		MPFROMSHORT(max - 1), NULL);
	WinSetWindowText(h, Data);
}

int QueryControlText(ULONG ID, PSZ Data,
	ULONG size) {
	WinQueryWindowText(WinWindowFromID
		(hwndtemp, ID), size, Data);
int ok = 0,
	 y = strlen(Data);
	if (!y) ok = -1;
	else {
		for(UINT x = 0; x < y; x++) {
			if ((Data[x] < '0') ||
				(Data[x] > '9')) {
				ok = -2;
				break;
			}
		}
	}
	return ok;
}

void SetUeberwDlg() {
	WinSendDlgItemMsg(hwndtemp, ID_Kontoname,
		SPBM_SETARRAY, &kontennamen,
		MPFROMSHORT(1));

	SetControlText(ID_Empf,		ueberw.name,
		sizeof(ueberw.name));
	SetControlText(ID_Konto,	ueberw.kontonummer,
		sizeof(ueberw.kontonummer));
	SetControlText(ID_DM,		ueberw.betragdm,
		sizeof(ueberw.betragdm));
	SetControlText(ID_Pf,		ueberw.betragpf,
		sizeof(ueberw.betragpf));
	SetControlText(ID_Zweck1,	ueberw.zweck1,
		sizeof(ueberw.zweck1));
	SetControlText(ID_Zweck2,	ueberw.zweck2,
		sizeof(ueberw.zweck2));

	WinInvalidateRect(hwndtemp, 0, 0);
}

int GetUeberwDlg() {
int ok;
	if ((ok = QueryControlText(ID_Empf,
		ueberw.name, sizeof(ueberw.name))) == -1) {
			fehlerfeldname = "EmpfÑnger";
			return ok;
	}
	if ((ok = QueryControlText(ID_Konto,
		ueberw.kontonummer,
		sizeof(ueberw.kontonummer))) < 0) {
			fehlerfeldname = "Kontonummer";
			return ok;
	}
	if ((ok = QueryControlText(ID_DM,
		ueberw.betragdm,
		sizeof(ueberw.betragdm))) < 0) {
			fehlerfeldname = "Betrag DM";
			return ok;;
	}
	if ((ok = QueryControlText(ID_Pf,
		ueberw.betragpf,
		sizeof(ueberw.betragpf))) < 0) {
			fehlerfeldname = "Betrag Pf";
			return ok;
	}
	if ((ok = QueryControlText(ID_Zweck1,
		ueberw.zweck1,
		sizeof(ueberw.zweck1))) == -1) {
			fehlerfeldname = "Verwendungszweck 1";
			return ok;;
	}
	QueryControlText(ID_Zweck2, ueberw.zweck2,
		sizeof(ueberw.zweck2));
	return 0;
}

void InsertUeberwDlg() {
strstream s;
	s	<< "--- öberweisung : " << ++ueberwanz
		<< " ---------";
	InsertList1(s.str());
	InsertList1(ueberw.name);
	InsertList1("50040000");
	InsertList1(ueberw.kontonummer);
	InsertList1(ueberw.betragdm);
	InsertList1(ueberw.betragpf);
	InsertList1(ueberw.zweck1);
	InsertList1(ueberw.zweck2);
}

MRESULT EXPENTRY UeberwDlgproc(HM12) {
	hwndtemp = hwnd;
	switch (msg) {
		case WM_INITDLG:
			SetUeberwDlg();
			return WinDefDlgProc(USE_HM12);

		case WM_COMMAND:
			switch (SHORT1FROMMP(mp1)) {
				case DID_OK: {
				int ok = GetUeberwDlg();
					if (ok) {
						WinMessageBox(HWND_DESKTOP,
							hwnd, (ok == -1) ?
							"fehlender Wert" :
							"ungÅltiger Wert",
							fehlerfeldname, 1,
							MB_ICONQUESTION |
							MB_MOVEABLE );
						break;
					} else {
						InsertUeberwDlg();
					}
				}
				case DID_CANCEL:
					WinDismissDlg(hwnd, TRUE);
					return (MRESULT)FALSE;
			}
			break;
		default:
			return WinDefDlgProc(USE_HM12);
	}
	return (MRESULT)FALSE;
}

