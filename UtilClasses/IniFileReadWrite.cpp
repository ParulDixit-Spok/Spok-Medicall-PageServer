
// IniFilesReadWrite.cpp


#include "afx.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



	// all parameters in and out are CString.
	//
	CString
GetIniString(CString Section, CString Key, CString Default, CString PathAndFilename)
	{
	CString ReturnValue = Default;
	char Buffer[256];

	if(GetPrivateProfileString(	(LPCTSTR) Section, 
								(LPCTSTR) Key, 
								NULL, 
								Buffer, 
								sizeof(Buffer), 
								(LPCTSTR) PathAndFilename) > 0)
		{
		ReturnValue = Buffer;
		}

	return ReturnValue;
	}

