
// DataBaseAccessor.cpp


#include "common.h"

#include "DataBaseAccessor.h"
#include "Util2.h"


#pragma warning ( disable: 4996 )


DataBaseAccessor::DataBaseAccessor()
	{
	// bstr's always contain unicode chars!
	bstrEmpty1 = (L"");
	bstrEmpty2 = (L"");
	bstrEmpty3 = (L"");

	// set a flag that indicates whether:
	//  the connection and recordset have ever been successfully opened.
	//  this flag prevents calling methods on un-opened objects.
	m_fADOObjectsCreatedOk = FALSE;

	m_fIsConnected = FALSE;

	// set the command timeout that we prefer 10 seconds seems appropriate to start.
	m_lCommandTimeout = 10;
	}


DataBaseAccessor::~DataBaseAccessor()
	{
	}


	void
DataBaseAccessor::ADORecordsetClose(void)
	{
	try
		{
		// close the recordset before re-using it
		//  or at database clean-up time.
		if(m_fADOObjectsCreatedOk)
			{
			// ok to use GetState() method.

			// if not closed, close it.
			if(ADODB::adStateClosed != Rs99->GetState())
				{
				Rs99->Close();
				}

			// note: DO-NOT set the recordset to NULL,
			//  if you do, it will be destroyed,
			//  and a new one must be created.
			}

		PrintMessage(MSGTYPE_DATABASE_DETAIL,
						hwndDatabaseMsg,
						"ADORecordsetClose(): OK");

		// an empty message is used for visual separation.
		PrintMessage(MSGTYPE_DATABASE_COMMAND, hwndDatabaseMsg, "");
		}
	catch(_com_error &e)
		{
		m_fIsConnected = FALSE;

		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"DataBaseAccessor::ADORecordsetClose: catch (_com_error) [%08lX]",
						e.Error());
		}
	catch(...)
		{
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"DataBaseAccessor::ADORecordsetClose: catch (...)");
		}

	m_fIsConnected = FALSE;
	}


	void
DataBaseAccessor::SetConnectString(CString csConnectString)
	{
	// connect string saved in a member variable,
	//  used for connect, and re-connect, also
	//  GetConnectString();
	m_csConnectString = csConnectString;
	}


	void
DataBaseAccessor::SetCommandTimeout(long Timeout)
	{
	// connect string saved in a member variable,
	//  used for connect, and re-connect, also
	//  GetConnectString();
	m_lCommandTimeout = Timeout;
	}


	CString
DataBaseAccessor::GetConnectString(void)
	{
	// connect string saved in a member variable,
	//  used for connect, and re-connect, also
	//  GetConnectString();
	return m_csConnectString;
	}


	int
