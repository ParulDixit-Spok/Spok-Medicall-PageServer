

// mpinit.cpp


#include "common.h"
#include "Xnchldwn.h"

#include <Commctrl.h>


// disable deprecation
#pragma warning(disable:4996)


extern LONG CALLBACK   MPFrameWndProc(HWND, UINT, WPARAM, LPARAM);
extern LONG CALLBACK   MPMDIChildWndProc( HWND, UINT, WPARAM, LPARAM );
extern LONG CALLBACK   MPMDIStatusWndProc( HWND, UINT, WPARAM, LPARAM );


CHAR szChild[] = "mpchild";			// Class name for MDI window
CHAR szStatus[] = "XNPGSTATUS";		// Class name for status window


	BOOL
InitializeApplication(void)
	{
    WNDCLASSEX wc;

    // Register the frame class
	wc.cbSize = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = (WNDPROC) MPFrameWndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 4;
    wc.hInstance	 = hInst;
    wc.hIcon         = LoadIcon(hInst, MAKEINTRESOURCE(IDI_PAGESRV));
    wc.hCursor       = LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground = (HBRUSH) (COLOR_APPWORKSPACE + 1);

	if(bCmdLineUseProgrammerMenu)
		{
		wc.lpszMenuName  = MAKEINTRESOURCE(IDR_PROG_MENU);
		}
	else if(bCmdLineUseAlternateMenu)
		{
		wc.lpszMenuName  = MAKEINTRESOURCE(IDR_ALT_MENU);
		}
	else
		{
		wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MAIN_MENU);
		}

    wc.lpszClassName = szAppName;
	wc.hIconSm		 = LoadIcon(hInst, MAKEINTRESOURCE(IDI_PAGESRV));

    if (!RegisterClassEx (&wc) )
		{
		return FALSE;
		}

    // Register the MDI child class
    wc.lpfnWndProc   = (WNDPROC) MPMDIChildWndProc;
    wc.lpszMenuName  = NULL;
    wc.cbWndExtra    = CBWNDEXTRA;
    wc.lpszClassName = szChild;
	wc.hIconSm		 = LoadIcon(hInst, MAKEINTRESOURCE(IDI_BLUE));

    if (!RegisterClassEx(&wc))
		{
		return FALSE;
		}

    //		Register the MDI status class
	wc.lpfnWndProc   = (WNDPROC) MPMDIStatusWndProc;
    wc.lpszMenuName  = NULL;
    wc.cbWndExtra    = CBWNDEXTRA;
    wc.lpszClassName = szStatus;
	wc.hIconSm		 = LoadIcon(hInst, MAKEINTRESOURCE(IDI_PURPLE));

    if (!RegisterClassEx(&wc))
		{
		return FALSE;
		}

    return TRUE;
	}


	BOOL
