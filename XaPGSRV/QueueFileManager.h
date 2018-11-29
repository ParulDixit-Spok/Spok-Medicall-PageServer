
// QueueFileManager.h: a class to manage Xtend 226 byte flat file queues.


#if !defined(AFX_LOG_H__854EEF6C_06CB_4E54_A8EE_AF176FE49252__INCLUDED_)
#define AFX_LOG_H__854EEF6C_06CB_4E54_A8EE_AF176FE49252__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Structs43.h"
#include "QueueBufferClass.h"


class QueueFileManager
	{
	public:
		// public interface
		//
		QueueFileManager(CString, CString, int);
		~QueueFileManager();

		CString GetPathAndFileName(void);
		bool IsItemAvailable(void);
		bool ClaimQueueItem(QueueBufferClass *, int);
		int GetUnclaimedCount(void);
		int GetLastWritten(void);
		bool ReadQueueItem(int, QueueBufferClass *);
		bool TruncateQueue(void);
		bool RecycleRequest(void);
		bool WriteQueueItem(QueueBufferClass*, bool listPage);

		int GetErrorCount(void);
		void ClearErrorInfo(void);
		CString GetFirstErrorText(void);

	private:
		// private member variables.
		//
		HANDLE m_fh;
		CString m_Path;
		CString m_Filename;
		int m_iQueueNumber;
		CString m_PathAndFilename;
		char m_DataBuffer[226];
		bool m_bFileIsOpen;
		bool m_bHeaderIsLocked;
		int m_iErrorCount;
		CString m_csFirstErrorText;

	private:
		// private methods.
		//
		// these ones throw...
		void Open(void);
		void LockHeader(void);
		void CreateEmptyQueueFile(void);
		void SetFilePointer(int);

		// these ones don't throw...
		void UnlockHeader(void);
		void Close(void);
		void ClearQueueData(void);
		CString GetTargetFilename();
		bool KopyFile(CString, CString);
		void CaptureFirstError(CString);	

	private:
		// properties
		int m_LastWritten;
		int m_UnclaimedCount;
	};

#endif // !defined(AFX_LOG_H__854EEF6C_06CB_4E54_A8EE_AF176FE49252__INCLUDED_)