DataBaseAccessor::ADOConnectionConnect(void)
	{
	int ReturnCode = FAIL;
	char ConnectString[1000];

	try
		{
		// for security reasons... don't show the password on the screen
		//  based on "menu" in-use, it may be shown...
		//  only if the programmer menu is currently enabled.
		//
		if(bCmdLineUseProgrammerMenu)
			{
			PrintMessage(MSGTYPE_DATABASE_COMMAND,
							hwndDatabaseMsg,
							"ADOConnectionConnect(): Connect String:[%s]",
							m_csConnectString);
			}
		else
			{
			PrintMessage(MSGTYPE_DATABASE_COMMAND,
							hwndDatabaseMsg,
							"ADOConnectionConnect(): Connect String:[%s]",
							"supressed for database security");
			}

		// convert to "C" character array string.
		sprintf(ConnectString, "%s", m_csConnectString);

		// report a warning if the connection is anything but properly
		//  shut down from a previous use.
		if(m_fADOObjectsCreatedOk)
			{
			// this flag is expected to be FALSE,
			//  FALSE means that the connection has never been used,
			//  or has been used, and was properly shut down by previous user.
			PrintMessage(MSGTYPE_DATABASE_COMMAND,
							hwndDatabaseMsg,
							"DataBaseAccessor::ADOConnectionConnect(): "
							"ERROR: connection not properly shutdown by previous user.");
			}


		// create connection object now, and open it now.
		//Conn99.CreateInstance(__uuidof(ADODB::Connection));
	HRESULT hr=S_OK;

	CLSID x;
	CComBSTR objName("ADODB.Connection");
	hr = CLSIDFromString(objName, &x);
	if (FAILED(hr)) _com_issue_error(hr);

	hr = CoCreateInstance(x, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown , (void **)&Conn99);
	if (FAILED(hr)) _com_issue_error(hr);


		Conn99->ConnectionString = ConnectString;

		// adjust the connect timeout, units are seconds.
		Conn99->ConnectionTimeout = 10;

		Conn99->Open(bstrEmpty1, bstrEmpty2, bstrEmpty3, -1);

		// a slight diversion, get the ADO version.
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"ADO Version [%s]",
						(LPCSTR) Conn99->GetVersion());

		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"Provider Version [%s]",
						(LPCSTR) (_bstr_t) Conn99->Properties->GetItem("Provider Version")->Value);

		// and identify the database manager.
		CString csDBMSName;

		csDBMSName = (CString) (LPCSTR) (_bstr_t) Conn99->Properties->GetItem("DBMS Name")->Value;

		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"DBMS Name [%s]",
						csDBMSName);

		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"DBMS Version [%s]",
						(LPCSTR) (_bstr_t) Conn99->Properties->GetItem("DBMS Version")->Value);



	hr=S_OK;

	CComBSTR objNameRS("ADODB.RecordSet");
	hr = CLSIDFromString(objNameRS, &x);
	if (FAILED(hr)) _com_issue_error(hr);

	hr = CoCreateInstance(x, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown , (void **)&Rs99);
	if (FAILED(hr)) _com_issue_error(hr);

	CComBSTR objNameCM("ADODB.Command");
	hr = CLSIDFromString(objNameCM, &x);
	if (FAILED(hr)) _com_issue_error(hr);

	hr = CoCreateInstance(x, NULL, CLSCTX_INPROC_SERVER, IID_IUnknown , (void **)&Cmd99);
	if (FAILED(hr)) _com_issue_error(hr);



		// successfully opened the connection.
		m_fADOObjectsCreatedOk = TRUE;

		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"ADOConnectionConnect(): OK");

		// connection was successfully opened.
		//  set the flag to indicate connected ok.
		m_fIsConnected = TRUE;

		ReturnCode = SUCCESS;
		}
	catch(_com_error &e)
		{
		m_fIsConnected = FALSE;

		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"DataBaseAccessor::ADOConnectionConnect: catch (_com_error) [%08lX]",
						e.Error());
		}
	catch(...)
		{
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"DataBaseAccessor::ADOConnectionConnect: catch (...)");
		}
	return ReturnCode;
	}


	int
DataBaseAccessor::ADOConnectionReConnect()
	{
	int ReturnCode = FAIL;

	// a determination has been made to attempt to re-connect
	//  after some error.

	// announce the beginning of the re-connect process.
	PrintMessage(MSGTYPE_DATABASE_COMMAND,
					hwndDatabaseMsg,
					"DataBaseAccessor::ADOConnectionReConnect: Begin Re-Connect");

	// attempt to close the existing connection
	//  as cleanly as possible.
	//
	// at this point we intentionally ignore the return code,
	//  we are doing the best we can to close a known error
	//  generating connection.
	ADOConnectionRelease();

	// and attempt to connect.
	if(SUCCESS == ADOConnectionConnect())
		{
		// re-connect was successful.
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"DataBaseAccessor::ADOConnectionReConnect: Ok");

		ReturnCode = SUCCESS;
		}
	else
		{
		// re-connection failed.
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"DataBaseAccessor::ADOConnectionReConnect: NG");

		// just delay 2 seconds.
		Sleep(2000);
		}

	return ReturnCode;
	}


	int
