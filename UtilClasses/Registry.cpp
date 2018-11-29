// disable deprecation
#pragma warning(disable:4996)


//registry.cpp

// this class lifted from [Templeman98]

//#include "afx.h"
#include "Registry.h"

#include <WinReg.h>


#include <string.h>
#include <tchar.h>
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
//
//#define DEBUG_NEW new


using namespace std;



//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif


Registry::Registry()
	{
	hkKey = NULL;
	hkHive = NULL;
	nKeyIndex = 0;
	nValIndex = 0;
	pKeyName = NULL;
	pValueName = NULL;
	}


Registry::~Registry()
	{
	// Close registry keys
	if (hkKey != NULL)
		RegCloseKey(hkKey);
	if (hkHive != NULL)
		RegCloseKey(hkHive);

	// Delete enumeration buffer if one has been assigned
	delete [] pKeyName;
	delete [] pValueName;
	}


	void
Registry::setHiveName()
	{
	if (hkHive == HKEY_LOCAL_MACHINE)
		_tcscpy(fullKeyName, _T("HKEY_LOCAL_MACHINE"));
	else if (hkHive == HKEY_USERS)
		_tcscpy(fullKeyName, _T("HKEY_USERS"));
	else if (hkHive == HKEY_CURRENT_USER)
		_tcscpy(fullKeyName, _T("HKEY_CURRENT_USER"));
	else if (hkHive == HKEY_CLASSES_ROOT)
		_tcscpy(fullKeyName, _T("HKEY_CLASSES_ROOT"));
	else if (hkHive == HKEY_CURRENT_CONFIG)
		_tcscpy(fullKeyName, _T("HKEY_CURRENT_CONFIG"));
	}


	bool
Registry::checkHiveName(LPCTSTR t, HKEY* pHive)
	{
	bool bOK = true;
	*pHive = NULL;

	if (!_tcscmp(t, "HKEY_CLASSES_ROOT"))
		*pHive = HKEY_CLASSES_ROOT;
	else if (!_tcscmp(t, "HKEY_LOCAL_MACHINE"))
		*pHive = HKEY_LOCAL_MACHINE;
	else if (!_tcscmp(t, "HKEY_USERS"))
		*pHive = HKEY_USERS;
	else if (!_tcscmp(t, "HKEY_CURRENT_USER"))
		*pHive = HKEY_CURRENT_USER;
	else if (!_tcscmp(t, "HKEY_CURRENT_CONFIG"))
		*pHive = HKEY_CURRENT_CONFIG;
	else
		bOK = false;

	return bOK;
	}


	bool
Registry::queryInfo()
	{
	TCHAR tc[REG_BUFF_SIZE+1];
	DWORD bufsize = sizeof tc;

	LONG err = RegQueryInfoKey(hkKey, tc, &bufsize,
				  static_cast<LPDWORD>(NULL), &numSubkeys, &maxSubkeyLength,
				  &maxClassLength, &numValues,
				  &maxValueNameLength, &maxValueDataLength,
				  &SDLength, &lastWriteTime);

	if (err == ERROR_SUCCESS)
		{
		_tcscpy(className, tc);   // save class name away
		SetLastError(ERROR_SUCCESS);
		return true;
		}
	else
		{
		SetLastError(err);
		return false;
		}
	}


	bool
Registry::openKeyRel(LPCTSTR pSubKey)
	{
	// Open a subkey relative to the current one
	// If we haven't got a key open, we can't open a subkey!
	if (hkKey == NULL)
		{
		SetLastError(ERROR_INVALID_PARAMETER);
		return false;
		}

	HKEY newKey;
	LONG err = RegOpenKeyEx(hkKey, pSubKey, NULL, KEY_ALL_ACCESS, &newKey);
	SetLastError(err);

	if (err == ERROR_SUCCESS)
		{
		// Close the old key and save the new one
		RegCloseKey(hkKey);
		hkKey = newKey;

		// Amend the path string
		_tcscat(fullKeyName, _T("\\"));
		_tcscat(fullKeyName, pSubKey);

		// Get key info
		queryInfo();
		}
	return err == ERROR_SUCCESS;
	}


	bool
