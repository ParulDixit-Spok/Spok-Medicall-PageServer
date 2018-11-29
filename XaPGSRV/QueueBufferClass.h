
// QueueBufferClass.h


//#include "afx.h"
#include <atlstr.h>
#include "Structs43.h"
#include <string>


#if !defined(AFX_QUEUEBUFFERCLASS_H__87587E0A_2BE6_4A66_974F_1BFAEDD0E036__INCLUDED_)
#define AFX_QUEUEBUFFERCLASS_H__87587E0A_2BE6_4A66_974F_1BFAEDD0E036__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class QueueBufferClass  
	{
	public:
		// public interface
		//
		QueueBufferClass();
		~QueueBufferClass();
		void TransferToMemberVars(char *, int);
		char * GetBufferPointer(void);		// this call implies TransferToQueueBuffer().

		//fills a QUEUE_BUF_STRUCT struct with members values
		int QueueBufferClass::SetBufferPointer(QUEUE_BUF_STRUCT& bPtr, std::string, bool continuation);

		// data items "set" functions.
		//
		void SetInitiatedDate(CString);
		void SetInitiatedTime(CString);
		void SetPageType(CString);
		void SetPageStatus(CString);
		void SetProfileId(CString);
		void SetPagerId(CString);
		void SetInitiatorProfileId(CString);
		void SetPageInfo(CString);
		void SetVoiceFileNumber(int);

		// data items "get" functions.
		//
		CString GetPageType(void);
		CString GetPageStatus(void);
		CString GetInitiatedDate(void);
		CString GetInitiatedTime(void);
		CString GetProfileId(void);
		CString GetPagerId(void);
		CString GetInitiatorProfleId(void);
		short GetACKMin(void);
		unsigned short GetNextAckPtr(void);
		CString GetPrinted(void);
		unsigned short GetVoiceFile(void);
		CString GetPageInfo(void);

		bool IsLastDataField() const;
		std::string GetWholeMessage() {return m_wholeMessage;}
		void SetWholeMessage(const std::string& str) {m_wholeMessage = str;}
		void SetWholeMessage(const char* str) {m_wholeMessage.assign(str);}

		// some poorly named flags, and methods, rename these at some point.
		void ClearWholeMessage() {startWrite = true;}
		void Mark() {startWrite = false;}
		bool IsFirstWrite() {return startWrite;}

	private:
		char * TransferToQueueBuffer(void);
		CString FillCString(char *, int);
		void InitializeMemberVars(void);
		void InitializeQueueBuffer(void);
		void QueueBufferClass::AllowOnlyASCII(char *, int);

	private:
		CString m_csPageType;			    
		CString m_csPageStatus;                        
		CString m_csInitiatedDate;        
		CString m_csInitiatedTime;     
		CString m_csProfileId;                 
		CString m_csPagerId;               
		CString m_csInitiatorProfileId;
		CString m_dataContinuation;
		short m_sACKMin;                          
		unsigned short m_usNextAckPtr;      
		CString m_csPrinted;                
		unsigned short m_usVoiceFile;		
		CString m_csPageInfo;
		std::string m_wholeMessage;
		bool startWrite;

		//i'm not writting setters and getters!!
	public:
		std::string pagerNumber;
		std::string pagerAuthCode;
	};

#endif // !defined(AFX_QUEUEBUFFERCLASS_H__87587E0A_2BE6_4A66_974F_1BFAEDD0E036__INCLUDED_)