DataBaseAccessor::ADORsSelect(CString csCaller,
								CString csTable,
								CString csSelectStatement)
	{
	int ReturnCode = DBA_RECORD_ACCESS_ERROR;
	char Caller[100];
	char Table[100];
	char SelectStatement[1000];

	// convert to "C" character array string.
	sprintf(Caller, "%s", csCaller);
	sprintf(Table, "%s", csTable);
	sprintf(SelectStatement, "%s", csSelectStatement);

	ADORecordsetClose();

	// changed to use "internal" function instead of rs->open().
	if(SUCCESS == ADORsCommand(csCaller, csTable, csSelectStatement))
		{
		// the Select statement was successful.
		try
			{
			// if both BOF == TRUE, and EOF == TRUE, no records
			//  are in the recordset, operations
			//  e.g. MoveFirst() are not possible.
			if((Rs99->BOF) && (Rs99->adoEOF))
				{
				PrintMessage(MSGTYPE_DATABASE_COMMAND,
								hwndDatabaseMsg,
								"ADORsSelect(): no records found ");

				ReturnCode = DBA_RECORD_NOT_FOUND;
				return ReturnCode;
				}

			Rs99->MoveFirst();

			ReturnCode = DBA_RECORD_FOUND;
			}
		catch(_com_error &e)
			{
			m_fIsConnected = FALSE;		// on error, declare that the connection is NG

			PrintMessage(MSGTYPE_DATABASE_COMMAND,
							hwndDatabaseMsg,
							"DataBaseAccessor::ADORsSelect: catch (_com_error) [%08lX]",
							e.Error());
			}
		catch(...)
			{
			PrintMessage(MSGTYPE_DATABASE_COMMAND,
							hwndDatabaseMsg,
							"DataBaseAccessor::ADORsSelect: catch (...)");
			}
		}
	else
		{
		// the Select statement was NOT successful.
		}

	return ReturnCode;
	}


#define MAX_ATTEMPTS_LIMIT 5



	// original one...
	//
	int
DataBaseAccessor::ADORsCommand(CString csCaller,
								CString csTable,
								CString csCommandStatement,
								bool fIsRecursiveCall)
	{
	int ReturnCode = FAIL;
	//US30642 - S0 - 6/11/2014 - Increase the size of the array from 1000 to 2000 
	//so that it can handle statements over 1000 character
	char CommandStatement[2000];

	memset(CommandStatement, 0, sizeof(CommandStatement));

	// a recursive call is indicated by the function parameter having value
	//  of fRecursiveCall == TRUE
	// when a recursive call is being used, the call is a re-try.
	//  the "attempts" count is not set to a starting value of 1, as in a normal call.
	// the attempts count is always compared to the MAX_ATTEMPTS_LIMIT to see if we should
	//  give up, and finally return an error.
	//
	if(fIsRecursiveCall)
		{
		// increase number of tries on a recursive call...
		m_iAttempts++;
		}
	else
		{
		m_iAttempts = 1;
		}

	if(fIsRecursiveCall)
		{
		// Recursive call.

		PrintMessage(MSGTYPE_DATABASE_COMMAND, 
						hwndDatabaseMsg, 
						"Attempt [%d]", 
						m_iAttempts);

		// attempt a re-connect.
		// if the re-connect was successful, then continue with an "attempt".
		PrintMessage(MSGTYPE_DATABASE_COMMAND, 
						hwndDatabaseMsg, 
						"Attempt to re-connect");

		// connection is not good.
		//  attempt to re-connect.
		if(ADOConnectionReConnect() != SUCCESS)
			{
			// no success, connection is still no good.

			// if not successfully re-connected, no point to "try", 
			//  just return from the recursion. the "attempts" count will 
			//  eventually run out.
			return FAIL;
			}
		else
			{
			// connection is now good, continue the process.
			//  just fall thru.
			}
		}

	// a first or subsequent attempt begins here.
	try
		{
		// convert to "C" character array string.
		sprintf(CommandStatement, "%s", (LPCSTR)(LPCTSTR)csCommandStatement);

		ADORecordsetClose();

		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"ADORsCommand(): Caller [%s] Table [%s]",
						csCaller,
						csTable);

		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"                CommandStatement [%s]",
						CommandStatement);

		// set a CommandTimeout on the connection object, 
		//  to timeout the recordset command.
		//
		Conn99->CommandTimeout = m_lCommandTimeout;	

		Rs99->Open(CommandStatement,
						Conn99.GetInterfacePtr(),
						ADODB::adOpenDynamic,
						ADODB::adLockOptimistic,
						ADODB::adCmdText
						);

		PrintMessage(MSGTYPE_DATABASE_DATA,
						hwndDatabaseMsg,
						"ADORsCommand(): OK");

		ReturnCode = SUCCESS;
		}
	catch(_com_error &e)
		{
		m_fIsConnected = FALSE;

		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"DataBaseAccessor::ADORsCommand: catch (_com_error) [%08lX]",
						e.Error());

		while(m_iAttempts < MAX_ATTEMPTS_LIMIT)
			{
			// note that the flag fIsRecursiveCall is set to true.
			//  this is the retrying recursive call.
			//
			int iStatus = ADORsCommand(csCaller,
							csTable,
							csCommandStatement,
							true);

			// check the status of the recursive re-try operation..
			if(SUCCESS == iStatus)
				{
				// successful... our work is done.
				ReturnCode = SUCCESS;

				// break out of the while() loop.
				break;
				}
			}

		// catch processing continues... no return...
		//  and executes code after all of the catch blocks.
		}
	catch(...)
		{
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"DataBaseAccessor::ADORsCommand: catch (...)");
		}

	// continue here after catch processing.
	PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"RETURNING FROM ADORsCommand with ReturnCode [%d]", 
						ReturnCode);
	return ReturnCode;
	}




	int
