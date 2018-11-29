
// DataBaseAccessor.h

#pragma once

// disable the warning about unary minus on an unsigned,
//  this is machine generated code from microsoft, and cannot be easily changed, so ignore it.
//#import "C:\Program Files\Common Files\System\ado\msado15.dll" rename( "EOF", "adoEOF" )
#import <msado15.dll> rename( "EOF", "adoEOF" )


#include <atlstr.h>
#include "enumsS.h"

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

		int m_iAttempts;

		// ado objects
		ADODB::_ConnectionPtr Conn99;
		ADODB::_CommandPtr Cmd99;
		ADODB::_RecordsetPtr Rs99;	

		_variant_t vText99;

		_bstr_t bstrEmpty1;
		_bstr_t bstrEmpty2;
		_bstr_t bstrEmpty3;

		// flags / booleans
		BOOL m_fADOObjectsCreatedOk;
		BOOL m_fIsConnected;

		// property values
		CString m_csConnectString;
		long m_lCommandTimeout;
	};

