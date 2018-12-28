

// Impairment.cpp
// disable deprecation
#pragma warning(disable:4996)


#include "common.h"


//#include "DataBaseAccessor.h"
#include "Impairment.h"



// this class is not thread safe...
//  intended to be called only by the main thread.
//  maintains its own internal "timer", and performs
//  its function only when the timer interval expires,
//  then re-arms the counter interval.
//

Impairment::Impairment()
	{
	// choose default values for properties,
	//  these may be replaced by class "set" methods.

	// setup default check interval.	
	//  units are seconds.
	setCheckInterval(30);

	// Page Status value required to enable the announcement.
	setStatusToEnable("1");

	// ProfileId pattern to identify all the 
	//  profiles that are to be selected from the database, 
	//  and inserted into the map.
	setProfileIdPattern("  IMP-");

	// setup for 30 items in the map, the maximum number
	//  of queue files that the PageServer handles. 
	//  use a prime number, 37 for good distribution.
	ImpairmentMap.InitHashTable(37); 

	ClearTheMap();

	// take the "time now" as "time zero".
	GetLocalTime(&m_iImpairmentTimer);
	}


Impairment::~Impairment()
	{
	ClearTheMap();
	}


// set and get methods ==> properties.
//
	void
Impairment::setCheckInterval(int Value)
	{
	m_iCheckInterval = Value;
	}


	void
Impairment::setProfileIdPattern(CString Value)
	{
	m_csProfileIdPattern = Value;
	}


	void
Impairment::setStatusToEnable(CString Value)
	{
	m_csEnabledStatusCharacter = Value;
	}


	void
Impairment::CheckImpairment(void)
	{
	// first check the time interval.
	//  if expired, read the Impairment Profiles.
	//  and set up the timer again.

	// "Wait" units are 1/100 second
	if(Wait(&m_iImpairmentTimer, (long) m_iCheckInterval * 100))
		{
		// read database items.
		//
		g_IMP.ReadDatabaseItems(m_csProfileIdPattern, m_csEnabledStatusCharacter);

		// take the "time now" as the new "time zero".
		GetLocalTime(&m_iImpairmentTimer);
		}
	else
		{
		// nothing to do... not time yet.
		}
	}


	// given a PageDeviceType,
	//  check to see if the Device Type is in our "map", 
	//  of impaired vendors, and if it is... return the ProfileId that holds/owns
	//  the "Custom Greeting" recording for this vendor.
	//
	CString
Impairment::GetCustomGreetingProfileId(CString csPageDeviceType)
	{
	CString csValue = "";

	if(ImpairmentMap.Lookup(csPageDeviceType, csValue))
		{
		// non-zero means that the item WAS found in the map.
		}

	return csValue;
	}


	// given a PageDeviceType,
	//  check to see if the Device Type is in our "map", 
	//  of impaired vendors, and if it is... return true.
	//  otherwise return false.
	//
	bool
Impairment::IsImpairedVendor(CString csPageDeviceType)
	{
	bool bStatus = false;

	CString csValue = "";

	if(ImpairmentMap.Lookup(csPageDeviceType, csValue))
		{
		// non-zero means that the item WAS found in the map.
		bStatus = true;
		}

	return bStatus;
	}


	void
Impairment::ClearTheMap()
	{
	ImpairmentMap.RemoveAll();
	}


	void
Impairment::AddItemToMap(CString csKey, CString csValue)
	{
	// add key, value pair to the map for subsequent look-up
	//
	ImpairmentMap[csKey] = csValue;
	}


	void