InitializeInstance(INT nCmdShow)
	{
	int	cxPosition;

    // Create the frame
    hwndFrame = CreateWindow(szAppName,
							szAppName,
							WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
							CW_USEDEFAULT,
							0,
							CW_USEDEFAULT,
							0,
							NULL,
							NULL,
							hInst,
							NULL);

    if ((!hwndFrame) || (!hwndMDIClient))
		{
		return FALSE;
		}

	//  Create Status Bar
	hStatusWindow =
		CreateStatusWindow(WS_CHILD | WS_VISIBLE | CCS_BOTTOM | SBARS_SIZEGRIP,
							NULL,	// no text in status window to start.
							hwndFrame,
							101);

    /* Display the frame window */
    ShowWindow (hwndFrame, nCmdShow);
    UpdateWindow (hwndFrame);

	cxPosition = GetSystemMetrics(SM_CXFULLSCREEN) - 160;

    /* Add the first MDI window */
    hwndGeneralMsg = XnCreateChild( "General Messges" );
    hwndD4xMsg	   = XnCreateChild( "Phone Line Messges" );
    hwndComMsg	   = XnCreateChild( "Comm Port Messges" );
    hwndDatabaseMsg	   = XnCreateChild( "Database Messges" );
	
	/*generalBuffer.AssignDocument(hwndGeneralMsg);
	phoneBuffer.AssignDocument(hwndD4xMsg);
	commBuffer.AssignDocument(hwndComMsg);*/

    //SO - 03/27/2013 - Jim H and I changed this to be pointer to have control over memory allocation.
    //This fixes crash during exit.
    LogBuffer *dataBaseBuffer = new LogBuffer(NULL);
	dataBaseBuffer->AssignDocument(hwndDatabaseMsg);
    dbLogStream = new ostream(dataBaseBuffer);

	SendMessage (hwndMDIClient, WM_MDITILE, MDITILE_HORIZONTAL, 0L);
	ShowWindow (hwndFrame, SW_MAXIMIZE );

	hwndComStatus  = XnCreateStatus("Comm Port Status     ",
									cxPosition - 80 - 20,	// x position
									100 + 50,				// y position
									200,					// width
									300);					// height

	hwndD4xStatus  = XnCreateStatus("Phone Line Status    ",
									cxPosition - 80 - 40,
									100 + 100,
									200,
									300);

	hwndQueStatus  = XnCreateStatus("Queue Status         ",
									cxPosition - 80 - 60,
									100 + 150,
									200,
									300);
	// add items to the window.
	{
	int L;
	CString csTemp;

	// "Queue Status" Window, has 3 header lines.
	csTemp.Format(" ");
	for(L = 0; L < (MAX_QUEUE_FILES + 3); L++)
		{
		// add one line item.
		SendMessage(hwndQueStatus, LB_ADDSTRING, (WPARAM) L, (LPARAM) (LPSTR) (LPCTSTR) csTemp);
		}

	// "IVR Port Status" Window.
	for(L = 0; L < (MAX_IVRPORTS + 3); L++)
		{
		// add one line item.
		SendMessage(hwndD4xStatus, LB_ADDSTRING, (WPARAM) L, (LPARAM) (LPSTR) (LPCTSTR) csTemp);
		}

	// "Comm Port Status" Window.
	for(L = 0; L < (MAX_COMMPORTS + 3); L++)
		{
		// add one line item.
		SendMessage(hwndComStatus, LB_ADDSTRING, (WPARAM) L, (LPARAM) (LPSTR) (LPCTSTR) csTemp);
		}
	}

    return TRUE;
	}


	void
XnSaveWindowPosition(HWND hWindow, char * winName)
	{
	char tempStr[80];
	char tempStr2[20];
	char fileName[80];
	WINDOWPLACEMENT	wp;

	memset(fileName, 0, sizeof(fileName));
	sprintf(fileName, "%s\\XNPGSRV.INI", param.BkupDir);

	if( GetWindowPlacement((HWND)GetWindowLong( hWindow, GWL_HWNDPARENT), &wp) != 0 )
		{
		memset( tempStr, 0, sizeof( tempStr ) );
		sprintf( tempStr, "%s_TOP", winName );
		memset( tempStr2, 0, sizeof( tempStr2 ) );
		sprintf( tempStr2, "%d", wp.rcNormalPosition.top );
		WritePrivateProfileString( (LPSTR)"WINDOWS", tempStr, tempStr2, fileName );

		memset( tempStr, 0, sizeof( tempStr ) );
		sprintf( tempStr, "%s_LEFT", winName );
		memset( tempStr2, 0, sizeof( tempStr2 ) );
		sprintf( tempStr2, "%d", wp.rcNormalPosition.left );
		WritePrivateProfileString( (LPSTR)"WINDOWS", tempStr, tempStr2, fileName );

		memset( tempStr, 0, sizeof( tempStr ) );
		sprintf( tempStr, "%s_RIGHT", winName );
		memset( tempStr2, 0, sizeof( tempStr2 ) );
		sprintf( tempStr2, "%d", wp.rcNormalPosition.right );
		WritePrivateProfileString( (LPSTR)"WINDOWS", tempStr, tempStr2, fileName );

		memset( tempStr, 0, sizeof( tempStr ) );
		sprintf( tempStr, "%s_BOTTOM", winName );
		memset( tempStr2, 0, sizeof( tempStr2 ) );
		sprintf( tempStr2, "%d", wp.rcNormalPosition.bottom );
		WritePrivateProfileString( (LPSTR)"WINDOWS", tempStr, tempStr2, fileName );

		memset( tempStr, 0, sizeof( tempStr ) );
		sprintf( tempStr, "%s_MINX", winName );
		memset( tempStr2, 0, sizeof( tempStr2 ) );
		sprintf( tempStr2, "%d", wp.ptMinPosition.x );
		WritePrivateProfileString( (LPSTR)"WINDOWS", tempStr, tempStr2, fileName );

		memset( tempStr, 0, sizeof( tempStr ) );
		sprintf( tempStr, "%s_MINY", winName );
		memset( tempStr2, 0, sizeof( tempStr2 ) );
		sprintf( tempStr2, "%d", wp.ptMinPosition.y );
		WritePrivateProfileString( (LPSTR)"WINDOWS", tempStr, tempStr2, fileName );

		memset( tempStr, 0, sizeof( tempStr ) );
		sprintf( tempStr, "%s_MAXX", winName );
		memset( tempStr2, 0, sizeof( tempStr2 ) );
		sprintf( tempStr2, "%d", wp.ptMaxPosition.x );
		WritePrivateProfileString( (LPSTR)"WINDOWS", tempStr, tempStr2, fileName );

		memset( tempStr, 0, sizeof( tempStr ) );
		sprintf( tempStr, "%s_MAXY", winName );
		memset( tempStr2, 0, sizeof( tempStr2 ) );
		sprintf( tempStr2, "%d", wp.ptMaxPosition.y );
		WritePrivateProfileString( (LPSTR)"WINDOWS", tempStr, tempStr2, fileName );
		}
	}


	void
