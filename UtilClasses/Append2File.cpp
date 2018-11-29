

// Append2File.cpp: implementation of the message logging class.
// disable deprecation
#pragma warning(disable:4996)


//#include "afx.h"
#include "Append2File.h"

//#include <atlstr.h>

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif

using namespace ATL;

// class to Append to a File

Append2File::Append2File()
	{
	// at instance creation, 
	//  signal to find a new filename/number to use.
	m_bPathAndFileNameIsOkToUse = FALSE;
	m_csPathAndFileName = "";

	// make defaults for member variables, there is
	//  no assurance that the user will use any of the provided
	//  setup methods.
	m_csDirectoryName = "Log";
	m_csBaseFilename = "Log_";
	m_csFileExtension = "Txt";
	m_iMaxLogFileSize = 3000;
	m_iNumberOfFilesInRotation = 10;

	m_bEnableLogging = FALSE;
	m_iMessageCount = 0;
	m_bUseSequenceNumber = TRUE;
	}


Append2File::~Append2File()
	{
	// at destruction, flush out any remaining unwritten messages.
	FlushMessagesToDisk(FINALIZE_CURRENT_FILENAME);
	}


	// set integer type options
	void
Append2File::SetProperty(int OptionName, int OptionValue)
	{
	switch(OptionName)
		{
		case LOG_SET_MAX_FILE_SIZE:
			// check for min/max size, must be in-range.
			if(OptionValue > 500000000)
				{
				// maximum size allowed.
				m_iMaxLogFileSize = 500000000;	// set to 500 Mb (about the capacity of CD media)
				}
			else if(OptionValue < 3000)
				{
				// minimum size allowed.
				m_iMaxLogFileSize = 3000;		// set to 3 Kb (about 1 printed page)
				}
			else
				{
				// specified value is in-range.
				m_iMaxLogFileSize = OptionValue;
				}
			break;

		case LOG_SET_MAX_FILES_IN_ROTATION:
			// check for min/max, must be in-range (2  - 10,000).
			//  to stay within the naming pattern, 
			//  and logging design.
			if(OptionValue > 10000)
				{
				// maximum allowed.
				m_iNumberOfFilesInRotation = 10000;	// set to 10,000
				}
			else if(OptionValue < 2)
				{
				// minimum allowed.
				m_iNumberOfFilesInRotation = 2;		// set to 2
				}
			else
				{
				// specified value is in-range.
				m_iNumberOfFilesInRotation = OptionValue;
				}
			break;

		default:
			{
			CString csTemp;

			csTemp.Format("Error: Append2File::SetProperty([%d], int) - Unknown Property!",
							OptionName);
			::MessageBox(NULL, 
							csTemp, 
							"Append2File::SetProperty(int, int)", 
							MB_OK);
			}
			break;
		}
	}


	// set CString type options
	void
Append2File::SetProperty(int OptionName, ATL::CString OptionValue)
	{
	switch(OptionName)
		{
		case LOG_SET_USE_SEQUENCE_NUMBER:
			OptionValue.TrimLeft();
			OptionValue.TrimRight();
			OptionValue.MakeUpper();

			if(OptionValue == "TRUE")
				{
				// set option to true
				m_bUseSequenceNumber = TRUE;
				}
			else
				{
				// set option to false.
				m_bUseSequenceNumber = FALSE;
				}
			break;

		case LOG_SET_BASE_FILENAME:
			m_csBaseFilename = OptionValue;
			break;

		case LOG_SET_FILE_EXTENSION:
			m_csFileExtension = OptionValue;
			break;

		case LOG_SET_DIRECTORY:
			m_csDirectoryName = OptionValue;
			break;

		case LOG_SET_ENABLE_LOGGING:
			OptionValue.TrimLeft();
			OptionValue.TrimRight();
			OptionValue.MakeUpper();

			if(OptionValue == "TRUE")
				{
				m_bEnableLogging = TRUE;
				}
			else
				{
				m_bEnableLogging = FALSE;
				}
			break;

 		default:
			{
			CString csTemp;

			csTemp.Format("Error: Append2File::SetProperty([%d], CString) - Unknown Property!",
							OptionName);
			::MessageBox(NULL, 
							csTemp, 
							"Append2File::SetProperty(int, CString) ", 
							MB_OK);
			}
			break;
		}
	}


	// build a proposed file path and name
	//  from the given numeric value.
	//
	void 
