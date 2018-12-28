
// Append2File.h: interface for the log class.


#if !defined(AFX_LOG_H__854EEF6C_06CB_4E54_A8EE_AF176FE4921C__INCLUDED_)
#define AFX_LOG_H__854EEF6C_06CB_4E54_A8EE_AF176FE4921C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <atlstr.h>


// typical usage for the Append2File class.
//
// global scope file logging object.
//  class Append2File g_MessageLog;

// access to global object.
//  extern Append2File g_MessageLog;
//
// include file for modules that need access.
//  #include Append2File.h
//
//	g_AbcLog.SetProperty(LOG_SET_DIRECTORY, (CString) "LOG");
//	g_AbcLog.SetProperty(LOG_SET_ENABLE_LOGGING, (CString) "TRUE");
//	g_AbcLog.SetProperty(LOG_SET_FILE_EXTENSION, (CString) "TXT");
//	g_AbcLog.SetProperty(LOG_SET_BASE_FILENAME, (CString) "LOG_");
//	g_AbcLog.SetProperty(LOG_SET_MAX_FILES_IN_ROTATION, 5);
//	g_AbcLog.SetProperty(LOG_SET_MAX_FILE_SIZE, 1000000);
//	g_AbcLog.CreateSubdirectory();
//	g_AbcLog.LogMessage((CString) "First Message");


// XX lines of text are buffered before writing to 
//  the log file, this is done to improve performance,
//  as each "write" or "flush" requires open, write and, close
//  operations on the file.
//
#define MESSAGE_BUFFER_SIZE		100

enum 
	{
	LOG_OPTION_DATE,
	LOG_OPTION_RAW, 
	LOG_OPTION_CRLF, 
	LOG_OPTION_SQUARE_BRACKETS,
	};



// a class to Append to a file.
class Append2File
	{
	public:
		// public interface
		Append2File();
		void SetProperty(int, int);
		void SetProperty(int, ATL::CString);
		ATL::CString GetFileAndPath(void);
		void CreateSubdirectory(void);
		void LogMessage(char *, int);
		void LogMessage(ATL::CString, int);
		void FlushMessagesToDisk(int);
		~Append2File();

	private:
		// private functions
		void BuildFileNameFromFileSequenceNumber(int);
		void DetermineNextFilenameToUse(void);
		void WriteData(ATL::CString);

		// properties
		ATL::CString m_csDirectoryName;
		ATL::CString m_csBaseFilename;
		ATL::CString m_csFileExtension;
		int m_iMaxLogFileSize;
		int m_iNumberOfFilesInRotation;
		BOOL m_bEnableLogging;

		// used between methods
		BOOL m_bPathAndFileNameIsOkToUse;
		ATL::CString m_csPathAndFileName;

		// buffered messages, so that the file can be 
		//  opened and closed less often.
		int m_iMessageCount;
		ATL::CString m_csMessage[MESSAGE_BUFFER_SIZE];
		bool m_bUseSequenceNumber;

	public:
		enum 
			{
			LOG_SET_ENABLE_LOGGING,
			LOG_SET_DIRECTORY,
			LOG_SET_BASE_FILENAME, 
			LOG_SET_FILE_EXTENSION, 
			LOG_SET_MAX_FILE_SIZE, 
			LOG_SET_MAX_FILES_IN_ROTATION,
			LOG_SET_USE_SEQUENCE_NUMBER,
			};

		enum
			{
			FINALIZE_CURRENT_FILENAME,
			RETAIN_CURRENT_FILENAME,
			};
	};

#endif // !defined(AFX_LOG_H__854EEF6C_06CB_4E54_A8EE_AF176FE4921C__INCLUDED_)

