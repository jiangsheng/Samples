# Microsoft Developer Studio Generated NMAKE File, Based on TrapKeys.dsp
!IF "$(CFG)" == ""
CFG=TrapKeys - Win32 Debug
!MESSAGE No configuration specified. Defaulting to TrapKeys - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "TrapKeys - Win32 Release" && "$(CFG)" != "TrapKeys - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "TrapKeys.mak" CFG="TrapKeys - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "TrapKeys - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "TrapKeys - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

!IF  "$(CFG)" == "TrapKeys - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\TrapKeys.exe"


CLEAN :
	-@erase "$(INTDIR)\StatLink.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TaskKeyMgr.obj"
	-@erase "$(INTDIR)\TrapKeys.obj"
	-@erase "$(INTDIR)\TrapKeys.pch"
	-@erase "$(INTDIR)\TrapKeys.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\TrapKeys.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\TrapKeys.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\TrapKeys.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TrapKeys.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=Release\TaskKeyHook.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\TrapKeys.pdb" /machine:I386 /out:"$(OUTDIR)\TrapKeys.exe" 
LINK32_OBJS= \
	"$(INTDIR)\StatLink.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TaskKeyMgr.obj" \
	"$(INTDIR)\TrapKeys.obj" \
	"$(INTDIR)\TrapKeys.res"

"$(OUTDIR)\TrapKeys.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "TrapKeys - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\TrapKeys.exe"


CLEAN :
	-@erase "$(INTDIR)\StatLink.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\TaskKeyMgr.obj"
	-@erase "$(INTDIR)\TrapKeys.obj"
	-@erase "$(INTDIR)\TrapKeys.pch"
	-@erase "$(INTDIR)\TrapKeys.res"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\TrapKeys.exe"
	-@erase "$(OUTDIR)\TrapKeys.ilk"
	-@erase "$(OUTDIR)\TrapKeys.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP=cl.exe
CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\TrapKeys.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

MTL=midl.exe
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC=rc.exe
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\TrapKeys.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\TrapKeys.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=Debug\TaskKeyHook.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\TrapKeys.pdb" /debug /machine:I386 /out:"$(OUTDIR)\TrapKeys.exe"
LINK32_OBJS= \
	"$(INTDIR)\StatLink.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\TaskKeyMgr.obj" \
	"$(INTDIR)\TrapKeys.obj" \
	"$(INTDIR)\TrapKeys.res"

"$(OUTDIR)\TrapKeys.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("TrapKeys.dep")
!INCLUDE "TrapKeys.dep"
!ELSE 
!MESSAGE Warning: cannot find "TrapKeys.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "TrapKeys - Win32 Release" || "$(CFG)" == "TrapKeys - Win32 Debug"
SOURCE=.\StatLink.cpp

"$(INTDIR)\StatLink.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TrapKeys.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "TrapKeys - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\TrapKeys.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\TrapKeys.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "TrapKeys - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\TrapKeys.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\TrapKeys.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\TaskKeyMgr.cpp

"$(INTDIR)\TaskKeyMgr.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TrapKeys.pch"


SOURCE=.\TrapKeys.cpp

"$(INTDIR)\TrapKeys.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\TrapKeys.pch"


SOURCE=.\TrapKeys.rc

"$(INTDIR)\TrapKeys.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)



!ENDIF 