Append2File::BuildFileNameFromFileSequenceNumber(int iNumber)
	{
	// Find the current Drive and Directory
	//  for this application instance (location of the .exe).
	//
	TCHAR pszPath[_MAX_PATH];
	GetModuleFileName(NULL, pszPath, _MAX_PATH);

	TCHAR pszDrive[_MAX_DRIVE];
	TCHAR pszDir[_MAX_DIR];
	_tsplitpath(pszPath, pszDrive, pszDir, NULL, NULL);

	CString csDirectoryName;

	if(!m_csDirectoryName.IsEmpty())
		{
		// subdirectory is not empty, add a leading '\' character.
		csDirectoryName = m_csDirectoryName + "\\";
		}

	CString csSequenceText;

	if(m_bUseSequenceNumber)
		{
		// prepare a 4 digit string.
		csSequenceText.Format("%04d", iNumber);
		}
	else
		{
		// make it empty.
		csSequenceText = "";
		}

	// build up the complete path and filename.
	m_csPathAndFileName.Format("%s%s%s%s%s.%s", 
								pszDrive,				// drive letter of application.
								pszDir,					// subdirectory where application exe is located.
								csDirectoryName,		// "LOG" subdirectory name. may be empty, 
														//  or contain a subdirectory name with a 
														//  trailing '\'  character e.g. "LOG\".
								m_csBaseFilename,		// base filename.
								csSequenceText,			// text containing the sequence number.
								m_csFileExtension);		// file extension.	
	}


	// attempt to find unused filename
	//  within the numeric filename pattern criteria.
	//  e.g. files in rotation is 5, 
	//       base filename is "LOG_"
	//       extension is "TXT"
	//       then, filenames LOG_0000.TXT, 
	//						 LOG_0001.TXT, 
	//						 LOG_0002.TXT, 
	//						 LOG_0003.TXT, 
	//				 	 and LOG_0004.TXT will be used.
	//  if they all exist, then the oldest one will be truncated
	//  to zero length, and will be re-used.
	//
	void 
Append2File::DetermineNextFilenameToUse(void)
	{
	HANDLE fh;
	int iFileNumberOfOldestFileFound = 0;

	// set filename to be "invalid"
	m_bPathAndFileNameIsOkToUse = FALSE;

	if(m_bUseSequenceNumber)
		{
		for(int N = 0; N < m_iNumberOfFilesInRotation; N++)
			{
			// build up a filename from the base file name, number, and extension.
			BuildFileNameFromFileSequenceNumber(N);

			// create the file if it doesn't exist,
			//  if successfully created, we have found an avaialble filename.
			fh = CreateFile(m_csPathAndFileName,
								GENERIC_WRITE | GENERIC_READ,
								FILE_SHARE_READ | FILE_SHARE_WRITE,
								NULL,
								CREATE_NEW,			
								FILE_ATTRIBUTE_NORMAL,
								NULL);

			// when using option CREATE_NEW, 
			//  the function will only succeed if file has just been created.

			// check to see if the file was just created.
			if(fh != INVALID_HANDLE_VALUE)
				{
				// instant success, created a new empty file.

				// remember to close the file.
				CloseHandle(fh);

				// set flag variable to indicate that it is ok
				//  to write to the prepared path and filename.
				m_bPathAndFileNameIsOkToUse = TRUE;

				return;
				}

			// file exists, keep looking.
			}
		// search for unused filename is complete, and
		//  no unused filename was found. 

		// since there are no available unused filenames, 
		//  find the oldest one, and truncate it to 0 bytes.
		//
		WIN32_FILE_ATTRIBUTE_DATA FileData;
		iFileNumberOfOldestFileFound = 0;

		LARGE_INTEGER liFileData = {0}, liOldestSoFar = {0};
		__int64 i64FileData = 0, i64OldestSoFar = 0;

		for(int i = 0; i < m_iNumberOfFilesInRotation; i++)
			{
			// scan all filenames for the oldest one. 
			BuildFileNameFromFileSequenceNumber(i);
			
			// retrieve FileTime attribute.
			GetFileAttributesEx(m_csPathAndFileName, 
								GetFileExInfoStandard,			// GET_FILEEX_INFO_LEVELS, 
								&FileData);
			if(i == 0)
				{
				// to start, take the filetime from the first file 
				//  as the "oldest" so far. obviously, the first 
				//  and only file would indeed be the 
				//  oldest one when only 1 is considered.
				//  it may soon be replaced.
				//
				liOldestSoFar.HighPart = FileData.ftLastWriteTime.dwHighDateTime;
				liOldestSoFar.LowPart  = FileData.ftLastWriteTime.dwLowDateTime;

				iFileNumberOfOldestFileFound = 0;

				// first one captured, no compare required.
				continue;
				}

			// put into LARGE_INTEGER structure for subsequent i64 compare.
			liFileData.HighPart = FileData.ftLastWriteTime.dwHighDateTime;
			liFileData.LowPart  = FileData.ftLastWriteTime.dwLowDateTime;

			// assign LARGE_INTEGER to i64 data type, for comparison.
			i64FileData    = liFileData.QuadPart;
			i64OldestSoFar = liOldestSoFar.QuadPart;

			if(i64FileData <= i64OldestSoFar)
				{
				// an older value has been found.
				//  take this as the "oldest so far".
				liOldestSoFar.HighPart = FileData.ftLastWriteTime.dwHighDateTime;
				liOldestSoFar.LowPart  = FileData.ftLastWriteTime.dwLowDateTime;

				// capture value of i as the oldest so far.
				iFileNumberOfOldestFileFound = i;
				}
			}
		}
	else
		{
		// not using a sequence number... dummy one up,
		//  it will be ignored later anyhow.
		iFileNumberOfOldestFileFound = 0;
		}


	// the sequence number of the oldest is now known.
	//  the path and filename of the oldest file 
	//  can now be built up from the captured sequence number.
	BuildFileNameFromFileSequenceNumber(iFileNumberOfOldestFileFound);

	// truncate this file to 0 bytes.
	fh = CreateFile(m_csPathAndFileName,
					GENERIC_WRITE | GENERIC_READ,
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					NULL,
					TRUNCATE_EXISTING,		
					FILE_ATTRIBUTE_NORMAL,
					NULL);

	// check to see if the file was successfully opened, and truncated, 
	//  close it if it was successfully opened.
	if(fh != INVALID_HANDLE_VALUE)
		{
		// remember to close the file.
		CloseHandle(fh);

		// set flag variable to indicate that it is ok
		//  to write to the prepared path and filename.
		m_bPathAndFileNameIsOkToUse = TRUE;
		}
	}


	void
