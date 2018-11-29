# Microsoft Developer Studio Project File - Name="XaPGSRV" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=XaPGSRV - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XaPGSRV.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XaPGSRV.mak" CFG="XaPGSRV - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XaPGSRV - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "XaPGSRV - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/PageServer/XaPGSRV/XaPGSRV", AHACAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XaPGSRV - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /Zp1 /MD /W3 /GX /Od /D "WIN32" /D "_WINDOWS" /D "BTI_WIN_32" /D "_AFXDLL" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 mfc42.lib mfcs42.lib Nafxcw.lib Comctl32.lib Version.lib libdtimt.lib libdxxmt.lib libsrlmt.lib sctools.lib libfaxmt.lib utilclasses.lib /nologo /subsystem:windows /map /machine:I386 /nodefaultlib:"Nafxcw.lib" /nodefaultlib:"mfc42.lib" /nodefaultlib:"mfcs42.lib" /nodefaultlib:"Libcmt.lib" /nodefaultlib:"libc.lib" /libpath:"C:\project\PAGEWIN5\UtilClasses\Release"
# SUBTRACT LINK32 /incremental:yes

!ELSEIF  "$(CFG)" == "XaPGSRV - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /Zp1 /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "BTI_WIN_32" /D "_AFXDLL" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 mfc42.lib mfcs42.lib mfc42d.lib mfcs42d.lib Nafxcw.lib Comctl32.lib Version.lib libdtimt.lib libdxxmt.lib libsrlmt.lib sctools.lib libfaxmt.lib utilclasses.lib /nologo /subsystem:windows /map /debug /machine:I386 /nodefaultlib:"Nafxcw.lib" /nodefaultlib:"mfc42.lib" /nodefaultlib:"mfcs42.lib" /nodefaultlib:"Libcmt.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libc.lib" /nodefaultlib:"mfc42d.lib" /nodefaultlib:"mfcs42d.lib" /pdbtype:sept /libpath:"C:\project\PAGEWIN5\UtilClasses\Debug"
# SUBTRACT LINK32 /incremental:no /nodefaultlib

!ENDIF 

# Begin Target

# Name "XaPGSRV - Win32 Release"
# Name "XaPGSRV - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DataBaseAccessor.cpp
# End Source File
# Begin Source File

SOURCE=.\Globals.cpp
# End Source File
# Begin Source File

SOURCE=.\Impairment.cpp
# End Source File
# Begin Source File

SOURCE=.\List2.cpp
# End Source File
# Begin Source File

SOURCE=.\Mpinit.cpp
# End Source File
# Begin Source File

SOURCE=.\QueueBufferClass.cpp
# End Source File
# Begin Source File

SOURCE=.\QueueFileManager.cpp
# End Source File
# Begin Source File

SOURCE=.\Registry43.cpp
# End Source File
# Begin Source File

SOURCE=.\SpoolCollection.cpp
# End Source File
# Begin Source File

SOURCE=.\SpoolUtil.cpp
# End Source File
# Begin Source File

SOURCE=.\Stub1.cpp
# End Source File
# Begin Source File

SOURCE=.\Stub2.cpp
# End Source File
# Begin Source File

SOURCE=.\Transaction.cpp
# End Source File
# Begin Source File

SOURCE=.\Util.cpp
# End Source File
# Begin Source File

SOURCE=.\Util2.cpp
# End Source File
# Begin Source File

SOURCE=.\Util43.cpp
# End Source File
# Begin Source File

SOURCE=.\UtilXA.cpp
# End Source File
# Begin Source File

SOURCE=.\Xnchldwn.cpp
# End Source File
# Begin Source File

SOURCE=.\Xncom.cpp
# End Source File
# Begin Source File

SOURCE=.\Xnd4x.cpp
# End Source File
# Begin Source File

SOURCE=.\Xnfcnts.cpp
# End Source File
# Begin Source File

SOURCE=.\Xnpgado.cpp
# End Source File
# Begin Source File

SOURCE=.\Xnpgchk.cpp
# End Source File
# Begin Source File

SOURCE=.\Xnpgcom.cpp
# End Source File
# Begin Source File

SOURCE=.\Xnpgd4x.cpp
# End Source File
# Begin Source File

SOURCE=.\Xnpgd4xb.cpp
# End Source File
# Begin Source File

SOURCE=.\Xnpginit.cpp
# End Source File
# Begin Source File

SOURCE=.\Xnpglst.cpp
# End Source File
# Begin Source File

SOURCE=.\Xnpgport.cpp
# End Source File
# Begin Source File

SOURCE=.\Xnpgque.cpp
# End Source File
# Begin Source File

SOURCE=.\Xnschado.cpp
# End Source File
# Begin Source File

SOURCE=.\Xtpgsrv.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Common.h
# End Source File
# Begin Source File

SOURCE=.\DataBaseAccessor.h
# End Source File
# Begin Source File

SOURCE=.\Defines.h
# End Source File
# Begin Source File

SOURCE=.\DefinesS.h
# End Source File
# Begin Source File

SOURCE=.\Enums.h
# End Source File
# Begin Source File

SOURCE=.\EnumsS.h
# End Source File
# Begin Source File

SOURCE=.\Globals.h
# End Source File
# Begin Source File

SOURCE=.\icstring.h
# End Source File
# Begin Source File

SOURCE=.\Impairment.h
# End Source File
# Begin Source File

SOURCE=.\List2.h
# End Source File
# Begin Source File

SOURCE=.\QueueBufferClass.h
# End Source File
# Begin Source File

SOURCE=.\QueueFileManager.h
# End Source File
# Begin Source File

SOURCE=.\Registry43.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\SpoolCollection.h
# End Source File
# Begin Source File

SOURCE=.\SpoolUtil.h
# End Source File
# Begin Source File

SOURCE=.\Structs.h
# End Source File
# Begin Source File

SOURCE=.\Structs43.h
# End Source File
# Begin Source File

SOURCE=.\StructsS.h
# End Source File
# Begin Source File

SOURCE=.\Transaction.h
# End Source File
# Begin Source File

SOURCE=.\Xtpgsrv.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\Delete.bmp
# End Source File
# Begin Source File

SOURCE=.\res\New.bmp
# End Source File
# Begin Source File

SOURCE=.\res\note300.ico
# End Source File
# Begin Source File

SOURCE=.\res\note301.ico
# End Source File
# Begin Source File

SOURCE=.\res\Openfold.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PageCfg.ico
# End Source File
# Begin Source File

SOURCE=.\res\PageSrv.ico
# End Source File
# Begin Source File

SOURCE=.\res\pgmnbmp.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Save.bmp
# End Source File
# Begin Source File

SOURCE=.\res\STA1.ICO
# End Source File
# Begin Source File

SOURCE=.\res\STA2.ICO
# End Source File
# Begin Source File

SOURCE=.\res\STA3.ICO
# End Source File
# Begin Source File

SOURCE=.\res\STA4.ICO
# End Source File
# Begin Source File

SOURCE=.\Xtpgsrv.rc
# End Source File
# End Group
# End Target
# End Project