Impairment::ReadDatabaseItems(CString csProfileIdPattern, CString csStatusCharacter)
	{
	CString csFunctionName = "ReadDatabaseItems";
	CString csTableName = "Pagers";
	CString csSQLString;
	int status;

	// Always clear out the map at the start...
	//  the map will be re-filled if there are any
	//  profiles with Status set to "Impaired" "PageStatus".
	//
	ClearTheMap();

	// access the database, and find all the profiles, and their xnfrq entries
	//  that are associated with the "Impairment Announcement" feature.
	//

	// prepare to open a database connection

	// decide which connection string to use
	//  based on failsafe/network command line option.
	CString csConnectionString;

	if(DATA_SOURCE_LOCAL == DataSource)
		{
		// use local drive database (failsafe).
		csConnectionString = param.ConnectStringFailsafe;
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"ReadDatabaseItems(): DATA_SOURCE_LOCAL == DataSource");
		}
	else
		{
		// use network database.
		csConnectionString = param.ConnectStringNetwork;
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"ReadDatabaseItems(): DATA_SOURCE_NETWORK == DataSource");
		}

	// create the database accessor object(s).
	PrintMessage(MSGTYPE_DATABASE_COMMAND, 
					hwndDatabaseMsg, 
					"ReadDatabaseItems(): new pDBA");

	DataBaseAccessor * pDBA6 = new DataBaseAccessor();

	// connection string as specified in application configuration parameter.
	pDBA6->SetConnectString(csConnectionString);

	if(SUCCESS == pDBA6->ADOConnectionConnect())
		{
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"ReadDatabaseItems(): ADOConnectionConnect: OK");
		}
	else
		{
		PrintMessage(MSGTYPE_DATABASE_COMMAND,
						hwndDatabaseMsg,
						"ReadDatabaseItems(): ADOConnectionConnect: NG");
		}

	// prepare the select/join statement.
	//  note that no spaces are fixed on the left side of the ProfileId pattern,
	//  if you want to match, type it into the config utility correctly,
	//  using leading spaces as you require.
	//
	csSQLString.Format(
						"SELECT TOP 30 Pagers.ProfileId, Pagers.Status, Pagers.PagerId, "
						"XnFrq.ProfileId, XnFrq.FrqType, "
						"Xkm.ProfileId, Xkm.Format "
						"FROM Pagers "
						"JOIN XnFrq "
						"ON (Pagers.ProfileId = XnFrq.ProfileId) "
						"JOIN Xkm "
						"ON (Pagers.ProfileId = Xkm.ProfileId) "
						"WHERE Pagers.ProfileId LIKE '%s%%' "	// no leading spaces added or removed.
						"AND Pagers.Status = '%1.1s' "
						"AND Xkm.Format = %d "
						"--SQL-1-1122",
						csProfileIdPattern,
						csStatusCharacter,
						g_iImpairmentFormatNumber);			// Xkm Format Number default is 75.

	status = pDBA6->ADORsSelect(csFunctionName, csTableName, csSQLString);

	if(DBA_RECORD_FOUND == status)
		{
		// get up to 30 items from the recordset.
		//
		bool bStepOk = true;
		int iCount = 1;

		char buffer1[10];
		char buffer2[10];
		char buffer3[1];
		char buffer4[3];

		CString csRecordsetProfileId;
		CString csRecordsetStatus;
		CString csRecordsetPagerId;
		CString csRecordsetFrqType;

		while(bStepOk && iCount <= 30)
			{
			// retrieve data from the recordset.

			ZeroMemory(buffer1, sizeof(buffer1));
			ZeroMemory(buffer2, sizeof(buffer2));
			ZeroMemory(buffer3, sizeof(buffer3));
			ZeroMemory(buffer4, sizeof(buffer4));

			pDBA6->ADOGetString("ProfileId", buffer1, sizeof(buffer1), csRecordsetProfileId);
			pDBA6->ADOGetString("Status",    buffer2, sizeof(buffer2), csRecordsetStatus);
			pDBA6->ADOGetString("PagerId",   buffer3, sizeof(buffer3), csRecordsetPagerId);
			pDBA6->ADOGetString("FrqType",   buffer4, sizeof(buffer3), csRecordsetFrqType);

			csRecordsetProfileId.TrimLeft();
			csRecordsetProfileId.TrimRight();

			csRecordsetStatus.TrimLeft();
			csRecordsetStatus.TrimRight();

			csRecordsetPagerId.TrimLeft();
			csRecordsetPagerId.TrimRight();

			csRecordsetFrqType.TrimLeft();
			csRecordsetFrqType.TrimRight();

			PrintMessage(MSGTYPE_SYSTEM,
							hwndDatabaseMsg,
							"ProfileId: [%s] Status: [%s] PagerId: [%s] FrqType: [%s]",
							csRecordsetProfileId,
							csRecordsetStatus,
							csRecordsetPagerId,
							csRecordsetFrqType);

			// store into the map.
			//
			AddItemToMap(csRecordsetFrqType, csRecordsetProfileId);

			// check that ADOStep() was successful,
			//  if not, assume that end of file was encountered.
			//
			if(SUCCESS != pDBA6->ADOStep())
				{
				// no success, end of file encountered.
				PrintMessage(MSGTYPE_SYSTEM, 
								hwndDatabaseMsg, 
								"end of file found...");

				bStepOk = false;
				}
			}
		}

	CString csTemp;

	if(SUCCESS == pDBA6->ADOConnectionRelease())
		{
		csTemp = "release OK";
		}
	else
		{
		csTemp = "release NG";
		}

	PrintMessage(MSGTYPE_SYSTEM, 
					hwndDatabaseMsg, 
					"ReadDatabaseItems(): [%s].", 
					csTemp);

	// delete the database accessor object.
	delete pDBA6;

	// show the contents of the ImpairmentMap.
	//
	CString csKey;
	CString csValue;

	PrintMessage(MSGTYPE_SYSTEM, 
					hwndDatabaseMsg, 
					"Map Contains [%d] Items.", 
					ImpairmentMap.GetCount());

	POSITION Pos;

	Pos = ImpairmentMap.GetStartPosition();

	while(Pos)		
		{
		// while Pos is not null
		
		ImpairmentMap.GetNextAssoc(Pos, csKey, csValue);

		PrintMessage(MSGTYPE_SYSTEM, 
						hwndDatabaseMsg, 
						"Key: [%s]  Value: [%s]", 
						csKey, 
						csValue);
		}
	}





