// AdoWrap.cpp: implementation of the AdoWrap class.
//
//////////////////////////////////////////////////////////////////////

#include "Common.h"
#include <ostream>
#include <iomanip>

#include "AdoWrap.h"


//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif




//using namespace ADODB;
using namespace std;
using namespace ADODB;

//extern ostream errorLogStream;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//AdoWrapException::AdoWrapException(const string& msg)
//{;}

#if 0
void __stdcall
   _com_raise_error(HRESULT hr, IErrorInfo* perrinfo ) throw(_com_error)
       {
           //this message box is for demonstration purpose only
          //AfxMessageBox( "_com_raise_error (HRESULT, IErrorInfo*)" );
			AfxMessageBox( "my message" );
		  //your own error handling code or just an abort
       }
#endif





// Obtain the error message for a given HRESULT
   CString LogCrackHR( HRESULT hr )
   {
      LPVOID  lpMsgBuf;
      CString strTmp;

      ::FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER |
                       FORMAT_MESSAGE_FROM_SYSTEM,
                       NULL,
                       hr,
                       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                       (LPTSTR) &lpMsgBuf,
                       0,
                       NULL );

        // STR_TMP is defined within LOG.CPP to provide safe format string
        // for both ANSI and UNICODE
        strTmp.Format( "%s", (char *) lpMsgBuf );

		strTmp.Trim();
      // Free the buffer.
      ::LocalFree( lpMsgBuf );

      return strTmp;
   }


   
   

//create a connection and open it
AdoWrapConnection::AdoWrapConnection(int cmdTimeout, int ConnectTimeout, /*CAdtwDoc* doc,*/ const char* connStr, const char* uId, const char* passw) : connection(NULL)
{
	HRESULT hr=S_OK;

	connectState = eUnknown;
	connectionStr = ""; //"Dsn=";
	connectionStr.append(connStr);
	userId = uId;
	password = passw;
	//pDoc = doc;

	commandTimeout = (cmdTimeout > 0 ? cmdTimeout : 30);
	connectionTimeout = (ConnectTimeout > 0 ? ConnectTimeout : 30);
	CLSID x;
	CComBSTR objName("ADODB.Connection");
	hr = CLSIDFromString(objName, &x);

	if(!FAILED(hr))
		hr = CoCreateInstance(x, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown , (void **)&connection);

	if (FAILED(hr)) 
		_com_issue_error(hr);

	try
	{
		connection->PutConnectionTimeout(connectionTimeout);
		connection->PutCommandTimeout(commandTimeout);
	}

	catch(_com_error &e)
	{
		*dbLogStream << "*****  HRESULT ******" << endl;
		*dbLogStream << LogCrackHR(e.Error()) << endl;
		LogAdoError();
		//ConnectionLost();
		throw e;
	}

	//connection error logging
	
    *dbLogStream << "Calling reconnect from AdoWrapConnection" << flush;

	OpenDbConnection();

}



void AdoWrapConnection::OpenDbConnection()
{
	try
	{
		*dbLogStream << "Calling open from Connect" << flush;

		if(connectState ==	eConnected)
			return;
		connection->Open(connectionStr.c_str(), userId.c_str(), password.c_str(), NULL);

		//connection error logging
		*dbLogStream << "Exiting Connect" << flush;
		connectState = eConnected;
	}

	catch(_com_error &e)
	{
		*dbLogStream << "*****  HRESULT ******" << endl;
		*dbLogStream << LogCrackHR(e.Error()) << endl;
		LogAdoError();
		//ConnectionLost();
		throw e;
	}
}



