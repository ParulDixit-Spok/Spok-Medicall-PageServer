// AdoWrap.h: interface for the AdoWrap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADOWRAP_H__E0219125_4201_4FC2_A92F_7FCCAAADD8DB__INCLUDED_)
#define AFX_ADOWRAP_H__E0219125_4201_4FC2_A92F_7FCCAAADD8DB__INCLUDED_

#pragma warning ( disable: 4146 )
//#import "C:\Program Files\Common Files\System\ado\msado15.dll" rename( "EOF", "adoEOF" )

#import "C:\Program Files\Common Files\System\ado\msado15.dll" \
	rename("EOF", "adoEOF")

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <stdexcept>
#include <memory>




class AdoWrapException : public std::runtime_error
{
public:
	AdoWrapException(const std::string& msg, HRESULT err) : std::runtime_error(msg), error(err){;}
	//AdoWrapException(const std::string& msg);

	//virtual ~AdoWrapException(){;}
private:
	HRESULT error;
};


class AdoWrapExceptionConnect : public AdoWrapException
{
public:
	AdoWrapExceptionConnect(const std::string& msg, HRESULT err) : AdoWrapException(msg, err)
	{;}
};


class AdoWrapConnection
{
public:
	AdoWrapConnection(int cmdTimeout, int ConnectTimeout, /* CAdtwDoc* doc,*/ const char* connStr, const char* uId, const char* passw);

	static AdoWrapConnection* CreateConnection(int cmdTimeout, int ConnectTimeout, /*CAdtwDoc* doc,*/ const char* connStr, const char* uId, const char* passw);


	void GetVersion(char* ver, int size)
	{strncpy_s(ver, size, connection->GetVersion(), size);}

	ADODB::_ConnectionPtr GetConnection() {return connection;}

	void PrintProviderError(int startCount);
	bool IsConnnected();
	void OpenDbConnection();
	void Reconnect(int retry = 1);

	operator IDispatch*() {return connection;}
	HRESULT LogAdoError();

private:
	void DropConnection();

public:
	enum ConnectState {eUnknown, eHalfBaked, eInitilized, eConnected, eDisconnected, eReconnection};

protected:

	ADODB::_ConnectionPtr connection;
	std::string connectionStr;
	std::string userId;
	std::string password;

	long connectionTimeout;
	long commandTimeout;
	ConnectState connectState;
	//CAdtwDoc* pDoc;
};



class AdoWrapRecordSet
{
public:
	AdoWrapRecordSet(const char* lpstrExec);
	AdoWrapRecordSet();

	AdoWrapRecordSet(ADODB::_RecordsetPtr recSetPtr);

	//virtual ~AdoWrapRecordSet();


	void Open(AdoWrapConnection* ,const char* query, int option);
	int GetRecordCount() {return recordSet->GetRecordCount();}
	int GetFieldCount() {return recordSet->GetFields()->GetCount();}

	void MoveNext(){recordSet->MoveNext();}

	bool IsEOF(){return (recordSet->GetadoEOF() == -1);}
	bool IsBOF(){return (recordSet->GetBOF() == TRUE);}

	void GetFieldValue(int index, std::string& val);
	void GetFieldValue(int index, CString& val);
	void GetFieldValue(int index, short& val);
	void GetFiledValue(int index, int& val);
	void GetFieldName(int index, std::string& val);
	void GetFieldName(int index, CString& val);


	void GetFieldType(short index, ADODB::DataTypeEnum& val);
	void GetFieldDefinedSize(int index, long& val);



protected:
	ADODB::FieldPtr GetField(int index)
	{
		_variant_t vtIndex;
		vtIndex.vt = VT_I2;
		vtIndex.iVal = index;
		return recordSet->GetFields()->GetItem(vtIndex);
	}


protected:
	ADODB::_RecordsetPtr recordSet;

};





class AdoWrapCommand
{
public:
	AdoWrapCommand(AdoWrapConnection* ptr);
	//virtual ~AdoWrapCommand();

	std::auto_ptr<AdoWrapRecordSet> Execute(const std::string& query);

protected:
	ADODB::_CommandPtr command;
	AdoWrapConnection* conn;
	std::string queryStr;
};





#endif // !defined(AFX_ADOWRAP_H__E0219125_4201_4FC2_A92F_7FCCAAADD8DB__INCLUDED_)
