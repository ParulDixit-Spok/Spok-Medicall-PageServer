
// SpoolCollection.h


// a class to create, contain and manage 
//  multiple SpoolUtil instances

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <afx.h>
#include <atlstr.h>
#include "SpoolUtil.h"
#include "defines.h"
#include <memory>

// a group of up to XX items.
#define N_SPOOLER_ITEMS 5


class SpoolCollection
	{
	public:
		enum PrinterType
			{
			PrinterType_ALL,
			PrinterType_NETWORK,
			PrinterType_LOCAL,
			};

		void FindPrintersOfType(PrinterType);

		void SetLinesPerPage(int);
		void SetLeftMargin(int);
		void SetTopMargin(int);

		void AddLine(CString);
		void CancelledPagePrinting(int PrintOption, char  *lpMessage, ...);

		void Close(void);
		~SpoolCollection();

	private:
		struct SpoolerInfo
			{
			bool bDefined;
			CString PrinterName;
			};

		SpoolerInfo SI[N_SPOOLER_ITEMS];
		SpoolUtil SU[N_SPOOLER_ITEMS];
	};
