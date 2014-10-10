/*			Demo Datex-J Opalis 1.2 OS/2
	Compiler	: BCOS2 2.0, ICC 2.1, WATCOM 10.0a
	Datei		: COMMON.HPP
				  compilerspezifische Deklarationen
*/

#ifdef __USE_DLL__
	#define EXTERN extern
#else
	#define EXTERN
#endif

#ifdef __BORLANDC__
	#define EXPORT          _export
	#define PFCHAR          char*
	#define _THREADLINK_
#endif

#ifdef __IBMCPP__
	#define EXPORT          _Export
	#define PFCHAR          char*
	#define _THREADLINK_    _Optlink
#endif

#ifdef __WATCOMC__
	#define EXPORT          _export
	#define PFCHAR          char far*
	#define _THREADLINK_
#endif

