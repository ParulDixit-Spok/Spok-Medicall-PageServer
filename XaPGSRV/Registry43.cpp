

// Registry43.cpp


// registry utility functions shared between 
//  PageServer4, PageServer3, and PageConfig applications.

// for CString class etc.  this needs to be before windows.h
//#include <afx.h>
#include "common.h"
#include <atlstr.h>
#include "Registry43.h"
#include "Registry.h"

// disable deprecation
#pragma warning(disable:4996)


	unsigned long
GetRegistryULong(CString csKeyName, CString csValueName)
	{
	Registry r;
	unsigned long dwTemp = 0;

	// see if the key exists.
	if(r.openKeyAbs(csKeyName))
		{
		// the key exists.
		r.getDWordValue(csValueName, &dwTemp);
		}
	else
		{
		// the key does not exist, create it.
		r.createKeyAbs(csKeyName);

		// store a starting (zero) value
		r.setDWordValue(csValueName, dwTemp);
		}

	return dwTemp;
	}


	void
SetRegistryULong(CString csKeyName,
					  CString csValueName,
					  unsigned long dwValue)
	{
	Registry r;

	// see if the key exists.
	if(r.openKeyAbs(csKeyName))
		{
		// the key exists.
		}
	else
		{
		// the key does not exist, create it.
		r.createKeyAbs(csKeyName);
		}

	// store the given value at this location.
	r.setDWordValue(csValueName, dwValue);
	}


	CString
GetRegistryStringValue(CString csKeyName, CString csValueName)
	{
	Registry r;
	char Value[400];
	unsigned long Size = sizeof(Value);
	CString csValue;

	ZeroMemory(Value, sizeof(Value));

	// see if the key exists.
	if(r.openKeyAbs(csKeyName))
		{
		// the key exists.

		// read the value into a variable.
		if(r.getStringValue(csValueName, Value, &Size))
			{
			// place into destination cstring.
			csValue = Value;
			}
		}
	return csValue;
	}


	void
SetRegistryStringValue(CString csKeyName,
						CString csValueName,
						CString csValue)
	{
	Registry r;
	char Value[400];

	ZeroMemory(Value, sizeof(Value));

	r.openKeyAbs(csKeyName);

	// use c type character array.
	strcpy(Value, csValue);

	// write it out.
	r.setStringValue(csValueName, Value, strlen(Value));		
	}