DataBaseAccessor::ADORsInsert(CString csCaller,
								CString csTable,
								CString csCommandStatement)
	{
	return ADORsCommand(csCaller, csTable, csCommandStatement);
	}


	int
DataBaseAccessor::ADORsUpdate(CString csCaller,
								CString csTable,
								CString csCommandStatement)
	{
	return ADORsCommand(csCaller, csTable, csCommandStatement);
	}


	int
DataBaseAccessor::ADORsDelete(CString csCaller,
								CString csTable,
								CString csCommandStatement)
	{
	return ADORsCommand(csCaller, csTable, csCommandStatement);
	}


	int
DataBaseAccessor::ADOGetNumeric(char * FieldName, int * FieldValue)
	{
	int ReturnCode = FAIL;
	int iNumericValue;

	PrintMessage(MSGTYPE_DATABASE_COMMAND,
					hwndDatabaseMsg,
					"DataBaseAccessor::ADOGetNumeric: Fieldname requested [%s]",
					FieldName);
	try
		{
		// get information from recordset fields into a local variable.
		vText99 = Rs99->Fields->Item[FieldName]->Value;

		// check the type of the variant that was returned
		//  it must be of the type expected, specifically VT_I4
		//  otherwise we have not received "acceptable" data
		//  the system can return VT_NULL if the database field
		//  is empty.  we are only interested in numeric data.

		switch(vText99.vt)
			{
			// expected data type was received.

			// retrieve the value of the numeric variable.
			case VT_I2:
				// retrieve the value of the numeric variable.
				iNumericValue = vText99.iVal;
				break;

			case VT_I4:
				iNumericValue = vText99.lVal;
				break;

			default:
				// expected data type was NOT received.
				//  empty fields will be returned as zero.
				iNumericValue = 0;
				break;
			}

		*FieldValue = iNumericValue;

		PrintMessage(MSGTYPE_DATABASE_DATA,
						hwndDatabaseMsg,
						"ADOGetNumeric(): FieldName [%s] Variant type [%d] Value [%d] OK",
						FieldName,
						vText99.vt,
						iNumericValue);

		ReturnCode = SUCCESS;
		}
	catch(_com_error &e)
		{
		m_fIsConnected = FALSE;

		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"DataBaseAccessor::ADOGetNumeric: catch (_com_error) [%08lX]",
						e.Error());
		}
	catch(...)
		{
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"DataBaseAccessor::ADOGetNumeric: catch (...)");
		}

	return ReturnCode;
	}


	int