XnRestoreWindowPosition(HWND hWindow, const char * iniKeyName)
	{
	int		top;
	int		left;
	int		right;
	int		bottom;
	int		minX;
	int		minY;
	int		maxX;
	int		maxY;
	char	tempStr[80];
	char	fileName[80];
	WINDOWPLACEMENT wp;

	memset( fileName, 0, sizeof( fileName ) );
	sprintf( fileName, "%s\\XNPGSRV.INI", param.BkupDir );

	memset( tempStr, 0, sizeof( tempStr ) );
	sprintf( tempStr, "%s_TOP", iniKeyName );
	top = GetPrivateProfileInt( (LPSTR)"WINDOWS", tempStr, -1, fileName );

	memset( tempStr, 0, sizeof( tempStr ) );
	sprintf( tempStr, "%s_LEFT", iniKeyName );
	left = GetPrivateProfileInt( (LPSTR)"WINDOWS", tempStr, -1, fileName );

	memset( tempStr, 0, sizeof( tempStr ) );
	sprintf( tempStr, "%s_RIGHT", iniKeyName );
	right = GetPrivateProfileInt( (LPSTR)"WINDOWS", tempStr, -1, fileName );

	memset( tempStr, 0, sizeof( tempStr ) );
	sprintf( tempStr, "%s_BOTTOM", iniKeyName );
	bottom = GetPrivateProfileInt( (LPSTR)"WINDOWS", tempStr, -1, fileName );

	memset( tempStr, 0, sizeof( tempStr ) );
	sprintf( tempStr, "%s_MINX", iniKeyName );
	minX = GetPrivateProfileInt( (LPSTR)"WINDOWS", tempStr, 0, fileName );

	memset( tempStr, 0, sizeof( tempStr ) );
	sprintf( tempStr, "%s_MINY", iniKeyName );
	minY = GetPrivateProfileInt( (LPSTR)"WINDOWS", tempStr, 0, fileName );

	memset( tempStr, 0, sizeof( tempStr ) );
	sprintf( tempStr, "%s_MAXX", iniKeyName );
	maxX = GetPrivateProfileInt( (LPSTR)"WINDOWS", tempStr, 0, fileName );

	memset( tempStr, 0, sizeof( tempStr ) );
	sprintf( tempStr, "%s_MAXY", iniKeyName );
	maxY = GetPrivateProfileInt( (LPSTR)"WINDOWS", tempStr, 0, fileName );

	if( top>-1 && left>-1 && right>-1 && bottom>-1 )
		{
		wp.rcNormalPosition.left = left;
		wp.rcNormalPosition.top = top;
		wp.rcNormalPosition.right = right;
		wp.rcNormalPosition.bottom = bottom;
		wp.ptMinPosition.x = minX;
		wp.ptMinPosition.y = minY;
		wp.ptMaxPosition.x = maxX;
		wp.ptMaxPosition.y = maxY;

		wp.showCmd = SW_SHOW;
		wp.length = sizeof(WINDOWPLACEMENT);
		wp.flags = WPF_SETMINPOSITION;
		SetWindowPlacement( (HWND)GetWindowLong( hWindow, GWL_HWNDPARENT), &wp);
		}
	}

