
// SpoolCollection.cpp


#include "common.h"
//#include "SpoolCollection.h"
#include <winspool.h>

// disable deprecation
#pragma warning(disable:4996)


	void
SpoolCollection::FindPrintersOfType(PrinterType PrinterType)
	{
	int iType;
	
	// find all, but accept only up to N_SPOOLER_ITEMS printer items.
	DWORD dwSize = 0;
	DWORD dwPrinters = 0;

	// set mask to be used by EnumPrinters() call to get the
	//  desired type of printers.
	//
	if(PrinterType_ALL == PrinterType)
		{
		// all printers.
		iType = PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS;
		}
	else if(PrinterType_NETWORK == PrinterType)
		{
		// only "Connected" network printers.
		iType = PRINTER_ENUM_CONNECTIONS;
		}
	else
		{
		// this is the "local" printers group,
		//  or the default group.
		iType = PRINTER_ENUM_LOCAL;
		}

	// this is only to find out the size required for 
	//  the buffer (dwSize).
	::EnumPrinters(iType,
					NULL,
					4,
					NULL,
					0,
					&dwSize,
					&dwPrinters);

	// allocate an appropriately sized buffer;

	BYTE * pBuffer = new BYTE[dwSize];

	::EnumPrinters(iType,
					NULL,
					4,
					pBuffer,
					dwSize,
					&dwSize,
					&dwPrinters);

	// check number of printers found.
	//
	if(dwPrinters > 0)
		{
		// some printer(s) found. 

		// determine the number of printers that we will serivce.
		//
		if(dwPrinters > N_SPOOLER_ITEMS)
			{
			// use a limited number of printers.
			dwPrinters = N_SPOOLER_ITEMS;
			}
		else
			{
			// use actual number of enumerated printers.
			//  already in place.
			}

		// step thru printer names.
		//  capture the name, and set the "Defined" flag.
		//
		PRINTER_INFO_4 * pPrnInfo = (PRINTER_INFO_4 *) pBuffer;

		for(UINT i = 0; i < dwPrinters; i++)
			{
			CString csTemp; csTemp = pPrnInfo->pPrinterName;

			SU[i].SetPrinterName(csTemp);
			SI[i].bDefined = true;

			// advance to next PRINTER_INFO_4 structure in the buffer.
			pPrnInfo++;
			}
		}
	else
		{
		// ZERO printers found.
		}

	delete[] pBuffer;
	}


	void
SpoolCollection::SetLinesPerPage(int Lines)
	{
	for(int i = 0; i < N_SPOOLER_ITEMS; i++)
		{
		if(SI[i].bDefined)
			{
			SU[i].SetLinesPerPage(Lines);
			}
		}
	}


	void
SpoolCollection::SetLeftMargin(int LeftMargin)
	{
	for(int i = 0; i < N_SPOOLER_ITEMS; i++)
		{
		if(SI[i].bDefined)
			{
			SU[i].SetLeftMargin(LeftMargin);
			}
		}
	}


	void
SpoolCollection::SetTopMargin(int TopMargin)
	{
	for(int i = 0; i < N_SPOOLER_ITEMS; i++)
		{
		if(SI[i].bDefined)
			{
			SU[i].SetTopMargin(TopMargin);
			}
		}
	}


	void
SpoolCollection::AddLine(CString Text)
	{
	// add the given line of text to all listed SpoolUtil items
	//  that are currently in the collection.
	for(int i = 0; i < N_SPOOLER_ITEMS; i++)
		{
		if(SI[i].bDefined)
			{
			// the item is defined...
			//  call the AddLine() method on the item.
			SU[i].AddLine(Text);
			}
		}
	}




	void SpoolCollection::CancelledPagePrinting(int PrintOption, char  *lpMessage, ...)
	{
		// FIX THIS TO NOT OVERRUN THE BUFFER.
		CString csTemp;
		va_list marker;
		char cBuf[1024];
		CString csHr('-', 65);	// "horizontal rule", a string of XX '-' chars.
		
		// deal with variable number of arguments, put it all in one string, 
		//  so that it can be easily passed to downstream functions.
		//
		if(lpMessage != NULL)
		{
			memset(cBuf, 0, sizeof(cBuf));
			
			va_start(marker, lpMessage);
			vsprintf(cBuf, lpMessage, marker);
			va_end(marker);
			
			csTemp = cBuf;
		}
		// use the global SpoolCollection object 
		//  reserved for cancelled-page reports.
		switch(PrintOption)
		{
		case START_PAGE:
			FindPrintersOfType(SpoolCollection::PrinterType_ALL);
			
			SetTopMargin(3);
			SetLeftMargin(6);
			SetLinesPerPage(55);
			break;
			
		case HEADER_1:
			AddLine(csHr);
			AddLine(csTemp);
			break;
			
		case HEADER_2:
			AddLine(csTemp);
			break;
			
		case HEADER_3:
			AddLine(csTemp);
			AddLine(csHr);
			AddLine(" ");
			break;
			
		case MID_PAGE:
			AddLine(csTemp);
			break;
			
		case END_PAGE:
			AddLine(" ");
			Close();
			break;
			
		default:
			// unknown option, ignored.
			break;
		}
	}
	




	void
SpoolCollection::Close(void)
	{
	for(int i = 0; i < N_SPOOLER_ITEMS; i++)
		{
		if(SI[i].bDefined)
			{
			// if the item is defined...
			//  undefine it, and close it.
			SI[i].bDefined = false;
			SU[i].Close();
			}
		}
	}


SpoolCollection::~SpoolCollection()
	{
	this->Close();
	}