DataBaseAccessor::ADOUpdateNumeric(char * FieldName, int FieldValue)
	{
	int ReturnCode = FAIL;

	try
		{
		vText99.intVal = FieldValue;

		// write information back to recordset.
		Rs99->Fields->Item[FieldName]->Value = vText99;

		Rs99->Update();

		PrintMessage(MSGTYPE_DATABASE_DATA,
						hwndDatabaseMsg,
						"ADOUpdateNumeric(): FieldName [%s] Value [%d] OK",
						FieldName,
						FieldValue);
		ReturnCode = SUCCESS;
		}
	catch(_com_error &e)
		{
		m_fIsConnected = FALSE;

		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"DataBaseAccessor::ADOUpdateNumeric: catch (_com_error) [%08lX]",
						e.Error());
		}
	catch(...)
		{
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"DataBaseAccessor::ADOUpdateNumeric: catch (...)");
		}

	return ReturnCode;
	}


	int
DataBaseAccessor::ADOGetString(char * FieldName, char * Text, int iTextSize)
	{
	CString csDummy;

	return DataBaseAccessor::ADOGetString(FieldName,
											Text,
											iTextSize,
											csDummy);
	}


	int
DataBaseAccessor::ADOGetString(char * FieldName, char * Text, int iTextSize, CString & csText1)
	{
	int ReturnCode = FAIL;
	CString csText;

	PrintMessage(MSGTYPE_DATABASE_COMMAND,
					hwndDatabaseMsg,
					"DataBaseAccessor::ADOGetString: Fieldname requested [%s]",
					FieldName);
	try
		{
		// fist thing, clear out the destination buffer.
		//  the buffer better be as big as it is stated to be.
		ZeroMemory(Text, iTextSize);

		vText99 = Rs99->Fields->Item[FieldName]->Value;

		// check the type of the variant that was returned
		//  it must be of the type expected, specifically VT_BSTR
		//  otherwise we have not received "acceptable" data
		//  the system can return VT_NULL if the database field
		//  is empty.  we are only interested in string data.
		if(VT_BSTR == vText99.vt)
			{
			// expected data type was received.

			// ok to assign even a zero length string to a CString.
			//  CString used for convenience to convert from
			//  a BSTR type, and to find the length of the string.
			csText = vText99.bstrVal;

			// take a copy of what was retrieved from the database.
			//  currently this is for test purposes.
			csText1 = vText99.bstrVal;

			// remove whitespace chars from the right hand
			//  end of the retrieved string.
			csText.TrimRight();

			int iLength = csText.GetLength();

			if(iLength > 0)
				{
				// expected data type, and non-zero length.

				// move from CString to "C" type character array.
				strncpy(Text, csText, iTextSize);

				// start at the beginning of the string,
				//  mask characters to the range of 0x0 thru 0x7f inclusive
				//  stop when a zero terminator is encountered,
				//  staying within the given buffer.

				for(int i = 0; i < (iTextSize - 1); i++)
					{
					if(Text[i] == 0)
						{
						// terminator found.
						break;
						}

					// mask into acceptable ASCII range.
					Text[i] &= 0x7f;
					}

				// at this point, the string is NOT necessarily
				//  NULL terminated.
				}
			else
				{
				// notify that the string is zero length.
				PrintMessage(MSGTYPE_DATABASE_COMMAND,
								hwndDatabaseMsg,
								"ADOGetString(): zero length string found");
				}
			}
		else
			{
			// unexpected data type,
			//  usually NULL but we are not at all interested in it.
			// the buffer was cleared out at startup, nothing
			//  left to do, but return.
			//  only a database error is considered a returnable error.
			//
			//PrintMessage(MSGTYPE_DATABASE_COMMAND,
			//				hwndDatabaseMsg,
			//				"ADOGetString(): unexpected data type, returning empty string.");
			}

		{
		// move the possibly un-terminated string characters into the CString.
		//  use a custom built formatting string. for example, if TextSize
		//  was 10, the format string would be built as: "%10.10s".
		//CString csFormatString;
		//csFormatString.Format("%%%d.%ds", iTextSize, iTextSize);

		//CString csTempText;
		//csTempText.Format(csFormatString, Text);

		PrintMessage(MSGTYPE_DATABASE_DATA,
						hwndDatabaseMsg,
						"ADOGetString(): FieldName [%s] Variant type [%d] Value [%s] OK",
						FieldName,
						vText99.vt,
						csText1);
		}

		ReturnCode = SUCCESS;
		}
	catch(_com_error &e)
		{
		m_fIsConnected = FALSE;

		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"DataBaseAccessor::ADOGetString: catch (_com_error) [%08lX]",
						e.Error());
		}
	catch(...)
		{
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"DataBaseAccessor::ADOGetString: catch (...)");
		}

	return ReturnCode;
	}


	int
