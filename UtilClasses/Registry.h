
//registry.h


#ifndef REGISTRY_H
#define REGISTRY_H

#include <Windows.h>

// Size of string buffers used in the class
#define REG_BUFF_SIZE 255


	class 
Registry
	{
	private:
		HKEY hkHive;    // hive we're using
		HKEY hkKey;     // current key

		TCHAR fullKeyName[REG_BUFF_SIZE+1];   // full path to key
		TCHAR className[REG_BUFF_SIZE+1];     // class name for key
		LPTSTR pKeyName;                      // buffer used in enumerating keys
		LPTSTR pValueName;                    // buffer used in enumerating values

		int nKeyIndex;     // subkey enumeration index
		int nValIndex;     // value enumeration index

		// QueryInfo data
		DWORD numSubkeys;
		DWORD maxSubkeyLength;
		DWORD maxClassLength;
		DWORD numValues;
		DWORD maxValueNameLength;
		DWORD maxValueDataLength;
		DWORD SDLength;
		FILETIME lastWriteTime;

		void setHiveName();
		bool checkHiveName(LPCTSTR t, HKEY* pHive);
		bool queryInfo();

	public:
		Registry();
		~Registry();

		// Return the string representing the path to the current key
		LPCTSTR getKeyPath() { return fullKeyName; }

		// Open a hive
		bool openHive(HKEY hk=HKEY_LOCAL_MACHINE);

		// Open a subkey relative to the current one
		bool openKeyRel(LPCTSTR pKey);

		// Open a key from an absolute path
		bool openKeyAbs(LPCTSTR pKey);

		// Move back up a level
		bool upOneLevel();

		// Create or open a subkey relative to the current one
		bool createKeyRel(LPCTSTR pKey, LPCTSTR pClass=NULL);

		// Create or open a key from an absolute path
		bool createKeyAbs(LPCTSTR pKey, LPTSTR pClass=NULL);

		// Enumerate subkeys
		LPCTSTR enumKeys(bool bReset=false);

		// Enumerate values
		LPCTSTR enumValues(LPDWORD pdwType, 
							LPBYTE pData=NULL, 
							LPDWORD pSize=NULL,
							bool bReset=false);

		// Get values
		bool getBinaryValue(LPCTSTR pName, LPBYTE pData, LPDWORD pSize);
		bool getDWordValue(LPCTSTR pName, LPDWORD pData);
		bool getStringValue(LPCTSTR pName, LPTSTR pData, LPDWORD pSize);

		// Set values
		bool setBinaryValue(LPCTSTR pName, LPBYTE pData, DWORD dwSize);
		bool setDWordValue(LPCTSTR pName, DWORD dwData);
		bool setStringValue(LPCTSTR pName, LPTSTR pData, DWORD dwSize);

		// Delete keys and values
		bool deleteKey(LPCTSTR pName);
		bool deleteValue(LPCTSTR pName);

		// Get info on current key
		DWORD numberOfSubkeys() { return numSubkeys; }
		DWORD numberOfValues() { return numValues; }
		LPCTSTR getClassName() { return className; }

		// Saving and restoring keys
		bool saveKey(LPCTSTR pFile, HKEY hk = NULL);
		bool loadKey(LPCTSTR pSubkey, LPCTSTR pFile, HKEY hk);
	};


#endif