Registry::createKeyRel(LPCTSTR pSubKey, LPCTSTR pClass)
	{
	// Create or open a key relative to the current one
	// If we haven't got a key open, we can't open a subkey!
	if (hkKey == NULL)
		{
		SetLastError(ERROR_INVALID_PARAMETER);
		return false;
		}

	HKEY newKey;
	DWORD dwDisp;
	LONG err = RegCreateKeyEx(hkKey, pSubKey, NULL, const_cast<LPTSTR>(pClass),
	REG_OPTION_NON_VOLATILE,      // non-volatile
	KEY_ALL_ACCESS, NULL,         // all access, no security
	&newKey,
	&dwDisp);                     // disposition

	SetLastError(err);

	if (err == ERROR_SUCCESS)
		{
		// Close the old key and save the new one
		RegCloseKey(hkKey);
		hkKey = newKey;

		// Amend the path string
		_tcscat(fullKeyName, _T("\\"));
		_tcscat(fullKeyName, pSubKey);


		// Get key info
		queryInfo();
		}

	return err == ERROR_SUCCESS;
	}


	bool
Registry::upOneLevel()
	{
	// Find the string between the hive and parent key
	LPTSTR pFirst = _tcschr(fullKeyName, '\\');
	LPTSTR pLast = _tcsrchr(fullKeyName, '\\');
	if (!pFirst || !pLast)
		{
		return false;
		}

	// If they're the same, then there's only one slash, so the parent
	// is the hive
	if (pFirst == pLast)
		{
		openHive(hkHive);
		}
	else
		{
		TCHAR t[REG_BUFF_SIZE+1];

		// Point to character after the first '\', and copy to buffer
		pFirst += 1;
		_tcscpy(t, pFirst);

		// Set the last '\' to null to terminate the path
		pLast = _tcsrchr(t, '\\');
		*pLast = '\0';

		// Close the old key
		HKEY newKey;
		LONG err = RegOpenKeyEx(hkHive, t, NULL, KEY_ALL_ACCESS, &newKey);
		SetLastError(err);

		if (err == ERROR_SUCCESS)
			{
			// Close the old key and save the new one
			RegCloseKey(hkKey);
			hkKey = newKey;

			// Amend the path string
			setHiveName();
			_tcscat(fullKeyName, _T("\\"));
			_tcscat(fullKeyName, t);

			// Get key info
			queryInfo();
			return true;
			}
		else
			{
			return false;
			}
		}
	return true;
	}


	bool
Registry::openHive(HKEY hk)
	{
	HKEY newKey;
	long err = RegOpenKeyEx(hk, NULL, NULL, NULL, &newKey);
	SetLastError(err);

	if (err == ERROR_SUCCESS)
		{
		// Close anything already open
		if (hkKey != NULL)
			RegCloseKey(hkKey);

		if (hkHive != NULL)
			RegCloseKey(hkHive);

		// Save the new keys
		hkHive = hk;
		hkKey = newKey;

		// Put the hive name into the path string, and get key information
		setHiveName();
		queryInfo();
		}

	return err == ERROR_SUCCESS;
	}


	bool
Registry::openKeyAbs(LPCTSTR pSubKey)
	{
	// Open a key, given the full path
	// We need to crack the hive off the front, and check it first
	TCHAR t[REG_BUFF_SIZE+1];

	// Check the string passed in isn't too long
	if(_tcslen(pSubKey) > REG_BUFF_SIZE)
		{
		SetLastError(ERROR_INVALID_PARAMETER);
		return false;
		}

	_tcscpy(t, pSubKey);

	LPTSTR pFirst = _tcschr(t, '\\');

	if(!pFirst)
		{
		SetLastError(ERROR_INVALID_PARAMETER);
		return false;
		}

	*pFirst = '\0';

	// Check the hive name and save as the current hive if OK
	HKEY hive;

	if(!checkHiveName(t, &hive))
		{
		SetLastError(ERROR_INVALID_PARAMETER);
		return false;
		}
	else
		{
		hkHive = hive;
		}

	// Point to the character after the null as the subkey
	LPTSTR pSub = pFirst+1;

	HKEY newKey;
	LONG err = ::RegOpenKeyEx(hive, pSub, NULL, KEY_ALL_ACCESS, &newKey);
	SetLastError(err);

	if(err == ERROR_SUCCESS)
		{
		// Close the old key and save the new one
		RegCloseKey(hkKey);
		hkKey = newKey;

		// Amend the path string
		_tcscpy(fullKeyName, pSubKey);

		// Get key info
		queryInfo();
		}

	return err == ERROR_SUCCESS;
	}


	bool
