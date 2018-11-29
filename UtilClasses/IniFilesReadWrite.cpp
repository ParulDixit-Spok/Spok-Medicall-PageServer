// IniFilesReadWrite.cpp

// disable deprecation
#pragma warning(disable:4996)


#include "IniFilesReadWrite.h"
//#include "afx.h"


//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif



	// all parameters in and out are CString.
	//
CString GetIniString(const CString& Section, const CString& Key, const CString& Default, const CString& PathAndFilename)
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


bool GetIniBool(const CString& Section, const CString& Key, const CString& Default, const CString& PathAndFilename)
{
	bool rv = false;

	CString csTextBool = GetIniString(Section, Key, Default, PathAndFilename);
	csTextBool.MakeUpper();

	if(0 == csTextBool.Compare("TRUE"))
	{
		rv = true;
	}

	return rv;
}
