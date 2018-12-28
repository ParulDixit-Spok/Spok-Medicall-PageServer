// IniFileReadWrite.h
#include <atlstr.h>

CString GetIniString(const CString& Section, const CString& Key, const CString& Default, const CString& PathAndFilename);
bool GetIniBool(const CString& Section, const CString& Key, const CString& Default, const CString& PathAndFilename);