DataBaseAccessor::ADOUpdateString(char * FieldName, char * Text)
	{
	int ReturnCode = FAIL;
	CString csTemp;

	try
		{
		// move information to be written to a CString.
		csTemp = Text;

		Rs99->Fields->Item[FieldName]->Value = (variant_t) csTemp;

		// after changing a field, either
		//  Update() or CancelUpdate() must be called.
		Rs99->Update();

		PrintMessage(MSGTYPE_DATABASE_DATA,
						hwndDatabaseMsg,
						"ADOUpdateString(): FieldName [%s] Text [%s] OK",
						FieldName,
						Text);

		ReturnCode = SUCCESS;
		}
	catch(_com_error &e)
		{
		m_fIsConnected = FALSE;

		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"DataBaseAccessor::ADOUpdateString: catch (_com_error) [%08lX]",
						e.Error());
		}
	catch(...)
		{
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"DataBaseAccessor::ADOUpdateString: catch (...)");
		}

	return ReturnCode;
	}


	int
DataBaseAccessor::ADOStep(void)
	{
	int ReturnCode = FAIL;

	// note that it is not an error to step past the last record
	//  but please notice this as it happens by checking
	//  the eof condition, this is considered a failure,
	//  as the next thing is an attempt to read
	//  data past the end of file. we prevent that by
	//  defining this as a failure.
	try
		{
		Rs99->MoveNext();

		// after the step() operation,
		//  check to see if EOF has been reached.
		//  if so, return "no more data", rather than
		//  an error code. usually 0x800A0BCD
		if(Rs99->adoEOF)
			{
			// eof encountered.
		   //PrintMessage(MSGTYPE_DATABASE_DATA,
		   //				hwndDatabaseMsg,
		   //				"ADOStep(): NG");
           
           //SO - 03/11/2013 - To make the fail more meaningful, 
           //I changed the ADOStep():NG to ADOStep():EOF      
              PrintMessage(MSGTYPE_DATABASE_DATA,
							hwndDatabaseMsg,
							"ADOStep(): EOF");

			ReturnCode = FAIL;
			}
		else
			{
			// not eof, still pointing at a recordset item.
			PrintMessage(MSGTYPE_DATABASE_DATA,
							hwndDatabaseMsg,
							"ADOStep(): OK");

			ReturnCode = SUCCESS;
			}
		}
	catch(_com_error &e)
		{
		m_fIsConnected = FALSE;

		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"DataBaseAccessor::ADOStep: catch (_com_error) [%08lX]",
						e.Error());
		}
	catch(...)
		{
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"DataBaseAccessor::ADOStep: catch (...)");
		}

	return ReturnCode;
	}


	int
