
// Util43.cpp


// common to PageWin4, and PageWin3

// for CString class etc.  this needs to be before windows.h
//#include <afx.h>
//#include <atlstr.h>
#include "Common.h"
//#include "QueueBufferClass.h"
//#include "Structs43.h"


// disable deprecation
#pragma warning(disable:4996)

extern void AddTimeSeconds(SYSTEMTIME &, int);
extern unsigned long GetNextRegistryNumber(void);
//extern void WriteToQueueFile(const QueueBufferClass &, char *);

	void 
AddTime(char *dstring)
	{
	SYSTEMTIME stTime;

 	// get local time into a SYSTEMTIME structure
	GetLocalTime(&stTime);

	// add 1 day = 24 hrs * 60 min * 60 sec.
	AddTimeSeconds(stTime, 24 * 60 * 60);

    sprintf(dstring, 
			"%02d/%02d/%4d  %02d:%02d:%02d", 
			stTime.wMonth, 
			stTime.wDay,
			stTime.wYear,
			stTime.wHour,
			stTime.wMinute,
			stTime.wSecond);
	}


	void
TestSendAlphaPage1(void)
	{
//	QueueBufferClass QBC;

	CString csModemNumber = ",9,1,8777208398";
	CString csPinNumber = "8772411322";
	CString csAlphaMessage = "Pls Call --- ¿doñde esta el baño?";
	CString csSequenceNumber;

	csSequenceNumber.Format("-[%d]", GetNextRegistryNumber());

//	QBC.SetInitiatedDate("2005-03-03");
//	QBC.SetInitiatedTime("23:59:59");
//	QBC.SetPageType("57");
//	QBC.SetPageStatus("R");
//	QBC.SetProfileId("  KG9999");
//	QBC.SetPagerId("9999");
//	QBC.SetInitiatorProfileId("9999");

//	QBC.SetPageInfo(csModemNumber + "+" + csPinNumber + "+" + csAlphaMessage + csSequenceNumber);
//	WriteToQueueFile(QBC, "57");
	}


	// without 9 for outside line
	void
TestSendAlphaPage2(void)
	{
//	QueueBufferClass QBC;

	CString csModemNumber = ",1,8777208398";
	CString csPinNumber = "8772411322";
	CString csAlphaMessage = "Pls Call - QuickSilver Girl.";
	CString csSequenceNumber;

	csSequenceNumber.Format("-[%d]", GetNextRegistryNumber());

//	QBC.SetInitiatedDate("2005-03-04");
//	QBC.SetInitiatedTime("23:59:59");
//	QBC.SetPageType("57");
//	QBC.SetPageStatus("R");
//	QBC.SetProfileId("  KG9999");
//	QBC.SetPagerId("9999");
//	QBC.SetInitiatorProfileId("9999");
//	QBC.SetPageInfo(csModemNumber + "+" + csPinNumber + "+" + csAlphaMessage + csSequenceNumber);

//	WriteToQueueFile(QBC, "57");
	}


	void
TestSendListPage1(void)
	{
//	QueueBufferClass QBC;

	CString csNumericMessage = "8005551212";
	CString csAlphaMessage = "List-Page - Come Quickly.";

//	QBC.SetInitiatedDate("2005-03-08");
//	QBC.SetInitiatedTime("23:59:59");
//	QBC.SetPageType("21");
//	QBC.SetPageStatus("L");
//	QBC.SetProfileId("  KG2121");
//	QBC.SetPagerId("2121");
//	QBC.SetInitiatorProfileId("2121");
//	QBC.SetPageInfo(csNumericMessage + "+" + csAlphaMessage);
//
//	WriteToQueueFile(QBC, "21");
	}


	void
TestSendListPage2(void)
	{
	}


	void
TestSendListPage3(void)
	{
	}



	void
TestSendListPage4(void)
	{
		/*
	TestSendListPage3();
	TestSendListPage3();
	TestSendListPage3();
	TestSendListPage3();

	TestSendListPage3();
	TestSendListPage3();
	TestSendListPage3();
	TestSendListPage3();
	*/
	}


	void
TestSendListPage5(void)
	{
		/*
	TestSendListPage3();
	TestSendListPage3();
	TestSendListPage1();
	TestSendListPage2();
	*/
	}