//AdoWrapConnection::~AdoWrapConnection()
//{
//	if (connectState == eConnected)
//	{
//		connectState = eUnknown;
//	}
//}

   HRESULT AdoWrapConnection::LogAdoError()
   {
      ErrorsPtr   pErrors = NULL;
      ErrorPtr    pError  = NULL;
      CString     strTmp;
      HRESULT     hr = (HRESULT) 0L;
      long        nCount;
	  bool		connectionError = false;

      // Don't have an un-handled exception in the handler that
      // handles exceptions!
      try
      {
         pErrors = connection->GetErrors();

         nCount = pErrors->GetCount();

         for( long i = 0; (!FAILED(hr)) && (i < nCount); i++ )
         {
            *dbLogStream << "  Dumping ADO Error " << i+1 << " of " << nCount << flush;

            hr = pErrors->get_Item((_variant_t)((long)i), &pError );

            _bstr_t bstrSource     ( pError->GetSource()      );
            _bstr_t bstrDescription( pError->GetDescription() );
            _bstr_t bstrHelpFile   ( pError->GetHelpFile()    );
            _bstr_t bstrSQLState   ( pError->GetSQLState()    );

			bstrHelpFile = (!bstrHelpFile ? "" : bstrHelpFile);


			LPCTSTR sqlState = (LPCTSTR) (!bstrSQLState ? "" : bstrSQLState);

			if (strlen(sqlState) >= 2)
			{
				if(sqlState[0] == '0' && sqlState[1] == '8')
				{
					connectionError = true;
				}
			}

			*dbLogStream << "    Number      = " << setw(8) << setfill('0') << std::hex << pError->GetNumber() << flush;
			*dbLogStream << "    Source      = " << (LPCTSTR) bstrSource << flush;
			*dbLogStream << "    Description = " << (LPCTSTR) bstrDescription << flush;
			*dbLogStream << "    HelpFile    = " << (LPCTSTR) bstrHelpFile << flush;
			*dbLogStream << "    HelpContext = " << pError->GetHelpContext() << flush;
			*dbLogStream << "    SQLState    = " << sqlState << flush;
			*dbLogStream << "    NativeError = " << pError->GetNativeError() << flush;
         }
		 if(connectionError)
			 DropConnection();
      }
	  //MFC EXCEPTION
      //catch( CException *e )
      //{
      //   dbLogStream << "*** UNABLE TO LOG EXCEPTION ***";
      //   e->Delete();
      //}
      catch(...)
      {
         *dbLogStream << "*** UNABLE TO LOG EXCEPTION ***";
      }

      if( pErrors ) pErrors->Release();
      if( pError  ) pError->Release();

      return hr;
   }



void AdoWrapConnection::DropConnection()
{
	if(IsConnnected())
	{
		connectState = eDisconnected;
		connection->Close();
	}
}


AdoWrapConnection* AdoWrapConnection::CreateConnection(int cmdTimeout, int ConnectTimeout, /* CAdtwDoc* doc, */ const char* connStr, const char* uId, const char* passw)
{
	AdoWrapConnection* wrapConnection;
	try
	{
		//connection error logging
		*dbLogStream << "Calling new" << flush;

		wrapConnection = new AdoWrapConnection(cmdTimeout, ConnectTimeout, /* doc,*/ connStr, uId, passw);
		//connection error logging
		*dbLogStream << "Calling reconnect from CreateConnection" << flush;
	}
	catch(std::bad_alloc)
	{
		//log error
		*dbLogStream << "new AdoWrapConnection failed" << flush;
		throw;
	}
	return wrapConnection;
}








void AdoWrapConnection::Reconnect(int retry)
{
	HRESULT hr=S_OK;

	int retryCount = 0; //retry;
	int retryMax = 1;

	if(connectState != eConnected)
	{
		while(retryCount++ < retryMax)
		{
			OpenDbConnection();
		}
	}
}




bool AdoWrapConnection::IsConnnected()
{
	bool res;
	
	res =  connectState == eConnected && connection->GetState() == ADODB::adStateOpen;
	return res;
}












AdoWrapRecordSet::AdoWrapRecordSet()
{
	HRESULT hr=S_OK;

	CLSID x;
	CComBSTR objName("ADODB.RecordSet");
	hr = CLSIDFromString(objName, &x);
	if (FAILED(hr)) _com_issue_error(hr);

	hr = CoCreateInstance(x, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown , (void **)&recordSet);
	if (FAILED(hr)) _com_issue_error(hr);
}


AdoWrapRecordSet::AdoWrapRecordSet(const char* lpstrExec)
{
	HRESULT hr=S_OK;

	CLSID x;
	CComBSTR objName("ADODB.RecordSet");
	hr = CLSIDFromString(objName, &x);
	if (FAILED(hr)) _com_issue_error(hr);

	hr = CoCreateInstance(x, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown , (void **)&recordSet);
	if (FAILED(hr)) _com_issue_error(hr);

}




AdoWrapRecordSet::AdoWrapRecordSet(ADODB::_RecordsetPtr recSetPtr)
{
	this->recordSet = recSetPtr;
}


//AdoWrapRecordSet::~AdoWrapRecordSet()
//{
//}




void AdoWrapRecordSet::Open(AdoWrapConnection* conn, const char* query, int option)
{
	//close record set
	if(conn == NULL)
	{
		throw AdoWrapExceptionConnect("AdoWrapRecordSet: Null pointer used to open record set", 0);
	}

	try
	{
		recordSet->CursorType = ADODB::adOpenStatic;
		recordSet->CursorLocation = ADODB::adUseClient;

		recordSet->Open(query, _variant_t(static_cast<IDispatch*>(*conn), TRUE), 
								ADODB::adOpenStatic, ADODB::adLockOptimistic, ADODB::adCmdText);
	}
	catch(_com_error& e)
	{
		*dbLogStream << "*****  HRESULT ******" << flush;
		*dbLogStream << LogCrackHR(e.Error()) << flush;
		conn->LogAdoError();
		throw e;
	}
}




