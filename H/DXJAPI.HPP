/******************************************************
	BCOS2 2.0	Opalis 1.2 C-Schnittstelle

	DXJAPI.HPP
	Opalis 1.2 C-Schnittstelle, als 32Bit DLL			
	Copyright (C) Borris und Rîben GbR 1993,1994
	Autor: Achim Galonska       Stand: 21.09.1994

	Anpassung fÅr BCOS2 2.0, IBM C/C++ 2.1, WATCOM 10.0a
	Raoul Gema
******************************************************/

#ifdef __BORLANDC__
	#undef  EXPENTRY
	#define EXPENTRY	__cdecl
#endif

extern "C" {

// Konstanten - Fehler-Codes
#define DXJ_ERROR_NO_ERROR					0
#define DXJ_ERROR_FILE_NOT_FOUND			1
#define DXJ_ERROR_ALREADY_STARTED		2
#define DXJ_ERROR_ALREADY_CLOSED			3
#define DXJ_ERROR_ALREADY_TERMINATED	4
#define DXJ_ERROR_ALREADY_ONLINE			5
#define DXJ_ERROR_ALREADY_DISCONNECTED	6
#define DXJ_ERROR_ACCES_DENIED			7
#define DXJ_ERROR_TIMEOUT_BREAK			8
#define DXJ_ERROR_INTERNAL					9
#define DXJ_ERROR_ONLINE_LOCKED			10
#define DXJ_ERROR_NOT_ONLINE				11
#define DXJ_ERROR_INVALID_PARAMETER		12
#define DXJ_ERROR_USER_BREAK				13
#define DXJ_ERROR_ALREADY_WAITING		14
#define DXJ_ERROR_INVALID_CONNECTION	15
#define DXJ_ERROR_CANT_WAIT_SUCCESS		16
#define DXJ_ERROR_TRP_NOT_AVAILABLE		17
#define DXJ_ERROR_NO_TRP_DATA				18
#define DXJ_ERROR_DCT_BREAK				19
#define DXJ_ERROR_TRP_OVERFLOW			20
#define DXJ_ERROR_INTERNAL_DOSALLOC		21
#define DXJ_ERROR_INTERNAL_MAXLOOPS		22
#define DXJ_ERROR_INTERNAL_WRONGMSG		23
#define DXJ_ERROR_INTERNAL_NOREADPIPE	24
#define DXJ_ERROR_NO_USER_AVAILABLE		25
#define DXJ_ERROR_NO_TEXT_AVAILABLE		26
#define DXJ_ERROR_INVALID_TRP_DATA		27

// Konstanten - Verbindungsdaten
#define ANSCH_LEN			13
#define TEILN_LEN			25
#define BENUTZ_LEN		5
#define PERS_LEN			9
#define BTX_STR_LEN		81
#define MAX_LIBNAME		255

// Konstanten - ParitÑt
#define KEINE				0
#define UNGERADE			1
#define GERADE				2
#define MARKIERUNG		3
#define LEERZEICHEN		4

// Konstanten - Befehlsabschlu·
#define CR					0
#define LF					1
#define CRLF				2

// Konstanten - Schnittstelle
#define COM1				0
#define COM2				1
#define COM3				2
#define COM4				3

// Konstanten - Stopbits
#define STOP1				0
#define STOP15				1
#define STOP2				2

// Konstanten Datenbits
#define DATEN5				0
#define DATEN6				1
#define DATEN7				2
#define DATEN8				3

// Datenstrukturen
struct MODEMDATEN {
	ULONG	ulStructLength;
	ULONG	ulDelayAfterCommand;
	ULONG	ulCharByCharDelay;
	char	szInitialisierung		[BTX_STR_LEN];
	char	szAnwahl					[BTX_STR_LEN];
	char	szRufnummer				[BTX_STR_LEN];
	char	szEscape					[BTX_STR_LEN];
	char	szAbwahl					[BTX_STR_LEN];
	char	szVerbindung			[BTX_STR_LEN];
	char	szAnschlusskennung	[ANSCH_LEN	];
	char	szTlnNummer				[TEILN_LEN	];
	char	szMitbenutzerNummer	[BENUTZ_LEN	];
	char	szPersKennwort			[PERS_LEN	];
	USHORT	usDCUeberwachung;
	USHORT	usParitaet;
	USHORT	usStopBits;
	USHORT	usDataBits;
	USHORT	usBefehlsabschluss;
	ULONG		ulBaudrate;
	USHORT	usSchnittstelle;
};
typedef	MODEMDATEN*	PMODEMDATEN;

struct ISDNDATEN {
	ULONG	ulStructLength;
	char	szRufnummer				[BTX_STR_LEN];
	char	szAnschlusskennung	[ANSCH_LEN	];
	char	szTlnNummer				[TEILN_LEN	];
	char	szMitbenutzerNummer	[BENUTZ_LEN	];
	char	szPersKennwort			[PERS_LEN	];
	USHORT	usTimeOut;
	char		szLibName			[MAX_LIBNAME];
	USHORT	usEAZ;
	USHORT	usKartenNr;
};
typedef	ISDNDATEN*	PISDNDATEN;

// exportiere Funfionen
void		EXPENTRY DxjInitializeDLL();
void		EXPENTRY DxjTerminateDLL();
USHORT	EXPENTRY DxjOpen(PSZ pszDecoderFileName,
					PSZ pszAPIUserName,
					BOOL bStartHidden);
USHORT	EXPENTRY DxjClose();
USHORT	EXPENTRY DxjTerminate();
USHORT	EXPENTRY DxjConnect(BOOL bModemConnect = TRUE);
USHORT	EXPENTRY DxjDisconnect();
USHORT	EXPENTRY DxjGetDecoderState(PUSHORT pusState);
USHORT	EXPENTRY DxjReadText(USHORT usX1, USHORT usY1,
					USHORT usX2, USHORT usY2,
					PSZ pszText, USHORT usMaxLen);
USHORT	EXPENTRY DxjWriteText(PSZ pszSendText);
USHORT	EXPENTRY DxjWaitDCT(ULONG ulTimeOut);
USHORT	EXPENTRY DxjGetDCTCount(PUSHORT pusDCTCount);
USHORT	EXPENTRY DxjWaitForPage(PSZ pszPageNumber,
					ULONG ulTimeOut);
USHORT	EXPENTRY DxjGetPageNumber(PSZ pszPageNumber,
					USHORT usMaxLen);

USHORT	EXPENTRY DxjTerminal();
USHORT	EXPENTRY DxjDisplay();
USHORT	EXPENTRY DxjHidden();
USHORT	EXPENTRY DxjSendTrpData(PBYTE pbData,
					ULONG ulDataLen,
					BOOL bDataEnd);
USHORT	EXPENTRY DxjReceiveTrpData(PBYTE pbData,
					PULONG pulReadData,
					ULONG ulMaxLen, LONG lTimeout);

USHORT	EXPENTRY DxjGetApiUserName(PSZ pszAPIUserName,
					USHORT usMaxNameLen);
USHORT	EXPENTRY DxjGetErrorString(USHORT usAPIErrorCode,
					PSZ pszErrorString,
					USHORT usMaxErrorStringLen);

USHORT	EXPENTRY DxjResetTrpBuffer();
USHORT	EXPENTRY DxjGetOnlineTime(PSZ pszOnlineTime);
USHORT	EXPENTRY DxjGetOnlineRate(PSZ pszOnlineRate);
USHORT	EXPENTRY DxjEnablePrg(BOOL bEnablePrg);

USHORT	EXPENTRY DxjGetDLLVersion(PULONG pulVersion);
USHORT	EXPENTRY DxjGetEXEVersion(PULONG pulVersion);

USHORT	EXPENTRY DxjEditOnlineSettings();
USHORT	EXPENTRY DxjSetModemData(PMODEMDATEN pvData);
USHORT	EXPENTRY DxjGetModemData(PMODEMDATEN pvData);
USHORT	EXPENTRY DxjSetISDNData(PISDNDATEN pvData);
USHORT	EXPENTRY DxjGetISDNData(PISDNDATEN pvData);
USHORT	EXPENTRY DxjGetModemChannel(PSZ pszModemChannelName);
USHORT	EXPENTRY DxjGetISDNChannel(PSZ pszISDNChannelName);

}	// extern "C"

