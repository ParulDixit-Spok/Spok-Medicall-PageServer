// disable deprecation
#pragma warning(disable:4996)

// SpoolUtil.cpp

#include "common.h"
//#include "SpoolUtil.h"
#include <winspool.h>		// for Windows Spooler functions

#include <atlbase.h>
#include <atlconv.h>
#include <winbase.h>


SpoolUtil::SpoolUtil()
	{
	m_bOkToSpool = false;
	m_hPrinter = NULL;
	m_LineCount = 0;
	m_csPrinterName = "";
	m_iLinesPerPage = 55;
	m_iLeftMargin = 6;
	m_iTopMargin = 3;
	}


	void
SpoolUtil::SetPrinterName(CString PrinterName)
	{
	m_csPrinterName = PrinterName;
	}


	void
SpoolUtil::SetLinesPerPage(int Lines)
	{
	m_iLinesPerPage = Lines;
	}


	void
SpoolUtil::SetLeftMargin(int LeftMargin)
	{
	m_iLeftMargin = LeftMargin;
	}


	void
SpoolUtil::SetTopMargin(int TopMargin)
	{
	m_iTopMargin = TopMargin;
	}


	CString
SpoolUtil::GetPrinterName(void)
	{
	return m_csPrinterName;
	}


	// a public method to close the spooler is available.
	void
SpoolUtil::Close(void)
	{
	// end the page, and document.
	//
	if(m_LineCount != 0)
		{
		// only send a formfeed, if the line count is non-zero
		//  this prevents a blank page being ejected.
		SendToStream("\f");
		}

	m_LineCount = 0;
	m_bOkToSpool = false;
	if(m_hPrinter != NULL)
	{
		EndPagePrinter(m_hPrinter);
		EndDocPrinter(m_hPrinter);
		ClosePrinter(m_hPrinter);
	}
	}


	void
SpoolUtil::SendToStream(CString csText)
	{
	DWORD dwBytesWritten;

	// note the use of CString GetBuffer(), and ReleaseBuffer() to deliver
	//  c style character array pointer "strings"
	WritePrinter(m_hPrinter, 
					csText.GetBuffer(0), 
					csText.GetLength(), 
					&dwBytesWritten);

	csText.ReleaseBuffer();
	}


	void
SpoolUtil::AddLine(CString csText)
	{
	// if the spooler is not open, attempt to open it.
	if(!m_bOkToSpool)
		{
		int iStatus = OpenPrinter((LPTSTR) (LPCTSTR) m_csPrinterName, 
									&m_hPrinter, 
									NULL);

		if(iStatus > 0)
			{
			// opened successfully, set the flag.
			//
			m_bOkToSpool = true;

			// prepare the DOC_INFO_1 structure.
			//
			DOC_INFO_1 di;
			::ZeroMemory(&di, sizeof(DOC_INFO_1));

			di.pDocName = "PageServer Report";
			di.pOutputFile = NULL;		// was: "RAW";
			di.pDatatype = "TEXT";

			// start the document, and the very first page.
			//
			StartDocPrinter(m_hPrinter, 1, (LPBYTE) &di);
			StartPagePrinter(m_hPrinter);

			// prepare left margin "spaces" variable.
			m_csLeftMarginString = "";

			for(int i = 0; i < m_iLeftMargin; i++)
				{
				m_csLeftMarginString += " ";
				}
			}
		}

	// attempt to write.
	if(m_bOkToSpool)
		{
		// send "top margin lines" at the count of zero
		if(m_LineCount == 0)
			{
			// add XX blank lines at top.  Top margin.
			for(int i = 0; i < m_iTopMargin; i++)
				{
				SendToStream("\r\n");
				}
			}

		// increase lines per page...
		m_LineCount++;

		// send text to the output stream.
		CString messageLine; messageLine = m_csLeftMarginString + csText;
		SendToStream(messageLine);

		// if first printer, printerNumber = 0, save string for
		//  email report

		// followed by <cr> and <lf>. the csText should have 
		//  only a message "string"
		SendToStream("\r\n");

		if(m_LineCount >= m_iLinesPerPage)
			{
			// at line count limit, 
			//
			//  send a formfeed character.
			//
			//  close the current page, and then open a new spooler "page".
			//  also, set the line count to zero.
			//
			SendToStream("\f");

			EndPagePrinter(m_hPrinter);
			StartPagePrinter(m_hPrinter);

			m_LineCount = 0;
			}
		}
	}


SpoolUtil::~SpoolUtil()
	{
	// close the spooler, at object destruction.
	Close();
	}

