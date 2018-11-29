
// DataBaseAccessor.h

#pragma once

// disable the warning about unary minus on an unsigned,
#include "AdoWrap.h"

class DataBaseAccessor
	{
	public:
		DataBaseAccessor();
		~DataBaseAccessor();

		int ADOConnectionConnect(void);

		int ADORsSelect(CString, CString, CString);
		int ADORsInsert(CString, CString, CString);
		int ADORsUpdate(CString, CString, CString);
		int ADORsDelete(CString, CString, CString);
		int ADOConnectionRelease();
		int ADOStep(void);

		int ADOGetNumeric(char *, int *);
		int ADOUpdateNumeric(char *, int);
		int ADOGetString(char *, char *, int, CString &); // for debug convenience.
		int ADOGetString(char *, char *, int);
		int ADOUpdateString(char *, char *);

		// used for SQL command "execution".
		int ADOCmdExecuteSql(CString, 
							CString, 
							CString);

		// methods to set and get property values.
		void SetConnectString(CString);
		CString GetConnectString(void);
		void SetCommandTimeout(long);

	private:
		void ADORecordsetClose(void);
		int ADORsCommand(CString, CString, CString, bool fRecursiveCall = false);
		int ADOConnectionReConnect(void);

		// ado objects
		AdoWrapConnection* adoConnection;
		CString m_csConnectString;
		long m_lCommandTimeout;

	};

