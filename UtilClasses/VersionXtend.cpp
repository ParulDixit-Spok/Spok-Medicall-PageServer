// Version.cpp
#pragma comment(lib,"Version.lib")

// disable deprecation
#pragma warning(disable:4996)


//#include "afx.h"
#include <Windows.h>
#include <WinVer.h>
#include <stdio.h>
#include "VersionXtend.h"


	// query the version information resource, and
	//  build a text version string of the form:
	//  "3-16-0000", or "3-15-0002-A"
	//
	// szfileName is the .EXE file to examine.
	// szAppVersion is the output string.

void GetFileVersionString(char * szFileName, char * szAppVersion)
{
	//CString csVersionLetter;
	char verLetter[50];
	VS_FIXEDFILEINFO* psVI;
	char Buffer[2000];			// block size is approximately 1350 bytes
	//  static allocation of 2000 should be ok.
	unsigned int dwLength;
	BOOL success;


	success = ::GetFileVersionInfo(szFileName, NULL, sizeof(Buffer), Buffer);
	if(!success)
	{
		DWORD err = GetLastError();

		return;
	}

	success = ::VerQueryValue(Buffer,
		"\\",
		(void **) &psVI,
		&dwLength);

	if(!success) return;

	// if the least significant 16 bits of the file version
	//  information is in the range of 1 thru 26, present
	//  it as a letter 'A' thru 'Z' with preceeding '-'.
	int LastPartOfVersion = (psVI->dwFileVersionLS & 0xffff);

	if(LastPartOfVersion == 0)
	{
		// if zero, show nothing.
		verLetter[0] = 0;	
		//csVersionLetter.Empty();
	}
	else if(LastPartOfVersion <= 26)
	{
		// prepare "-A" thru "-Z" letter extension.
		verLetter[0] = (LastPartOfVersion + '@');
		verLetter[1] = 0;
		//csVersionLetter.Format("-%c", (LastPartOfVersion + '@'));
	}
	else
	{
		// just show it as 4 digit numeric information.
		//csVersionLetter.Format("-%04d", LastPartOfVersion);
		_itoa_s(LastPartOfVersion, verLetter, 50, 10);
	}

	sprintf(szAppVersion, 
		"%d-%02d-%04d%s", 
		(psVI->dwFileVersionMS >> 16) & 0xffff, 
		psVI->dwFileVersionMS & 0xffff, 
		(psVI->dwFileVersionLS >> 16) & 0xffff,
		verLetter);
}

