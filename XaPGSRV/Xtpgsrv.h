// xtpgsrv.h
#pragma once

#ifdef WIN32
#define GET_EM_SETSEL_MPS(iStart, iEnd) (UINT)(iStart), (LONG)(iEnd)
#define GET_WM_COMMAND_ID(wp, lp)       LOWORD(wp)
#define GET_WM_COMMAND_CMD(wp, lp)      HIWORD(wp)
#define GET_WM_COMMAND_HWND(wp, lp)     (HWND)(lp)
#define GET_WM_COMMAND_MPS(id, hwnd, cmd) (UINT)MAKELONG(id, cmd), (LONG)(hwnd)
#define GET_WM_MDIACTIVATE_FACTIVATE(hwnd, wp, lp)  (lp == (LONG)hwnd)
#else
#define GET_EM_SETSEL_MPS(iStart, iEnd) 0, MAKELONG(iStart, iEnd)
#define GET_WM_COMMAND_ID(wp, lp)       (wp)
#define GET_WM_COMMAND_CMD(wp, lp)      HIWORD(lp)
#define GET_WM_COMMAND_HWND(wp, lp)     (HWND)LOWORD(lp)
#define GET_WM_COMMAND_MPS(id, hwnd, cmd) (UINT)(id), MAKELONG(hwnd, cmd)
#define GET_EM_SETSEL_MPS(iStart, iEnd) 0, MAKELONG(iStart, iEnd)
#define GET_WM_MDIACTIVATE_FACTIVATE(hwnd, wp, lp)  (BOOL)(wp)
#endif


#define WINDOWMENU		2   // position of window menu 
#define MAX_BUFFER      512
#define MAXFILENAME     256

#ifdef RC_INVOKED
#define ID(id) id
#else
#define ID(id) MAKEINTRESOURCE(id)
#endif

// edit control identifier 
#define ID_EDIT 0xCAC

// Window word values for child windows 
#define GWL_HWNDEDIT    0
#define GWW_CHANGED     4
#define GWL_WORDWRAP    6
#define GWW_UNTITLED    10
#define CBWNDEXTRA      12

// menu ID's 
#define IDM_FILEEXIT    1006
#define IDM_FILEABOUT   1007
#define IDM_WINDOWTILE  4001
#define IDM_WINDOWCASCADE 4002
#define IDM_WINDOWICONS 4004

#define IDM_WINDOWCHILD 4100

#define IDM_HELPHELP    5001
#define IDM_HELPABOUT   5002
#define IDM_HELPSPOT    5003

#define IDD_FILEOPEN    ID(200)
#define IDD_FILENAME    201
#define IDD_FILES       202
#define IDD_PATH        203
#define IDD_DIRS        204

// dialog ids 
#define IDD_ABOUT       ID(300)

#define IDD_FIND        ID(400)
#define IDD_SEARCH      401
#define IDD_PREV        402
#define IDD_NEXT        IDOK
#define IDD_CASE        403

#define IDD_SAVEAS      ID(500)
#define IDD_SAVEFROM    501
#define IDD_SAVETO      502

#define IDD_PRINT       ID(600)
#define IDD_PRINTDEVICE 601
#define IDD_PRINTPORT   602
#define IDD_PRINTTITLE  603

#define IDD_FONT        ID(700)
#define IDD_FACES       701
#define IDD_SIZES       702
#define IDD_BOLD        703
#define IDD_ITALIC      704
#define IDD_FONTTITLE   705

// strings 
#define IDS_CANTOPEN    1
#define IDS_CANTREAD    2
#define IDS_CANTCREATE  3
#define IDS_CANTWRITE   4
#define IDS_ILLFNM      5
#define IDS_ADDEXT      6
#define IDS_CLOSESAVE   7
#define IDS_CANTFIND    8
#define IDS_HELPNOTAVAIL 9
#define IDS_HELPMENUTEXT 10

#define IDS_UNTITLED    17
#define IDS_APPNAME     18

#define IDS_PRINTJOB    24
#define IDS_PRINTERROR  25


//  externally declared functions

//extern BOOL InitializeApplication(VOID);
//extern BOOL InitializeInstance(INT);
//HWND XnCreateChild( LPSTR lpName);
//
//VOID CommandHandler(HWND, WPARAM, LPARAM);
//LONG CALLBACK   MPFrameWndProc(HWND, UINT, WPARAM, LPARAM);
//LONG CALLBACK   MPMDIChildWndProc( HWND, UINT, WPARAM, LPARAM );
//LONG CALLBACK   MPMDIStatusWndProc( HWND, UINT, WPARAM, LPARAM );

