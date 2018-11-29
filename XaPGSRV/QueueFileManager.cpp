
// disable deprecation
#pragma warning(disable:4996)

// QueueFileManager.cpp: implementation of the class.

//#include "afx.h"
#include "common.h"
#include <atlstr.h>
#include "QueueFileManager.h"

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif


#include <stdlib.h>
#include <time.h>
#include <string>

using namespace std;

 
struct QUEUE_HEADER
	{
	short LastRead;
	int dummy1;
	int dummy2;
	short LastWritten;
	int dummy3;
	int dummy4;
	};


QueueFileManager::QueueFileManager(CString Path, CString Filename, int QueueNumber)
	{
	// constructor...
	//
	m_fh = (HANDLE) -1;
	m_Path = Path;
	m_Filename = Filename;
	m_iQueueNumber = QueueNumber;

	m_PathAndFilename = m_Path + "\\" + m_Filename;
	ClearQueueData();
	m_bFileIsOpen = false;
	m_bHeaderIsLocked = false;
	m_LastWritten = -1;
	m_UnclaimedCount = -1;
	ClearErrorInfo();
	}


	void	
QueueFileManager::CaptureFirstError(CString reason)	
	{
	// always increase the error count,
	//  but capture only the first error message.

	if(m_iErrorCount < INT_MAX)
		{
		m_iErrorCount++;

		if(m_iErrorCount == 1)
			{
			// capture only the first error.
			m_csFirstErrorText = reason;
			}
		}
	}


	CString
QueueFileManager::GetTargetFilename(void)
	{
	SYSTEMTIME CurrTime;
	CString TargetFilename;
	int SequenceNumber = 0;
	unsigned long rv;

    GetLocalTime(&CurrTime);
 
    do	{
		// try the next sequential number.
		//  (binary search would be better).
		//
        SequenceNumber++;

        TargetFilename.Format("%s\\R%02d%02d%03d.Q%02d",
								m_Path,
								CurrTime.wMonth,
								CurrTime.wDay,
								SequenceNumber,
								m_iQueueNumber);

        rv = ::GetFileAttributes(TargetFilename);

		} while((rv != INVALID_FILE_ATTRIBUTES) && (SequenceNumber < 999));
 
	return TargetFilename;
	}


	bool