DataBaseAccessor::ADOConnectionRelease(void)
	{
	int ReturnCode = FAIL;

	try
		{
		// set this flag to FALSE now, if we are unable to
		//  successfuly close the connection,
		//  we have done the best that we can do.
		m_fADOObjectsCreatedOk = FALSE;

		ADORecordsetClose();

		if(m_fADOObjectsCreatedOk)
			{
			// ok to call GetState() method.

			// close down the command object.
			//
			// if not NULL, make it NULL,
			//  this is the end of life for the object.
			if(Cmd99 != NULL)
				{
				Cmd99 = NULL;
				}

			// close down the connection object.
			//
			// if not closed, close it.
			if(ADODB::adStateClosed != Conn99->GetState())
				{
				Conn99->Close();
				}

			// if not NULL, make it NULL,
			//  this is the end of life for the object.
			if(Conn99 != NULL)
				{
				Conn99 = NULL;
				}
			}
		else
			{
			// never opened, or previously successfuly closed,
			//  nothing needs to be done.
			}

		PrintMessage(MSGTYPE_DATABASE_DATA,
						hwndDatabaseMsg,
						"ADOConnectionRelease(): OK");

		ReturnCode = SUCCESS;
		}
	catch(_com_error &e)
		{
		m_fIsConnected = FALSE;

		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"DataBaseAccessor::ADOConnectionRelease: catch (_com_error) [%08lX]",
						e.Error());
		}
	catch(...)
		{
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"DataBaseAccessor::ADOConnectionRelease: catch (...)");
		}

	m_fIsConnected = FALSE;

	return ReturnCode;
	}


	int
DataBaseAccessor::ADOCmdExecuteSql(CString csCaller,
								CString csTable,
								CString csSqlCommandText)
	{
	int ReturnCode = FAIL;

	PrintMessage(MSGTYPE_DATABASE_COMMAND, 
					hwndDatabaseMsg, 
					"DataBaseAccessor::ADOCmdExecuteSql: STEP 0");

	// check that the Connection state is OK.
	//  if not ok, attempt to reconnect.
	//
	if(!m_fIsConnected)
		{
		// connection is not good.
		//  attempt to re-connect.
		if(ADOConnectionReConnect() != SUCCESS)
			{
			// no success, connection is still no good.
			return ReturnCode;
			}
		}

	try
		{
		ADORecordsetClose();

		PrintMessage(MSGTYPE_DATABASE_COMMAND, 
						hwndDatabaseMsg, 
						"ADOCmdExecuteSql: STEP 1");

		Cmd99->CommandTimeout = m_lCommandTimeout;

		Cmd99->ActiveConnection = Conn99;
		Cmd99->CommandText = (_bstr_t) csSqlCommandText;
		Cmd99->CommandType = ADODB::adCmdText;

		PrintMessage(MSGTYPE_DATABASE_COMMAND, 
						hwndDatabaseMsg, 
						"ADOCmdExecuteSql: STEP 2");

		PrintMessage(MSGTYPE_DATABASE_COMMAND, 
						hwndDatabaseMsg, 
						"csSqlCommandText [%s]", 
						csSqlCommandText);

		PrintMessage(MSGTYPE_DATABASE_COMMAND, 
						hwndDatabaseMsg, 
						"ADOCmdExecuteSql: Execute begin..........");

		Rs99 = Cmd99->Execute(NULL, NULL, ADODB::adCmdText);

		PrintMessage(MSGTYPE_DATABASE_COMMAND, 
						hwndDatabaseMsg, 
						"ADOCmdExecuteSql: Execute completed......");

		PrintMessage(MSGTYPE_DATABASE_COMMAND, 
						hwndDatabaseMsg, 
						"ADOCmdExecuteSql: STEP 6");

		// at this point, the recordset is usable...
		//  by getnumeric(), getstring(), etc.
		PrintMessage(MSGTYPE_DATABASE_DATA,
						hwndDatabaseMsg,
						"ADOCmdExecuteSql(): OK");

		ReturnCode = SUCCESS;
		}
	catch(_com_error &e)
		{
		m_fIsConnected = FALSE;

		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"DataBaseAccessor::ADOCmdExecuteSql: catch (_com_error) [%08lX]",
						e.Error());
		}
	catch(...)
		{
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"DataBaseAccessor::ADOCmdExecuteSql: catch (...)");
		}

	return ReturnCode;
	}

