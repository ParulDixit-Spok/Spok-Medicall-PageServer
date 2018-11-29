
// SpoolUtil.h


// a helper class to support writing to the windows 
//  Print-Spooler for a single printer.

#ifndef _SPOOL_UTIL_H_
#define _SPOOL_UTIL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <afx.h>
#include <atlstr.h>
#include <string>


class SpoolUtil
	{
	public:
		SpoolUtil();
		void SetPrinterName(CString);
		void SetLinesPerPage(int);
		void SetLeftMargin(int);
		void SetTopMargin(int);
		CString GetPrinterName(void);
		virtual void AddLine(CString);

		virtual void Close(void);
		virtual ~SpoolUtil();

	protected:	
		// properties
		int m_iLinesPerPage;
		int m_iLeftMargin;
		int m_iTopMargin;

	private:
		virtual void SendToStream(CString);

	protected:
		// working variables
		bool m_bOkToSpool;
		CString m_csPrinterName;
		HANDLE m_hPrinter;
		int m_LineCount;
		CString m_csLeftMarginString;
	};

#endif