QueueFileManager::KopyFile(CString SourceFilename, CString TargetFilename)
	{
	bool bReturnStatus = false;
	char * pBuffer = NULL;
	HANDLE fhTarget = 0;
	bool bTargetFileIsOpen = false;
	unsigned long NumberOfBytesReadOrWritten = 0;

	try
		{
		// no need to open the source file, it is already open.
		//  and the first 20 bytes are locked, so we know no-one else
		//  will have access to it.
		//

		// open the target file.
		fhTarget = ::CreateFile(TargetFilename,
								GENERIC_READ | GENERIC_WRITE,
								FILE_SHARE_READ | FILE_SHARE_WRITE,
								NULL,
								CREATE_NEW,
								FILE_ATTRIBUTE_NORMAL,
								NULL);

		// check if file create was successful.
		//
		if(fhTarget == INVALID_HANDLE_VALUE)
			{
			// file not opened.
			throw (CString) "unable to create target file";
			}
		bTargetFileIsOpen = true;

		// determine filesize.
		int iFileSize = ::GetFileSize(m_fh, NULL);

		if(INVALID_FILE_SIZE == iFileSize)
			{
			// unable to determine filesize
			throw (CString) "unable to determine filesize";
			}
		else
			{
			// file size was determined.
			}

		// setup to read, starting at byte 0, the first byte of the file.
		//
		int	iStatus = ::SetFilePointer(m_fh, 0, NULL, FILE_BEGIN);

		if(0xFFFFFFFF == iStatus)
			{
			// the value 0xFFFFFFFF indicates an error.
			throw((CString) "  source - SetFilePointer NG");
			}
		else
			{
			// ok.
			}

		// setup to write, starting at byte 0 of the new file. 
		//
		iStatus = ::SetFilePointer(fhTarget, 0, NULL, FILE_BEGIN);

		if(0xFFFFFFFF == iStatus)
			{
			// the value 0xFFFFFFFF indicates an error.
			throw((CString) "  target - SetFilePointer NG");
			}
		else
			{
			// ok.
			}

		#define FILE_COPY_BUFFER_SIZE 1024

		pBuffer = new char[FILE_COPY_BUFFER_SIZE];

		// a little math to determine how many full buffer reads, 
		//  and what is the size of the last, short read, if necessary.

		int iBlockReads = iFileSize / FILE_COPY_BUFFER_SIZE;
		int iLastChunk = iFileSize % FILE_COPY_BUFFER_SIZE;

		// if any "remainder" is present, then the last block read
		//  will be shorter than a full block.
		//
		if(iLastChunk > 0)
			{
			// one more "short" read will be required.
			iBlockReads++;
			}

		int iReadSuccess = 1;
		int iWriteSuccess = 1;

		while((iBlockReads > 0) && iReadSuccess && iWriteSuccess)
			{
			//  read block contents into memory.
			//
			iReadSuccess = ::ReadFile(m_fh, 
										pBuffer, 
										FILE_COPY_BUFFER_SIZE, 
										&NumberOfBytesReadOrWritten, 
										NULL);
			if(iReadSuccess != 0)
				{
				// a block of bytes from the source file was read, 
				//
				}
			else
				{
				throw (CString) "unable to read source file";
				}

			//  write to the new file.
			//
			iWriteSuccess = ::WriteFile(fhTarget,                    
											pBuffer,               
											NumberOfBytesReadOrWritten,     
											&NumberOfBytesReadOrWritten,  
											NULL);
			if(iWriteSuccess != 0)
				{
				// all bytes from the read operation were written to the file.
				//
				}
			else
				{
				throw (CString) "unable to write target file";
				}


			PrintMessage(MSGTYPE_SYSTEM, 
							hwndGeneralMsg, 
							"KopyFile() - Read# [%d] blocksize [%d] bytes: [%d]",
							iBlockReads,
							FILE_COPY_BUFFER_SIZE,
							NumberOfBytesReadOrWritten);	


			iBlockReads--;
			}

		// claim success.
		bReturnStatus = true;
		}
	catch(CString Reason)
		{
		// print reason for failure...
		//
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"QueueFileManager::KopyFile() - catch() [%s]", 
						Reason);

		CaptureFirstError((CString) "QueueFileManager::KopyFile() - [" + Reason + "]");
		}	

	// cleanup operations.
	//
	// close target file if opened.
	//
	if(bTargetFileIsOpen)
		{
		// close it, ignoring any return code.
		//  best efforts attempt.
		//
		bTargetFileIsOpen = false;

		::CloseHandle(fhTarget);
		}

	// deallocate memory if allocated.
	if(pBuffer)
		{
		delete [] pBuffer;
		}

	return bReturnStatus;
	}


	// lock the header record.
	//
	// do the job, or throw.
	//
	void
QueueFileManager::LockHeader(void)
	{
	int iAttempts = 0;
	const int MAX_LOCK_ATTEMPTS = 10;

	// seed the random number generator with the current time.
	//  this is a minimal implementation of random number 
	//  generation, it should be "adequate" to the task.
	//
	srand((unsigned) time(NULL));

	while(iAttempts++ < MAX_LOCK_ATTEMPTS)
		{
		// attempt to lock the header.
		if(::LockFile(m_fh, 0, 0, 20, 0))
			{
			// header locked successfully.
			//
			m_bHeaderIsLocked = true;

			// break out of the while, 
			//  once successful lock is taken.
			break;
			}

		// above XXX attempts report the unsuccessful lock attempt.
		if(iAttempts >= 3)
			{
			// report the inability to lock.
			//
			PrintMessage(MSGTYPE_SYSTEM, 
							hwndGeneralMsg, 
							"QueueFileManager::LockHeader() - trying to lock [%s]... iAttempts [%d]",
							GetPathAndFileName(),
							iAttempts);
			}

		// impose a short delay plus random amount.
		//
		// collect a random number, make the value between 0 and XXX
		//  representing milliseconds to be used as a delay value.
		//
		int iFixedTime = 100;
		int iRandomTime = (rand() % 100);

		Sleep(iFixedTime + iRandomTime);
		}

	// evaluate success or failure.
	if(!m_bHeaderIsLocked)
		{
		// not locked, all attempts used up.
		//

		// throw when unsuccessful.
		throw((CString) "Lock Header NG on: [" + GetPathAndFileName() + "]");
		}
	}
	
	
	// best effort, no throw.
	//
	void
