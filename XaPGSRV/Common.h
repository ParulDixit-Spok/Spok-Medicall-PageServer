
// common.h


// disable warning regarding changing structure packing, 
//  8 byte boundary for COM and MSADO vs 1 byte boundary for dialogic,
//  and config file structures written directly to parameter data files.
#pragma warning ( disable: 4103 )  
#pragma warning ( disable: 4514 )  

#include "resource.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


// for CString class etc.  this needs to be before windows.h
//#include <afx.h>
#include <atlstr.h>

//#include <io.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <tchar.h>
//#include <process.h>    // _beginthreadex


//The only type def in the pervasive include files required to compile
typedef short int             BTI_SINT;

#include "srllib.h"
#include "dxxxlib.h"
#include "dtilib.h"
#include "sctools.h"


// common stuff PageWin4, and PageWin3
#include "Structs43.h"
//#include "Append2File.h"
//#include "Version.h"
//#include "Checksum.h"
#pragma once

//#include "IniFilesReadWrite.h"

//#include "QueueFileManager.h"

#include "definesS.h"
#include "defines.h"
#include "xtpgsrv.h"
#include "enumsS.h"
#include "enums.h"
#include "structsS.h"
#include "structs.h"

//#include "DataBaseAccessor.h"
//#include "SpoolCollection.h"
//#include "Impairment.h"
#include "globals.h"
// #include <vld.h>

// software programming references (bibliography).
//
//[Richter99]		"Programming Applications for Microsoft Windows fourth edition", 
//					Jeffrey Richter,
//					Microsoft Press, 1999,
//					ISBN 1-57231-996-8
//
//[Prosise99]		"Programming Windows with MFC second edition",
//					Jeff Prosise,
//					Microsoft Press, 1999,
//					ISBN 1-57231-695-0
//							
//[Beveridge99]		"Multithreading Applications in Win32",
//					Jim Beveridge, and Robert Wiener,
//					Addison-Wesley, 1997,
//					ISBN 0-201-44234-5(pb)
//							
//[Stroustrup97]	"The C++ Programming Language third edition",
//					Bjarne Stoustrup,
//					Addison-Wesley, 1997,
//					ISBN 0-201-88954-4
//
//[Kernighan88]		"The C Programming Language second edition",
//					Brian W. Kernighan, and Dennis M. Ritchie,
//					Prentice Hall P T R, 1988,
//					ISBN 0-13-110362-8 (pbk.)
//
//[Kruglinski98]	"Programming Microsoft Visual C++ fifth edition",
//					David J. Kruglinski, George Shepherd, and Scot Wingo,
//					Microsoft Press, 1998,
//					ISBN 1-57231-857-0
//							
//[Giannini01]		"Windows Programming Programmers Notebook",
//					Mario Giannini, and Jim Keogh,
//					Prentice Hall P T R, 2001,
//					ISBN 0-13-027845-9
//							
//[Gamma95]			"Design Patterns: elements of reusable object-oriented software",
//					Erich Gamma, Richard Helm, Ralph Johnson, Jojn Vlissides,
//					Addison-Wesley, 1995,
//					ISBN 0-201-63361-2
//							
//[Rector00]		"Win32 Programming revised printing",
//					Brent E. Rector, and Joseph M. Newcomer,
//					Addison-Wesley Longman, 2000,
//					ISBN 0-201-63492-9
//
//[Robbins00]		"Debugging Applications",
//					John Robbins,
//					Microsoft Press, 1998,
//					ISBN 0-7356-0886-5
//
//[Quinn96]			"Windows Sockets Network Programming",
//					Bob Quinn, and Dave Shute,
//					Addison-Wesley, 1996,
//					ISBN 0-201-63372-8
//
//[Musser01]		"STL Tutorial and Reference Guide second edition",
//					David R. Musser, Gillmer J. Derge, Atul Saini,
//					Modena Software, 2001,
//					ISBN 0-201-37923-6
//
//[Box98]			"Essential COM",
//					Don Box,
//					Addison-Wesley Longman, 1998,
//					ISBN 0-201-63446-5 (alk.paper)
//
//[Nelson98]		"Serial Communications Developer's Guide second edition",
//					Mark Nelson,
//					M&T Books, 1998,
//					ISBN 0-7645-4570-1 (alk.paper)
//
//[Templeman98]		"Beginning Windows NT Programming",
//					Julian Templeman,
//					Wrox Press Ltd, Birmingham, UK, 1998,
//					ISBN 1-861000-17-0 
//
//[Comer97]			"Internetworking With TCP/IP volume III",
//					Douglas E. Comer, and David L. Stevens,
//					Prentice-Hall, Inc. 1997,
//					ISBN 0-13-848714-6 
//
//[Snader00]		"Effective TCP/IP Programming (44 tips)",
//					Jon C. Snader,
//					Addison-Wesley, 2000,
//					ISBN 0201615894 
//
