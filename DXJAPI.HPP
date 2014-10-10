/*
	DXJAPI.HPP
*/

// Konstantenwerte anpassen
#define	BTX_STR_LEN		20
#define ANSCH_LEN		20
#define	TEILN_LEN		20
#define	BENUTZ_LEN		20
#define	PERS_LEN		20
#define	MAX_LIBNAME		20

#ifdef __BORLANDC__
 #define _System		__cdecl
#endif

extern "C" {

struct MODEMDATEN {
	ULONG	ulStructLength;
	ULONG	ulDelayAfterCommand;
	ULONG	ulCharByCharDelay;
	char	szInitialisierung	[BTX_STR_LEN];
	char	szAnwahl			[BTX_STR_LEN];
	char	szRufnummer			[BTX_STR_LEN];
	char	szEscape			[BTX_STR_LEN];
	char	szAbwahl			[BTX_STR_LEN];
	char	szVerbindung		[BTX_STR_LEN];
	char	szAnschlusskennung	[ANSCH_LEN	];
	char	szTlnNummer			[TEILN_LEN	];
	char	szMitbenutzerNummer	[BENUTZ_LEN	];
	char	szPersKennwort		[PERS_LEN	];
	USHORT	usDCUeberwachung;
	USHORT  usParitaet;
	USHORT  usStopBits;
	USHORT  usDataBits;
	USHORT  usBefehlsabschluss;
	ULONG	ulBaudrate;
	USHORT	usSchnittstelle;
};
typedef	MODEMDATEN*	PMODEMDATEN;

struct ISDNDATEN {
	ULONG	ulStructLength;
	char	szRufnummer			[BTX_STR_LEN];
	char	szAnschlusskennung	[ANSCH_LEN	];
	char	szTlnNummer			[TEILN_LEN	];
	char	szMitbenutzerNummer	[BENUTZ_LEN	];
	char	szPersKennwort		[PERS_LEN	];
	USHORT	usTimeOut;
	char	szLibName			[MAX_LIBNAME];
	USHORT  usEAZ;
	USHORT  usKartenNr;
};
typedef	ISDNDATEN*	PISDNDATEN;

void	_System DxjInitializeDLL();
void	_System DxjTerminateDLL();
USHORT	_System DxjOpen(PSZ pszDecoderFileName,
					PSZ pszAPIUserName,
					BOOL bStartHidden);
USHORT	_System DxjClose();
USHORT	_System DxjTerminate();
USHORT	_System DxjConnect(BOOL bModemConnect = TRUE);
USHORT	_System DxjDisconnect();
USHORT	_System DxjGetDecoderState(PUSHORT pusState);
USHORT	_System DxjReadText(USHORT usX1, USHORT usY1,
					USHORT usX2, USHORT usY2,
					PSZ pszText, USHORT usMaxLen);
USHORT	_System DxjWriteText(PSZ pszSendText);
USHORT	_System DxjWaitDCT(ULONG ulTimeOut);
USHORT	_System DxjGetDCTCount(PUSHORT pusDCTCount);
USHORT	_System DxjWaitForPage(PSZ pszPageNumber,
					ULONG ulTimeOut);
USHORT	_System DxjGetPageNumber(PSZ pszPageNumber,
					USHORT usMaxLen);

USHORT	_System DxjTerminal();
USHORT	_System DxjDisplay();
USHORT	_System DxjHidden();
USHORT	_System DxjSendTrpData(PBYTE pbData,
					ULONG ulDataLen,
					BOOL bDataEnd);
USHORT	_System DxjReceiveTrpData(PBYTE pbData,
					PULONG pulReadData,
					ULONG ulMaxLen, LONG lTimeout);

USHORT	_System DxjGetApiUserName(PSZ pszAPIUserName,
					USHORT usMaxNameLen);
USHORT	_System DxjGetErrorString(USHORT usAPIErrorCode,
					PSZ pszErrorString,
					USHORT usMaxErrorStringLen);

USHORT	_System DxjResetTrpBuffer();
USHORT	_System DxjGetOnlineTime(PSZ pszOnlineTime);
USHORT	_System DxjGetOnlineRate(PSZ pszOnlineRate);
USHORT	_System DxjEnablePrg(BOOL bEnablePrg);

USHORT	_System DxjGetDLLVersion(PULONG pulVersion);
USHORT	_System DxjGetEXEVersion(PULONG pulVersion);

USHORT	_System DxjEditOnlineSettings();
USHORT	_System DxjSetModemData(PMODEMDATEN pvData);
USHORT	_System DxjGetModemData(PMODEMDATEN pvData);
USHORT	_System DxjSetISDNData(PISDNDATEN pvData);
USHORT	_System DxjGetISDNData(PISDNDATEN pvData);
USHORT	_System DxjGetModemChannel(PSZ pszModemChannelName);
USHORT	_System DxjGetISDNChannel(PSZ pszISDNChannelName);

}	// extern "C"

