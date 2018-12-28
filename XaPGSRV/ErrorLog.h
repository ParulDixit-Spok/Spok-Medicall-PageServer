// ErrorLog.h: interface for the ErrorLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ERRORLOG_H__90BBECEA_8DE7_4BCD_9A8F_A7ED2CD38B11__INCLUDED_)
#define AFX_ERRORLOG_H__90BBECEA_8DE7_4BCD_9A8F_A7ED2CD38B11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <streambuf>
#include "Util2.h"

class LogBuffer : public std::streambuf
{
public:
	LogBuffer(HWND winHandle)   : winHnd(winHandle)
	{
		setp(buffer, buffer+(kBufferSize-1));
	}

	virtual ~LogBuffer()
	{     
		sync();
	}

	void AssignDocument(HWND winHandle)
	{ 
		winHnd = winHandle;
	}

protected:
	int flushBuffer(){
		int num = pptr()-pbase();
		
		//make sure line is null terminated
		if(num < kBufferSize)
			buffer[num] = 0;
		else
			buffer[kBufferSize]=0;

		//docPtr->DisplayMsg(buffer);

		DumpMessage(MSGTYPE_SYSTEM, winHnd, buffer);

		pbump(-num);
		return num;
	}

	virtual int_type overflow(int_type c)
	{
		if(c != EOF){
			*pptr() = c;
			pbump(1);
		}
		if(flushBuffer() == EOF) {
			return EOF;
		}
		return c;
	}

	virtual int sync() {
		if(flushBuffer() == EOF){
			return -1;
		}
		return 0;
	}

protected:
	enum{kBufferSize = 120};
	char buffer[kBufferSize+1]; //add one for case of full buffer getting null terminated
	HWND winHnd;
};





#endif // !defined(AFX_ERRORLOG_H__90BBECEA_8DE7_4BCD_9A8F_A7ED2CD38B11__INCLUDED_)