Append2File::CreateSubdirectory(void)
	{
	if(!m_bEnableLogging)
		{
		// logging is not enabled.
		return;
		}

	if(m_csDirectoryName.IsEmpty())
		{
		// subdirectory is empty.
		return;
		}

	// Find the current Drive and Directory
	//  for this application instance (location of the .exe).
	//
	TCHAR pszPath[_MAX_PATH];
	GetModuleFileName(NULL, pszPath, _MAX_PATH);

	TCHAR pszDrive[_MAX_DRIVE];
	TCHAR pszDir[_MAX_DIR];
	_tsplitpath(pszPath, pszDrive, pszDir, NULL, NULL);

	CString csPath;

	// build up the complete subdirectory name.
	csPath.Format("%s%s%s", 
					pszDrive,				
					pszDir,					
					m_csDirectoryName);

	// create the subdirectory.
	CreateDirectory(csPath, NULL);
	}


	void
Append2File::FlushMessagesToDisk(int Option)
	{
	if(!m_bEnableLogging)
		{
		// logging is not enabled.
		return;
		}

	// if there are no messages to flush, 
	//  a new name is not required,
	//  no file needs to be created,
	//  no messages need to be written.
	// this prevents an empty file from being created
	//  when none is necessary.
	//
	if(0 == m_iMessageCount)
		{
		return;
		}

	// see if a "new" filename should be built.
	if(m_bPathAndFileNameIsOkToUse)
		{
		// the current path and filename
		//  contained in variable m_csPathAndFileName
		//  is ok for message logging, has not been 
		//  invalidated on a previous message write, 
		//  due to file size "limit".
		}
	else
		{
		// prepare a "new" or re-cycled filename,
		//  the previous write went over the limit 
		//  of maximum desired filesize, 
		//  or this is the first message after application startup.
		DetermineNextFilenameToUse();
		}

	// append message to the file.
	HANDLE fh = CreateFile(m_csPathAndFileName,
						GENERIC_WRITE | GENERIC_READ,
						FILE_SHARE_READ | FILE_SHARE_WRITE,
						NULL,
						OPEN_ALWAYS,
						FILE_ATTRIBUTE_NORMAL,
						NULL);

	// move file pointer to the end of file, 
	//  so that new information will be "appended".
	SetFilePointer(fh, 0, NULL, FILE_END);


	// write all accumulated messages.
	for(int i = 0; i < m_iMessageCount; i++)
		{
		// write message.
		unsigned long nBytesWritten;	// dummy variable.

		WriteFile(fh,
					 LPCTSTR(m_csMessage[i]),
					 m_csMessage[i].GetLength(),
					 &nBytesWritten, 
					 NULL);
		}

	// close the file.
	CloseHandle(fh);

	// start the count back at zero.
	m_iMessageCount = 0;

	if(FINALIZE_CURRENT_FILENAME == Option)
		{
		// force a new filename to be used next time.
		m_bPathAndFileNameIsOkToUse = FALSE;

		return;
		}

	if(RETAIN_CURRENT_FILENAME == Option)
		{
		// check to see if the write, just completed,
		//  has caused the file size to exceed the limit.
		//  if exceeded, close the file.  when a new message is
		//  to be written, a file will be be opened in
		//  the normal course of events.

		WIN32_FILE_ATTRIBUTE_DATA FileData;

		// retrieve FileTime attribute.
		GetFileAttributesEx(m_csPathAndFileName, 
							GetFileExInfoStandard,		// GET_FILEEX_INFO_LEVELS 
							&FileData);

		// we expect that the file will never be > 4 gigabytes
		//  the nFileSizeHigh is expected to always be == 0.
		if((FileData.nFileSizeLow >= (unsigned) m_iMaxLogFileSize) || 
			(FileData.nFileSizeHigh != 0))
			{
			// signal the current file sequence number, 
			//  and path and filename as "invalid"
			//  because the size has gone over the acceptable limit.
			//  the next write request will take appropriate action
			//  to determine a new path and filename to use.
			m_bPathAndFileNameIsOkToUse = FALSE;
			}
		else
			{
			// nothing more to do, the filesize was not exceeded.

			// DIRTY FIX... INDICATE THAT THE FILENAME IS OK
			//  FOR USER-SELECTED NON-SEQUENCED FILENAMES.
			//  SEE IF IT WORKS.
			m_bPathAndFileNameIsOkToUse = TRUE;

			}
		}
	}


	void