Registry::createKeyAbs(LPCTSTR pSubKey, LPTSTR pClass)
	{
	// Create or open a key from an absolute path
	// Open a key, given the full path
	// We need to crack the hive off the front, and check it first
	int nKeyLen = _tclen(pSubKey);


	//auto_ptr<LPTSTR> t =  
	LPTSTR t = new TCHAR(nKeyLen + 1);

	// Check the string passed in isn't too long
	if (_tcslen(pSubKey) > REG_BUFF_SIZE)
		{
		SetLastError(ERROR_INVALID_PARAMETER);
		return false;
		}

	_tcscpy(t, pSubKey);

	LPTSTR pFirst = _tcschr(t, '\\');
	if (!pFirst)
		{
		SetLastError(ERROR_INVALID_PARAMETER);
		return false;
		}

	*pFirst = '\0';

	// Check the hive name and save as the current hive if OK
	HKEY hive;

	if(!checkHiveName(t, &hive))
		{
		SetLastError(ERROR_INVALID_PARAMETER);
		return false;
		}
	else
		{
		hkHive = hive;
		}

	// Point to the character after the null as the subkey
	LPTSTR pSub = pFirst+1;

	HKEY newKey;
	DWORD dwDisp;
	LONG err = RegCreateKeyEx(hive, pSub, NULL, pClass,
	REG_OPTION_NON_VOLATILE,      // non-volatile
	KEY_ALL_ACCESS, NULL,         // all access, no security
	&newKey,
	&dwDisp);                     // disposition
	SetLastError(err);

	if(err == ERROR_SUCCESS)
		{
		// Close the old key and save the new one
		RegCloseKey(hkKey);
		hkKey = newKey;

		// Amend the path string
		_tcscpy(fullKeyName, pSubKey);

		// Get key info
		queryInfo();
		}

	return err == ERROR_SUCCESS;
	}


	bool
Registry::getBinaryValue(LPCTSTR pName, LPBYTE pData, LPDWORD pSize)
	{
	LONG err = RegQueryValueEx(hkKey,
								const_cast<LPTSTR>(pName),
								NULL,
								NULL,
								pData,
								pSize);
	SetLastError(err);
	return (err == ERROR_SUCCESS);
	}


	bool
Registry::getDWordValue(LPCTSTR pName, LPDWORD pData)
	{
	DWORD dwSize = sizeof(DWORD);
	LONG err = RegQueryValueEx(hkKey, const_cast<LPTSTR>(pName), NULL, NULL,
	  reinterpret_cast<LPBYTE>(pData), &dwSize);
	SetLastError(err);
	return (err == ERROR_SUCCESS);
	}


	bool
Registry::getStringValue(LPCTSTR pName, LPTSTR pData, LPDWORD pSize)
	{
	LONG err = RegQueryValueEx(hkKey, const_cast<LPTSTR>(pName), NULL, NULL,
	  reinterpret_cast<LPBYTE>(pData), pSize);
	SetLastError(err);
	return (err == ERROR_SUCCESS);
	}


	bool
Registry::setDWordValue(LPCTSTR pName, DWORD dwData)
	{
	LONG err = RegSetValueEx(hkKey, const_cast<LPTSTR>(pName), NULL, REG_DWORD,
	  reinterpret_cast<LPBYTE>(&dwData), sizeof(DWORD));
	SetLastError(err);
	return (err == ERROR_SUCCESS);
	}


	bool
Registry::setStringValue(LPCTSTR pName, LPTSTR pData, DWORD dwSize)
	{
	LONG err = RegSetValueEx(hkKey, const_cast<LPTSTR>(pName), NULL, REG_SZ,
	  reinterpret_cast<LPBYTE>(pData), dwSize);
	SetLastError(err);
	return (err == ERROR_SUCCESS);
	}


	bool
Registry::setBinaryValue(LPCTSTR pName, LPBYTE pData, DWORD dwSize)
	{
	LONG err = RegSetValueEx(hkKey, pName, NULL, REG_BINARY, pData, dwSize);
	SetLastError(err);
	return (err == ERROR_SUCCESS);
	}


	bool
Registry::deleteValue(LPCTSTR pName)
	{
	// Delete a value associated with a key
	LONG err = RegDeleteValue(hkKey, pName);

	SetLastError(err);
	return (err == ERROR_SUCCESS);
	}


	LPCTSTR