QueueFileManager::UnlockHeader(void)
	{
	// check for successful lock, unlock if necessary.
	//
	if(m_bHeaderIsLocked)
		{
		if(::UnlockFile(m_fh, 0, 0, 20, 0))
			{
			// header unlocked successfully.
			//
			m_bHeaderIsLocked = false;
			}
		else
			{
			PrintMessage(MSGTYPE_SYSTEM, 
							hwndGeneralMsg, 
							"QueueFileManager::UnlockHeader() - unlock NG");

			PrintMessage(MSGTYPE_SYSTEM, 
							hwndGeneralMsg, 
							"value from GetLastError(): [%d]", 
							GetLastError());

			}
		}
	}


	void
QueueFileManager::SetFilePointer(int Offset)
	{
	int	iStatus = ::SetFilePointer(m_fh, Offset, NULL, FILE_BEGIN);

	if(0xFFFFFFFF == iStatus)
		{
		// the value 0xFFFFFFFF indicates an error.
		throw((CString) "SetFilePointer NG");
		}
	else
		{
		// ok.
		}
	}


	void
QueueFileManager::CreateEmptyQueueFile(void)
	{
	int iStatus;
	unsigned long NumberOfBytesReadOrWritten;
	QUEUE_HEADER QueueHeader = {0, 0, 0, 0, 0, 0};

	try
		{
		// win32 api version of "open" for a file that is expected 
		//  to NOT already exist.
		//
		m_fh = ::CreateFile(m_PathAndFilename,
							GENERIC_READ | GENERIC_WRITE,
							FILE_SHARE_READ | FILE_SHARE_WRITE,
							NULL,
							CREATE_NEW,
							FILE_ATTRIBUTE_NORMAL,
							NULL);

		if(m_fh == INVALID_HANDLE_VALUE)
			{
			// the file could not be opened.
			throw((CString) " Unable to CREATE_NEW queue file");
			}

		m_bFileIsOpen = true;

		LockHeader();

		// seek before write header.
		//
		SetFilePointer(0);

		// write the "empty" queue header info to the file.
		//
		iStatus = ::WriteFile(m_fh,                    
								&QueueHeader,               
								20,     
								&NumberOfBytesReadOrWritten,  
								NULL);

		if((0 != iStatus) && (NumberOfBytesReadOrWritten == 20))
			{
			// ok
			}
		else
			{
			throw((CString) "Write Header NG");
			}
		}
	catch(CString Reason)
		{
		// print reason for failure...
		//
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"QueueFileManager::CreateEmptyQueueFile() - catch() [%s]", 
						Reason);
		
		CaptureFirstError((CString) "QueueFileManager::CreateEmptyQueueFile() - [" + Reason + "]");
		}	
	
	UnlockHeader();
	Close();
	}


	// do the job, or throw.
	//
	void
QueueFileManager::Open(void)
	{	
	// win32 api version of "open" for a file that is expected to already exist.
	//
	// note: don't use FILE_FLAG_NO_BUFFERING, it requires sector size writes, 
	//  memory alignment, etc. ... a lot of work.
	//

	m_fh = ::CreateFile(m_PathAndFilename,
					GENERIC_READ | GENERIC_WRITE,
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					NULL,
					OPEN_EXISTING,
					FILE_FLAG_WRITE_THROUGH,
					NULL);

	if(m_fh == INVALID_HANDLE_VALUE)
		{
		// the file could not be opened.

		// attempt to create an empty queue, 
		//  by writing just the header info.
		//
		CreateEmptyQueueFile();

		// whether or not the file is created successfully or not, 
		//  this open request fails, better luck next time.
		//  your luck will be better the next time if the file was
		//  created by CreateEmptyQueueFile() this time.

		CString csErrorText;
		csErrorText.Format("Unable to Open():[%s]", m_PathAndFilename);
		throw(csErrorText);

		//throw((CString) "Unable to Open().");
		}

	m_bFileIsOpen = true;
	}


	void