void AdoWrapRecordSet::GetFieldValue(int index, std::string& val)
{
	_variant_t value;
	_bstr_t str;
	ADODB::FieldPtr pField = GetField(index);
	value = pField->GetValue();

	switch(value.vt)
	{
	case VT_EMPTY:
	case VT_NULL:
		//return as empty string, done in original ATD
		val = _T("");
		break;

	case VT_I2:
	case VT_I4:
		value.ChangeType(VT_BSTR);

	case VT_BSTR:
		str.Assign(value.bstrVal);
		val = str;
		break;
	default:
		throw AdoWrapException("GetFieldValue conversion error", 0);
	}
}




void AdoWrapRecordSet::GetFieldValue(int index, CString& val)
{
	_variant_t value;
	_bstr_t str;

	ADODB::FieldPtr pField = GetField(index);
	value = pField->GetValue();
	
	//do type conversion to string
	switch(value.vt)
	{
	case VT_EMPTY:
	case VT_NULL:
		//return as empty string, done in original ATD
		val = _T("");
		break;

	case VT_I2:
	case VT_I4:
		value.ChangeType(VT_BSTR);

	case VT_BSTR:
		str.Assign(value.bstrVal);
		val = (char*) str;
		break;

	default:
		throw AdoWrapException("GetFieldValue conversion error", 0);
	}
}





void AdoWrapRecordSet::GetFieldValue(int index, short& val)
{
	_variant_t value;

	ADODB::FieldPtr pField = GetField(index);
	value = pField->GetValue();

	val = value;
}



void AdoWrapRecordSet::GetFiledValue(int index, int& val)
{
	_variant_t value;

	ADODB::FieldPtr pField = GetField(index);
	value = pField->GetValue();

	val = (long) value;
}





void AdoWrapRecordSet::GetFieldName(int index, std::string& val)
{
	_variant_t value;

	ADODB::FieldPtr pField = GetField(index);

	value = pField->GetName();

	_bstr_t str;
	str.Assign(value.bstrVal);
	val = str;
}



void AdoWrapRecordSet::GetFieldName(int index, CString& val)
{
	_variant_t value;

	ADODB::FieldPtr pField = GetField(index);
	value = pField->GetName();

	_bstr_t str;
	str.Assign(value.bstrVal);
	val = (char*) str;
}


void AdoWrapRecordSet::GetFieldType(short index, ADODB::DataTypeEnum& val)
{
	ADODB::PropertyPtr propPtr = NULL;

	ADODB::FieldPtr pField = GetField(index);
	//propPtr = pField->GetProperties();

	//val = propPtr->GetType();
	val = pField->GetType();

}

void AdoWrapRecordSet::GetFieldDefinedSize(int index, long& val)
{

	ADODB::FieldPtr pField = GetField(index);
	val = pField->GetDefinedSize();
}





AdoWrapCommand::AdoWrapCommand(AdoWrapConnection* ptr) : command(NULL), conn(ptr)
{
	HRESULT hr=S_OK;

	if(ptr == NULL)
	{
		throw AdoWrapExceptionConnect("AdoWrapCommand: Null pointer used to create command", 0);
	}

	if(!ptr->IsConnnected())
	{
		throw AdoWrapExceptionConnect("AdoWrapCommand: DB connection not open", 0);
	}
	
	CLSID x;
	CComBSTR objName("ADODB.Command");
	hr = CLSIDFromString(objName, &x);
	if (FAILED(hr)) _com_issue_error(hr);


	if(!FAILED(hr))
		hr = CoCreateInstance(x, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown , (void **)&command);

	if (FAILED(hr)) _com_issue_error(hr);
	try
	{
		command->PutActiveConnection(_variant_t(static_cast<IDispatch*>(*ptr), TRUE));
		//dummy sql command
		command->PutCommandText("SELECT Name FROM xkm WHERE ProfileId = 'P122022430'");

	}
	catch(_com_error &e)
	{
		*dbLogStream << "*****  HRESULT ******" << endl;
		*dbLogStream << LogCrackHR(e.Error()) << endl;
		ptr->LogAdoError();

	}
}



//AdoWrapCommand::~AdoWrapCommand()
//{
//}



auto_ptr<AdoWrapRecordSet> AdoWrapCommand::Execute(const string& query)
{
	try
	{
		//set sql command
		command->PutCommandText(query.c_str());
		ADODB::_RecordsetPtr cPtr = command->Execute(&vtMissing, &vtMissing, ADODB::adCmdText);

		AdoWrapRecordSet* result = new AdoWrapRecordSet(cPtr);
		return auto_ptr<AdoWrapRecordSet>(result);

	}
	catch(_com_error &e)
	{
		*dbLogStream << "SQL exception: " << query << flush;
		*dbLogStream << "*****  HRESULT ******" << flush;
		*dbLogStream << LogCrackHR(e.Error()) << flush;
		conn->LogAdoError();
		throw e;
	}
}