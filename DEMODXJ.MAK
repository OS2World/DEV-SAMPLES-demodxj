# *********************************************************
#
#		Demonstration Opalis C-Schnittstelle
#
#	DEMODXJ.MAK
#   Aufruf	BCOS2	MAKE.EXE  -f DEMODXJ.MAK BCC=1
#           CSet	NMAKE.EXE -f DEMODXJ.MAK ICC=1

NAME		= DEMODXJ
SOURCEFILE	= $(NAME).CPP
MAKEFILE	= $(NAME).MAK

# *** BCOS2 DEMODXJB.EXE **********************************
!ifdef BCC
COMPILER	= BCC.EXE
LINKER		= TLINK.EXE
LIBPFAD		= E:\BCOS2\LIB

EXT			= B
LIBRARYS	= $(LIBPFAD)\C2I.LIB $(LIBPFAD)\OS2.LIB
CPPOPTION 	= -c -I$E:\BCOS2\INCLUDE -o"$(SOURCEOBJ)"
LINKOPTION	= /x /ap
LINKOBJ		= $(LIBPFAD)\C02.OBJ

!endif
# *** ICC DEMODXJI.EXE ************************************
!ifdef ICC

COMPILER	= ICC.EXE
LINKER		= LINK386.EXE

EXT			= I
LIBRARYS	= D:\IBMCPP\LIB\DDE4MBSI.LIB	\
			  D:\TOOLKT21\OS2LIB\OS2386.LIB
LINKOPTION	= /nologo
CPPOPTION	= /C /Fo"$(SOURCEOBJ)"

!endif
# *********************************************************
TARGET		= $(NAME)$(EXT).EXE
SOURCEOBJ	= $(NAME)$(EXT).OBJ
LINKOBJ		= $(LINKOBJ) $(SOURCEOBJ)
DEFFILE		= $(NAME)$(EXT).DEF

$(TARGET): $(SOURCEOBJ) $(DEFFILE) $(MAKEFILE)
	$(LINKER) $(LINKOPTION) $(LINKOBJ), $(TARGET), ,	\
		$(LIBRARYS), $(DEFFILE)

$(SOURCEOBJ): $(SOURCEFILE) $(MAKEFILE)
	$(COMPILER) $(CPPOPTION) $(SOURCEFILE)

# *** EOF *************************************************