QueueFileManager::Close(void)
	{
	// if file is known to be open...
	//
	if(m_bFileIsOpen)
		{
		// close it, ignoring any return code.
		//  best efforts attempt.
		//
		m_bFileIsOpen = false;

		::CloseHandle(m_fh);
		}
	}


	bool 
QueueFileManager::IsItemAvailable(void)
	{
	int iStatus;
	bool bReturnStatus = false;
	unsigned long NumberOfBytesReadOrWritten;
	QUEUE_HEADER QueueHeader = {0, 0, 0, 0, 0, 0};

	try
		{
		// throw will occur if unsuccessful.
		Open();		
		LockHeader();
		SetFilePointer(0);

		// read the header to determine what record number is
		//  the next one to read from.
		iStatus = ::ReadFile(m_fh,                
							  &QueueHeader,             
							  20,  
							  &NumberOfBytesReadOrWritten,
							  NULL);

		if((0 != iStatus) && (NumberOfBytesReadOrWritten == 20))
			{
			// ok.
			}
		else
			{
			CString csErrorText;
			csErrorText.Format("Read Header NG: [%d] [%d]", iStatus, NumberOfBytesReadOrWritten);
			throw(csErrorText);

			//throw((CString) "Read Header NG");
			}

		// slightly different information for informational (display) purposes.
		if(QueueHeader.LastWritten >= QueueHeader.LastRead)
			{
			// capture information to be used purely for display purposes.
			//  this is kept, for now in member variables, and exposed
			//  thru read-only properties.
			m_LastWritten = QueueHeader.LastWritten;
			m_UnclaimedCount = QueueHeader.LastWritten - QueueHeader.LastRead;
			}

		if(QueueHeader.LastWritten > QueueHeader.LastRead)
			{
			// ok to subtract, the number will come out positive.
			bReturnStatus = true;
			}
		}
	catch(CString Reason)
		{
		// print reason for failure...
		//
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"QueueFileManager::IsItemAvailable() - catch() [%s]", 
						Reason);
		
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"  Path and Filename [%s]", 
						m_PathAndFilename);	

		CaptureFirstError((CString) "QueueFileManager::IsItemAvailable() - [" + Reason + "]");
		}

	UnlockHeader();
	Close();

	return bReturnStatus;
	}


	void
QueueFileManager::ClearQueueData()
	{
	memset(m_DataBuffer, 0 , 226);
	}


	bool 
QueueFileManager::TruncateQueue(void)
	{
	bool bReturnStatus = false;
	int iStatus;
	unsigned long NumberOfBytesReadOrWritten;
	QUEUE_HEADER QueueHeader = {0, 0, 0, 0, 0, 0};

	try
		{
		// throw will occur if unsuccessful.
		Open();			
		LockHeader();	
		SetFilePointer(0);

		// write the updated header back to the file.
		iStatus = ::WriteFile(m_fh,                    
								&QueueHeader,               
								20,     
								&NumberOfBytesReadOrWritten,  
								NULL);

		if((0 != iStatus) && (NumberOfBytesReadOrWritten == 20))
			{
			// ok
			}
		else
			{
			CString csErrorText;
			csErrorText.Format("Write Header NG: [%d] [%d]", iStatus, NumberOfBytesReadOrWritten);
			throw(csErrorText);

			///throw((CString) "Write Header NG");
			}

		// set file length of the file to 20 bytes.
		//  to truncate the file.

		// use SetFilePointer() to position to byte 20, 
		//  then use SetEndOfFile() to truncate.
		//
		SetFilePointer(20);
		::SetEndOfFile(m_fh);

		bReturnStatus = true;	// success.
		}
	catch(CString Reason)
		{
		// print reason for failure...
		//
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"QueueFileManager::TruncateQueue() - catch() [%s]", 
						Reason);	

		CaptureFirstError((CString) "QueueFileManager::TruncateQueue() - [" + Reason + "]");
		}

	UnlockHeader();
	Close();

	return bReturnStatus;
	}

	
	bool 