Append2File::WriteData(CString csMessage)
	{
	// if message has zero length, nothing to do.
	if(csMessage.GetLength() <= 0)
		{
		return;
		}

	// accept the given string, make a simple determination 
	//  about whether the file should be opened, written, and then closed, 
	//  based on how many "strings" have been received since "last write".
	//
	// message elements are 0 thru (MESSAGE_BUFFER_SIZE - 1)
	// the message count is 1 thru MESSAGE_BUFFER_SIZE.

	// always add the given message to the array.
	m_csMessage[m_iMessageCount] = csMessage;
	m_iMessageCount++;

	// then decide if it is "time to write"
	if(m_iMessageCount < MESSAGE_BUFFER_SIZE)
		{
		// not enough items received yet.
		}
	else
		{
		// time to write...
		FlushMessagesToDisk(RETAIN_CURRENT_FILENAME);
		}
	}


	// when raw mode is used, length must be specified
	void
Append2File::LogMessage(char * Message, int iOption)
	{
	CString csTemp;

	if(!m_bEnableLogging)
		{
		// logging is not enabled.
		return;
		}

	switch(iOption)
		{
		case LOG_OPTION_RAW:
			csTemp = Message;

			WriteData(csTemp);
			break;

		default:
			{
			CString csTemp;

			csTemp.Format("Append2File::LogMessage(char *, int, int): Unknown option [%d]", 
							iOption);

			::MessageBox(NULL, 
							csTemp, 
							"Append2File::LogMessage(int, int)", 
							MB_OK);
			}
			break;
		}
	}


	void
Append2File::LogMessage(CString csMessage, int iOption)
	{
	CString csTemp;
	SYSTEMTIME st;

	if(!m_bEnableLogging)
		{
		// logging is not enabled.
		return;
		}

	// based on the selected formatting option, 
	//  write data to file as requested.
	switch(iOption)
		{
		case LOG_OPTION_DATE:
			// write date, message, and <cr> and <lf>.
			GetLocalTime(&st);	

			// show milliseconds timing resolution.
			csTemp.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d [%s]\r\n",
							st.wYear,
							st.wMonth,
							st.wDay,
							st.wHour,
							st.wMinute,
							st.wSecond,
							st.wMilliseconds,
							csMessage);
			break;

		case LOG_OPTION_SQUARE_BRACKETS:
			// write message, inside square brackets, followed by <cr> and <lf>.
			csTemp.Format("[%s]\r\n", csMessage);
			break;

		case LOG_OPTION_CRLF:
			// write message, followed by <cr> and <lf>.
			csTemp.Format("%s\r\n", csMessage);
			break;

		case LOG_OPTION_RAW:
			// write message, no additional formatting
			csTemp.Format("%s", csMessage);
			break;

		default:
			{
			CString csTemp;

			csTemp.Format("Append2File::LogMessage(CString & csMessage, int iOption): Unknown option [%d]",
							iOption);

			::MessageBox(NULL, 
							csTemp, 
							"Append2File::LogMessage(CString, int)", 
							MB_OK);
			}
			break;
		}

	WriteData(csTemp);
	}


	CString
Append2File::GetFileAndPath(void)
	{
	return m_csPathAndFileName;
	}


