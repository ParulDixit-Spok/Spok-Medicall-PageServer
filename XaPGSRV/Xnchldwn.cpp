
// xnchldwn.cpp

#include "common.h"
#include <io.h>
// disable deprecation
#pragma warning(disable:4996)


	HWND
XnCreateChild(LPSTR lpName)
	{
    HWND hwnd;
	HWND hwndChild;
	int i;
	int ListBoxSize;
    CHAR sz[160];
    MDICREATESTRUCT mcs;

    if(!lpName)
		{
        /* The pName parameter is NULL -- load the "Untitled" string from */
        /* STRINGTABLE and set the title field of the MDI CreateStruct.    */
        LoadString (hInst, IDS_UNTITLED, sz, sizeof(sz));
        mcs.szTitle = (LPSTR) sz;
		}
    else
		{
        /* Title the window with the fully qualified pathname obtained by
         * calling OpenFile() with the OF_PARSE flag (in function
         * AlreadyOpen(), which is called before XnCreateChild().
         */
        mcs.szTitle = lpName;
		}

    mcs.szClass = szChild;
    mcs.hOwner  = hInst;

    /* Use the default size for the window */
    mcs.x = CW_USEDEFAULT;
	mcs.cx = CW_USEDEFAULT;
    mcs.y = CW_USEDEFAULT;
	mcs.cy = CW_USEDEFAULT;

    /* Set the style DWORD of the window to default */
    mcs.style = CS_NOCLOSE;

    /* tell the MDI Client to create the child */
    hwndChild = (HWND) SendMessage(hwndMDIClient,
                              WM_MDICREATE,
                              0,
                              (LONG) (LPMDICREATESTRUCT)&mcs);

	hwnd = (HWND) GetWindowLong(hwndChild, GWL_HWNDEDIT);

	// select fixed pitch font in test mode(s).
	if(bCmdLineUseAlternateMenu || bCmdLineUseProgrammerMenu)
		{
		SendMessage(hwnd, WM_SETFONT, (WPARAM) GetStockObject(SYSTEM_FIXED_FONT), TRUE);
		}

	// list box size is a development option,
	//  this is feature is attached to the alternate menu option.
	if(bCmdLineUseAlternateMenu || bCmdLineUseProgrammerMenu)
		{
		ListBoxSize = 1000;
		}
	else
		{
		ListBoxSize = 100;
		}

	// disable re-draw temporarily.
	SendMessage(hwnd, WM_SETREDRAW, (WPARAM) FALSE, (LPARAM) 0);

	// do the screen update.

	for(i = 0; i < ListBoxSize; i++)
		{
		SendMessage(hwnd, LB_ADDSTRING, (WPARAM) 0, (LPARAM) (LPSTR) " ");
		}

	// allow re-draw now.
	SendMessage(hwnd, WM_SETREDRAW, (WPARAM) TRUE, (LPARAM) 0);

    return hwnd;
	}


	HWND
XnCreateStatus(LPSTR lpName, int x, int y, int cx, int cy)
	{
    HWND hwnd;
	HWND hwndChild;
    MDICREATESTRUCT mcs;

    mcs.szTitle = lpName;
    mcs.szClass = szStatus;
    mcs.hOwner  = hInst;

    mcs.x = x;
	mcs.cx = cx;
	mcs.y = y;
	mcs.cy = cy;

    // Set the style DWORD of the window to default
    mcs.style = CS_NOCLOSE;

    // tell the MDI Client to create the child
    hwndChild = (HWND) SendMessage(hwndMDIClient,
                              WM_MDICREATE,
                              0,
                              (LONG) (LPMDICREATESTRUCT) &mcs);

	hwnd = (HWND) GetWindowLong(hwndChild, GWL_HWNDEDIT);

	SendMessage(hwnd, WM_SETFONT, (WPARAM) GetStockObject(SYSTEM_FIXED_FONT), TRUE);

    return hwnd;
	}


	void
Log_Message(int fileHandle, unsigned long * msgPosition, char * message)
	{
    int i;
    char string[8192 + 3];
	int CharsToCopy;

    if(fileHandle <= 0)
		return;

	if(strlen(message) >= ((sizeof(string) - 1)- 2))
		{
		CharsToCopy = (sizeof(string) - 3);
		}
	else
		{
		CharsToCopy = strlen(message);
		}

	strncpy(string, message, CharsToCopy);

	string[CharsToCopy + 0] = 0x0d;
	string[CharsToCopy + 1] = 0x0a;
	string[CharsToCopy + 2] = 0x00;

	// ensure properly terminated string
	string[sizeof(string) - 1] = 0;

    if(_lseek(fileHandle,  *msgPosition + 4L, SEEK_SET ) == -1)
		return;

    i = _write(fileHandle, string, strlen(string));

    if(i != -1)
		*msgPosition = *msgPosition + i;

    if(_lseek( fileHandle,  0L, SEEK_SET ) == -1)
		return;

    i = _write(fileHandle, (char *) &*msgPosition, 4);

	// see if maximum size of log file has been exceeded, then wrap.
	//  was 425,000, now 1,000,000 bytes
    if(*msgPosition > 1000000)
		*msgPosition = 0;
	}