QueueFileManager::RecycleRequest(void)
	{
	int iStatus;
	bool bReturnStatus = false;
	unsigned long NumberOfBytesReadOrWritten;
	QUEUE_HEADER QueueHeader = {0, 0, 0, 0, 0, 0};

	// the one-and-only place to "recycle", this replaces the old "file rename",
	//  and is equivalent in function, but straight forward in its implementation.
	//
	try
		{
		// throw will occur if unsuccessful.
		Open();		
		LockHeader();
		SetFilePointer(0);

		// read the header to determine what record number is
		//  the next one to read from.
		//
		iStatus = ::ReadFile(m_fh,                
							  &QueueHeader,             
							  20,  
							  &NumberOfBytesReadOrWritten,
							  NULL);

		if((0 != iStatus) && (NumberOfBytesReadOrWritten == 20))
			{
			// ok.
			}
		else
			{
			CString csErrorText;
			csErrorText.Format("Read Header NG: [%d] [%d]", iStatus, NumberOfBytesReadOrWritten);
			throw(csErrorText);

			//throw((CString) "Read Header NG");
			}

		if(QueueHeader.LastWritten == 0)
			{
			// claim successful recycle, when none is necessary,
			//  due to zero items in queue.
			//
			PrintMessage(MSGTYPE_SYSTEM, 
				hwndGeneralMsg, 
				"QueueFileManager::RecycleRequest() - "
				"no point to recycle, queue has no items.");

			bReturnStatus = true;
			}
		else
			{
			// some items are in queue, a non-empty queue,
			//  a recycle should be further considered. 

			// conditions to attempt recycle actions.
			//  * non-zero item count.
			//  * all items processed.
			//		
			if((QueueHeader.LastWritten > 0) && 
				(QueueHeader.LastWritten == QueueHeader.LastRead)) 
				{
				// the recycle will be attempted.
				//

				// determine whether to copy the contents of the queue file
				//  for reporting purposes, or simply truncate the queue,
				//  saving no information.
				//
				if(!param.QueueDelete)
					{
					// when the parameter QueueDelete is false, 
					//  this means "YES", copy the contents 
					//  of the queue files to a "target" file.

					// before re-cycling the queue, 
					//  create a new filename of the format "Rmmddnnn.Rqq".
					//  to store the complete contents of the current queue into.
					//  then issue a copy command.
					// 
					CString TargetFilename; TargetFilename = GetTargetFilename();

					PrintMessage(MSGTYPE_SYSTEM, 
									hwndGeneralMsg, 
									"  source filename: [%s]", 
									m_PathAndFilename);	

					PrintMessage(MSGTYPE_SYSTEM, 
									hwndGeneralMsg, 
									"  target filename: [%s]", 
									TargetFilename);

					if(KopyFile(m_PathAndFilename, TargetFilename))
						{
						PrintMessage(MSGTYPE_SYSTEM, 
										hwndGeneralMsg, 
										"  successfully copied queue file.");
						}
					else
						{
						PrintMessage(MSGTYPE_SYSTEM, 
										hwndGeneralMsg, 
										"  unable to copy queue file.");
						}
					}

				// on successful, or unsuccessful copy, 
				//  or just truncation has been requeusted,
				//  go ahead and truncate the file.
				//
				QueueHeader.LastWritten = 0;
				QueueHeader.LastRead = 0;

				// write the updated header back to the file.
				SetFilePointer(0);
				iStatus = ::WriteFile(m_fh,                    
										&QueueHeader,               
										20,     
										&NumberOfBytesReadOrWritten,  
										NULL);

				if((0 != iStatus) && (NumberOfBytesReadOrWritten == 20))
					{
					// ok
					}
				else
					{
					CString csErrorText;
					csErrorText.Format("Unable to Recycle Queue File: [%d] [%d]", iStatus, NumberOfBytesReadOrWritten);
					throw(csErrorText);

					//throw((CString) "Unable to Recycle Queue File.");
					}

				// set file length of the file to 20 bytes.
				//  to truncate the file.
				//
				SetFilePointer(20);
				::SetEndOfFile(m_fh);
				
				// claim successful recycle.
				//
				bReturnStatus = true;

				// announce successful truncation.
				//
				PrintMessage(MSGTYPE_SYSTEM, 
								hwndGeneralMsg, 
								"QueueFileManager::RecycleRequest() - "
								"queue file truncated:");
				}
			}
		}
	catch(CString Reason)
		{
		// print reason for failure...
		//
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"QueueFileManager::RecycleRequest() - catch() [%s]", 
						Reason);
		
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"  Path and Filename [%s]", 
						m_PathAndFilename);	

		CaptureFirstError((CString) "QueueFileManager::RecycleRequest() - [" + Reason + "]");
		}

	UnlockHeader();
	Close();

	return bReturnStatus;
	}

	bool 