Registry::enumKeys(bool bReset)
	{
	// Enumerate all the subkeys for the current key, returning a
	// pointer to the name string. Return null when no more.

	if(bReset)
		nKeyIndex = 0;

	// Query the current key to find the maximum subkey size
	queryInfo();

	// Allocate a buffer to hold largest subkey name
	delete [] pKeyName;

	pKeyName = new TCHAR[maxSubkeyLength+1];
	DWORD bufsize = maxSubkeyLength+1;
	FILETIME lastWriteTime;

	// get the subkey info... don't worry about class name or file time
	LONG err = RegEnumKeyEx(hkKey, nKeyIndex++,
	pKeyName, &bufsize, NULL, NULL, NULL, &lastWriteTime);
	SetLastError(err);

	if(err == ERROR_SUCCESS)
		return pKeyName;
	else
		return NULL;
	}


	LPCTSTR
Registry::enumValues(LPDWORD pdwType, LPBYTE pData, LPDWORD pSize, bool bReset)
	{
	// Enumerate all the values for the current key, returning a
	// pointer to the name string. Return null when no more.

	if(bReset)
		nValIndex = 0;

	// Query the current key to find the maximum value size
	queryInfo();

	// Allocate a buffer to hold largest value name
	delete [] pValueName;

	pValueName = new TCHAR[maxValueNameLength+1];

	// get the value info...
	DWORD bufsize = maxValueNameLength+1;
	LONG err = RegEnumValue(hkKey, nValIndex++,
	pValueName, &bufsize, NULL, pdwType, pData, pSize);
	SetLastError(err);

	if (err == ERROR_SUCCESS)
		return pValueName;
	else
		return NULL;
	}


	bool
Registry::deleteKey(LPCTSTR pName)
	{
	// Delete a key and all its values
	// NOTE: on Win95 this will delete a key and all its subkeys.
	// On WinNT it will only delete a key without subkeys
	LONG err = RegDeleteKey(hkKey, pName);

	SetLastError(err);
	return err == ERROR_SUCCESS;
	}


	BOOL
AdjustPrivilege(LPTSTR pPriv, BOOL bSet)
	{
	HANDLE token;

	if(!OpenProcessToken(GetCurrentProcess(),
	  TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token))
		{
		// cout << "Error in SetPrivilege (" << GetLastError() << ")" << endl;
		return FALSE;
		}

	LUID lu;
	if(!LookupPrivilegeValue(0, pPriv, &lu))
		{
		// cout << "Error in LookupPrivilegeValue (" << GetLastError() << ")" << endl;
		return FALSE;
		}

	TOKEN_PRIVILEGES tp;
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = lu;

	// Set or revoke privilege depending on flag
	tp.Privileges[0].Attributes = (bSet) ? SE_PRIVILEGE_ENABLED : 0;

	if(!AdjustTokenPrivileges(token, FALSE, &tp, 0, 0, 0))
		{
		// cout << "Cannot adjust token privilege (" << GetLastError() << ")" << endl;
		return FALSE;
		}

	return TRUE;
	}


	bool
Registry::saveKey(LPCTSTR pFile, HKEY hk)
	{
	// Save a key to a file. If no key is specified, the current one
	// is saved

	if(!pFile)
		{
		SetLastError(ERROR_INVALID_PARAMETER);
		return false;
		}

	HKEY hkSave = (hk == NULL) ? hkKey : hk;

	// Adjust the privilege so we'll be able to save
	if (!AdjustPrivilege(SE_BACKUP_NAME, TRUE))
		{
		SetLastError(ERROR_PRIVILEGE_NOT_HELD);
		return false;
		}

	// This version ignores security. Do the save, then adjust the privilege back
	LONG err = RegSaveKey(hkSave, pFile, NULL);
	SetLastError(err);

	AdjustPrivilege(SE_BACKUP_NAME, FALSE);

	return err == ERROR_SUCCESS;
	}


	bool
Registry::loadKey(LPCTSTR pSubkey, LPCTSTR pFile, HKEY hk)
	{
	// Load key data from a file. If hk is null, it will be created
	// under the current key

	if (!pFile || !pSubkey)
		{
		SetLastError(ERROR_INVALID_PARAMETER);
		return false;
		}

	HKEY hkLoad = (hk == NULL) ? hkKey : hk;

	// Adjust the privilege so we'll be able to load
	if (!AdjustPrivilege(SE_RESTORE_NAME, TRUE))
		{
		SetLastError(ERROR_PRIVILEGE_NOT_HELD);
		return false;
		}

	// This version ignores security. Do the load, then adjust the privilege back
	LONG err = RegLoadKey(hkLoad, pSubkey, pFile);
	SetLastError(err);

	AdjustPrivilege(SE_RESTORE_NAME, FALSE);

	return err == ERROR_SUCCESS;
	}
