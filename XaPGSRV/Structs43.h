

// Structs43.h

// contains things that are common to PageWin4, and PageWin3.


#if !defined(AFX_S43_H__B3B4A7DB_A2EB_4B42_AA11_B7903EE82209__INCLUDED_)
#define AFX_S43_H__B3B4A7DB_A2EB_4B42_AA11_B7903EE82209__INCLUDED_
#pragma once


//  page server "paging queue" record layout.
//	  this 226 byte format is used extensively 
//	  by Xtend applications.
//
typedef struct
	{
    char PageType[2];				//   0 Type of page 00 - 99         
    char PageStatus[2];             //   2 Status of page               
    char InitiatedDate[10];         //   4 Date page was initiated      
    char InitiatedTime[8];          //  14 Time page was initiated  
    
    char QQQQQ[10];					//  CompletedDate[10] - was never used - eliminated - kgill - 2005-03-17.     
    char RRRRR[8];					//  CompletedTime[8]  - was never used - eliminated - kgill - 2005-03-17.	

    char Extension[8];              //  40 ProfileId to be paged (note: only 8 chars wide)       
    char PagerId[7];                //  48 PagerId number  
    char InitiatorId[10];           //  55 Indentifies page source      
 	char dataContinuation;			//  
    short ACKMin;                   //  66 Minutes for ACK  
	
    unsigned short NextAckPtr;      //  68 Pointer to next record to acknowlege 
    char Printed[1];                //  70 If not ack has it been printed 
                                    //     P - indicates password verified 
                                    //     for status change lines. 
	char xxx[5];                    //     char VoiceLine[5] - was never used - eliminated - kgill - 2005-03-02. 
    unsigned short VoiceFile;		//  76 number of File used to record voice   
									//     file build this into a filename later
									//     VoiceFile number replaces XXXX
									//     MM is the month, DD is day number.
									//     filename format is: MMDDXXXX.VPO 
									//     for December 11th the first voice page 
									//     of the day will be 12110001.VPO
    char PageInfo[148];				//  78 NOTIS paging macro           
	} QUEUE_BUF_STRUCT;				// 226


#endif // !defined(AFX_S43_H__B3B4A7DB_A2EB_4B42_AA11_B7903EE82209__INCLUDED_)