QueueFileManager::ClaimQueueItem(QueueBufferClass * QBC, int MessagePartsExpected)
	{
	bool bReturnStatus = false;
	int iStatus;
	unsigned long NumberOfBytesReadOrWritten;
	QUEUE_HEADER QueueHeader = {0, 0, 0, 0, 0, 0};

	QBC->ClearWholeMessage();	// clears a flag.

	try
		{
		// throw will occur if unsuccessful.
		Open();			
		LockHeader();
		SetFilePointer(0);

		// read the header to determine what record number is
		//         the next one to read from.
		iStatus = ::ReadFile(m_fh,                
							  &QueueHeader,             
							  20,  
							  &NumberOfBytesReadOrWritten,
							  NULL);

		if((0 != iStatus) && (NumberOfBytesReadOrWritten == 20))
			{
			// ok.
			}
		else
			{
			CString csErrorText;
			csErrorText.Format("Read Header NG: [%d] [%d]", iStatus, NumberOfBytesReadOrWritten);
			throw(csErrorText);

			//throw((CString) "Read Header NG");
			}

		do
			{
			ClearQueueData();	// 226 bytes cleared out.

			// calculate the byte offset to read from.
			int iByteOffset = (QueueHeader.LastRead * 226) + 20;
			
			// seek before reading record that we intend to claim.
			SetFilePointer(iByteOffset);
			
			// read the record.
			iStatus = ::ReadFile(m_fh,                
									&m_DataBuffer,             
									226,  
									&NumberOfBytesReadOrWritten,
									NULL);			
			
			if((0 != iStatus) && (NumberOfBytesReadOrWritten == 226))
				{
				// ok
				}
			else
				{
				CString csErrorText;
				csErrorText.Format("Read Record NG: [%d] [%d]", iStatus, NumberOfBytesReadOrWritten);
				throw(csErrorText);

				//throw((CString) "Read Record NG");
				}
			QBC->TransferToMemberVars((char *) &m_DataBuffer, MessagePartsExpected);
			QueueHeader.LastRead++;		

			} while(!QBC->IsLastDataField());

		// seek before write header.
		SetFilePointer(0);

		// after item has been read successfully, 
		//  only then write back the LastRead element of the flat-file queue header.

		// write the updated header back to the file.
		iStatus = ::WriteFile(m_fh,                    
								&QueueHeader,               
								20,     
								&NumberOfBytesReadOrWritten,  
								NULL);

		if((0 != iStatus) && (NumberOfBytesReadOrWritten == 20))
			{
			// ok
			}
		else
			{
			CString csErrorText;
			csErrorText.Format("Write Header NG: [%d] [%d]", iStatus, NumberOfBytesReadOrWritten);
			throw(csErrorText);

			//throw((CString) "Write Header NG");
			}

		bReturnStatus = true;	// success.
		}
	catch(CString Reason)
		{
		// print reason for failure...
		//
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"QueueFileManager::ClaimQueueItem() - catch() [%s]", 
						Reason);
		
		CaptureFirstError((CString) "QueueFileManager::ClaimQueueItem() - [" + Reason + "]");
		}

	UnlockHeader();
	Close();

	return bReturnStatus;
	}


	bool
