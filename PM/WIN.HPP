/*			Demo Datex-J Opalis 1.2 OS/2
	Compiler	: BCOS2 2.0, ICC 2.1, WATCOM 10.0a
	Datei		: WIN.HPP
*/

// C++ -Header define & include
#include <string.h>
#include <fstream.h>
#include <iomanip.h>
#include <strstrea.h>
#include <process.h>
#include <stdio.h>
#ifndef __IBMCPP__
   #include <dos.h>
#endif

// OS define & include
#define INCL_WIN
#define INCL_DOSPROCESS
#include <os2.h>

// Anwendung define & include
#include "common.hpp"
#include "win.rc"

#define	HM12			HWND hwnd, ULONG msg, \
							MPARAM mp1, MPARAM mp2
#define	USE_HM12		hwnd, msg, mp1, mp2

#define	DATAFILENAME	"DXJDATA.LST"
#define	PROTOKOLLFILE	"DXJPROTOK.LST"

#define	ID_LISTE1	50
#define	ID_LISTE2	51

// **********************************************
class Ueberw {
public:
			Ueberw() {
				name[0] = blz[0] = kontonummer[0] =
				betragdm[0] = betragpf[0] =
				zweck1[0] = zweck2[0] =
				tan[0] = '\0';
			}

	char	name[28];
	char	blz[9];
	char	kontonummer[11];
	char	betragdm[11];
	char	betragpf[3];
	char	zweck1[28];
	char	zweck2[28];
	char	tan[7];
};

// *** globale Daten und Funktionen *************
extern	Ueberw	ueberw;
extern	HWND		hwndclient,
						hliste1,
						hliste2;
extern	int		ueberwanz;
extern	BOOL		express;
extern	char		text[40];
extern	BOOL		online;
extern	fstream	datafile;

extern	BOOL		Beep();
extern	void		InsertList2(PSZ text);
extern	void		GetData(int x);
extern	void		SetData(int x);
extern	void		SaveInProtokoll();