QueueFileManager::WriteQueueItem(QueueBufferClass* QBC, bool listPage)
	{
	int iStatus;
	bool bReturnStatus = false;
	unsigned long NumberOfBytesReadOrWritten;
	QUEUE_HEADER QueueHeader = {0, 0, 0, 0, 0, 0};

	try
		{
		// throw will occur if unsuccessful.
		Open();			
		LockHeader();	
		SetFilePointer(0);

		// read the header to determine what record number is
		//  the next one to write to.
		//
		iStatus = ::ReadFile(m_fh,                
							  &QueueHeader,             
							  20,  
							  &NumberOfBytesReadOrWritten,
							  NULL);

		if((0 != iStatus) && (NumberOfBytesReadOrWritten == 20))
			{
			// ok.
			}
		else
			{
			CString csErrorText;
			csErrorText.Format("Read Header NG: [%d] [%d]", iStatus, NumberOfBytesReadOrWritten);
			throw(csErrorText);

			//throw((CString) "Read Header NG");
			}

		// the information just read from the queue header
		//  is a 0 based number that represents how many items
		//  are currently available in the queue for processing.
		//
		// it is also the index (zero based) of where to write 
		//  a new 226 byte queue record.
		//  after successful write of the 226 byte record, 
		//  LastWritten (short integer) is increased by 1.
		//
		int iByteOffset = (QueueHeader.LastWritten * 226) + 20;

		// seek before writing record.
		SetFilePointer(iByteOffset);

		// loop thru message and write out all bytes

		const int messageMAX = 148;
		string message = QBC->GetWholeMessage();
		string msgHeader;

		// check if list page and pull out numeric message.
		//
		if(listPage)
			{
			// list page, not single page.
			string::size_type pos = message.find('+');

			if(pos != string::npos)
				{
				msgHeader.assign(message.substr(0, pos + 1));
				// remove from message
				message.erase(0, pos + 1);
				}
			else	// no numeric part so add it
				{
				msgHeader.assign("+");
				}
			}
		else
			{
			// single page, not list page.

			// add the pager number to the pager number field if not empty.
			if(!QBC->pagerNumber.empty())
				{
				msgHeader.append(QBC->pagerNumber);
				msgHeader.append(1, '+');
				}
			
			// add the AuthCode to the pager number field if not empty.
			if(!QBC->pagerAuthCode.empty())
				{
				msgHeader.append(QBC->pagerAuthCode);
				msgHeader.append(1, '+');
				}
			}		
		
		PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "MessageHeader: [%s]", msgHeader.c_str());

		int FullMessageLength = message.length();
		int bytesToWrite = FullMessageLength;

		int mesLength = bytesToWrite;
		int bytesWritten = 0;
		int msgSubIndex;
		int msgSubLen;
		bool continuation;
		QUEUE_BUF_STRUCT physBuffer;

		int msgHeaderLen = msgHeader.length();
		int msgFree = messageMAX - msgHeaderLen;

		// break down the message into flat-file queue chunks.
		//  the 226 bytes will be written.
		//  the contents are the header bytes, followed by the 148 byte payload
		//  the 148 byte payload is made up of the phone number + pagerid + message content.
		//  our multi-block queue method requires that the phone number + pagerid will
		//  be included in each 148 byte payload chunk that is sent. it is redundant in 
		//  the second, third, etc block, but the format is maintained.
		//
		do
			{
			msgSubIndex = mesLength - bytesToWrite;
			msgSubLen = (bytesToWrite > msgFree) ? msgFree : bytesToWrite;
			continuation = bytesToWrite > msgFree;

			string msgSubStr(msgHeader);
			
			msgSubStr.append(message.substr(msgSubIndex, msgSubLen));

			bytesWritten = QBC->SetBufferPointer(physBuffer, msgSubStr, continuation);

			// for test, just print out the content.
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "chunk length:  [%d]", msgSubStr.length());
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "chunk content: [%s]", msgSubStr.c_str());

			// write to file
			iStatus = ::WriteFile(m_fh,                    
								&physBuffer,               
								226,   
								&NumberOfBytesReadOrWritten,  
								NULL);

			if((0 == iStatus) || (NumberOfBytesReadOrWritten != 226)) 
				{
				DWORD dwError = ::GetLastError();

				CString csErrorText;
				csErrorText.Format("Write Record NG: [%d] [%d] error:[%d]", iStatus, NumberOfBytesReadOrWritten, dwError);
				throw(csErrorText);

				//throw((CString) "Write Record NG");
				}

			bytesToWrite -= (bytesWritten - msgHeaderLen);
			QueueHeader.LastWritten++;

			// for test, just print out the content.
			PrintMessage(MSGTYPE_SYSTEM, hwndGeneralMsg, "chunk: [%s]", msgHeader.c_str());
			}
		while(bytesToWrite);

		// seek before write header.
		SetFilePointer(0);

		// write the updated header back to the file.
		iStatus = ::WriteFile(m_fh,                    
								&QueueHeader,               
								20,     
								&NumberOfBytesReadOrWritten,  
								NULL);

		if((0 != iStatus) && (NumberOfBytesReadOrWritten == 20))
			{
			// ok
			}
		else
			{
			CString csErrorText;
			csErrorText.Format("Write Header NG: [%d] [%d]", iStatus, NumberOfBytesReadOrWritten);
			throw(csErrorText);

			//throw((CString) "Write Header NG");
			}

		bReturnStatus = true;	// success.
		}
	catch(CString Reason)
		{
		// print reason for failure...
		//
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"QueueFileManager::WriteQueueItem() - catch() [%s]", 
						Reason);
		
		CaptureFirstError((CString) "QueueFileManager::WriteQueueItem() - [" + Reason + "]");
		}

	UnlockHeader();
	Close();

	return bReturnStatus;
	}


	CString
QueueFileManager::GetPathAndFileName(void)
	{
	return m_PathAndFilename;
	}


	int
QueueFileManager::GetUnclaimedCount(void)
	{
	return m_UnclaimedCount;
	}


	int
QueueFileManager::GetLastWritten(void)
	{
	return m_LastWritten;
	}


QueueFileManager::~QueueFileManager()
	{
	// at destruction...
	ClearQueueData();
	}


	// read a queue item.
	//  this is a read-only function,
	//  data to be used for display purposes only.
	//
	bool 
QueueFileManager::ReadQueueItem(int Item, QueueBufferClass * QBC)
	{
	bool bReturnStatus = false;
	int iStatus;
	unsigned long NumberOfBytesReadOrWritten;
	char LocalDataBuffer[226];

	ClearQueueData();

	try
		{
		// throw will occur if unsuccessful.
		Open();			
		LockHeader();	

		// calculate the byte offset to read from.
		int iByteOffset = (Item * 226) + 20;

		// seek before reading record that we intend to read.
		//
		SetFilePointer(iByteOffset);

		// read the record.
		//
		iStatus = ::ReadFile(m_fh,                
							  &LocalDataBuffer,             
							  226,  
							  &NumberOfBytesReadOrWritten,
							  NULL);

		// ensure that the PageInfo string is properly terminated 
		//  immediately after read from the disk.
		//
		m_DataBuffer[(sizeof(m_DataBuffer) - 1)] = 0;

		if((0 != iStatus) && (NumberOfBytesReadOrWritten == 226))
			{
			// ok
			}
		else
			{
			CString csErrorText;
			csErrorText.Format("Read Record NG: [%d] [%d]", iStatus, NumberOfBytesReadOrWritten);
			throw(csErrorText);

			//throw((CString) "Read Record NG");
			}
		// end of read 226 byte record.

		// the 226 bytes of data that were successfully read are stored
		//  in the buffer.
		//

		// put this information into member variables of the QueueBufferClass.

		// for read only, the number of expected message elemets can be set to the minimum (2).
		QBC->TransferToMemberVars((char *) &LocalDataBuffer, 2);

		bReturnStatus = true;	// success.
		}
	catch(CString Reason)
		{
		// print reason for failure...
		//
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"QueueFileManager::ReadQueueItem() - catch() [%s]", 
						Reason);
		
		CaptureFirstError((CString) "QueueFileManager::ReadQueueItem() - [" + Reason + "]");
		}

	UnlockHeader();
	Close();

	return bReturnStatus;
	}


// new properties for error reporting.
//
//  These properties are generally "set" by a condition internal to this class.
//  typically a "throw", due to unable to lock, read, write, etc.
//  An external monitoring process (main application loop) will query the 
//  error count and the last error description text to find out if and 
//  what problem is occurring. The monitoring process can then request that the
//  error count and error description be cleared, using public methods of the class.
//
	int 
QueueFileManager::GetErrorCount(void)
	{
	return m_iErrorCount;
	}


	void 
QueueFileManager::ClearErrorInfo(void)
	{
	m_iErrorCount = 0;
	m_csFirstErrorText = "";
	}


	CString 
QueueFileManager::GetFirstErrorText(void)
	{
	return m_csFirstErrorText;
	}



