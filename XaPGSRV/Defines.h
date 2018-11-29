
// defines.h

#ifndef _defines_h_
#define _defines_h_
#pragma once

// used with RemoveAlpha() 
#define NUMERIC_FILTER_COMM_PORT		1
#define NUMERIC_FILTER_DIALOGIC_PORT	2
#define NUMERIC_FILTER_ABSOLUTE			3


// message type definitions
//  for all windows.
//
#define MSGTYPE_SYSTEM							0x00000001
#define MSGTYPE_ERROR							0x00000002
#define MSGTYPE_SYSTEM_FINAL					0x00000004		// somewhat verbose "final" message
#define MSGTYPE_SYSTEM_PROGRESS					0x00000008		// quite verbose progress message

#define MSGTYPE_DATABASE_COMMAND				0x00000100
#define MSGTYPE_DATABASE_DATA					0x00000200
#define MSGTYPE_DATABASE_DETAIL					0x00000400
#define MSGTYPE_DATABASE_8						0x00000800
#define MSGTYPE_DATABASE_ERROR					0x00001000

#define MSGTYPE_IVR								0x00100000
#define MSGTYPE_IVR_VOICE_PROMPT_TEXT			0x00200000
#define MSGTYPE_DIALOGIC_4						0x00400000
#define MSGTYPE_IVR_PROGRESS					0x00800000

#define MSGTYPE_COMM							0x01000000
#define MSGTYPE_COMM_ERROR						0x02000000
#define MSGTYPE_COMM_DTR						0x04000000		
#define MSGTYPE_COMM_RESET						0x08000000

#define MSGTYPE_DEBUG							0x10000000
#define MSGTYPE_DEBUG_2000						0x20000000
#define MSGTYPE_DEBUG_4000						0x40000000
#define MSGTYPE_DEBUG_8000						0x80000000


// define several print-message groups.
#define MESSAGE_GROUP_STANDARD	MSGTYPE_SYSTEM | \
								MSGTYPE_ERROR | \
								MSGTYPE_IVR | \
								MSGTYPE_COMM | \
								MSGTYPE_DATABASE_COMMAND;

#define MESSAGE_GROUP_DEBUG		MSGTYPE_SYSTEM | \
								MSGTYPE_ERROR | \
								MSGTYPE_IVR | \
								MSGTYPE_COMM | \
								MSGTYPE_IVR_VOICE_PROMPT_TEXT | \
								MSGTYPE_DATABASE_COMMAND | \
								MSGTYPE_DATABASE_DATA | \
								MSGTYPE_DATABASE_DETAIL | \
								MSGTYPE_IVR_VOICE_PROMPT_TEXT | \
								MSGTYPE_COMM | \
								MSGTYPE_COMM_ERROR | \
								MSGTYPE_COMM_DTR | \
								MSGTYPE_COMM_RESET | \
								MSGTYPE_DEBUG; 

#define MESSAGE_GROUP_ALL		0xffffffff
#define MESSAGE_GROUP_NONE		0x00000000


enum 
	{ 
	CHECKMARK_RESET = 0, 
	CHECKMARK_SET, 
	CHECKMARK_TOGGLE, 
	CHECKMARK_UPDATE,
	};


enum 
	{ 
	START_PAGE = 0, 
	HEADER_1,
	HEADER_2,
	HEADER_3,
	MID_PAGE,		// table data, or just data item.
	BEGIN_TABLE,
	BEGIN_TABLE_HEADER,
	END_TABLE_HEADER,
	BEGIN_TABLE_ROW,
	END_TABLE_ROW,
	END_TABLE,
	END_PAGE,
	};

//	
//		Defines for Page Structure allocation
//																		// # of items   start index
#define	PAGE_D4X_START				0									// 96                0
#define PAGE_COM_START				(PAGE_D4X_START + MAX_IVRPORTS)		// 34               96
#define PAGE_LIST_POS				(PAGE_COM_START + MAX_COMMPORTS)	//  1              130
#define PAGE_COV_POS				(PAGE_LIST_POS + 1)					//  1              131
#define PAGE_MULTI_POS				(PAGE_COV_POS + 1)					//  1              132
#define PAGE_MISC_POS				(PAGE_MULTI_POS + 1)				//  1              133
#define PAGE_MANUAL_POS				(PAGE_MISC_POS + 1)					//  1              134
#define PAGE_TEST_POS				(PAGE_MANUAL_POS + 1)				//  1              135
#define PAGE_ITEMS_TO_ALLOCATE		(PAGE_TEST_POS + 1)					//  total items == 136              

											// 0 to 95		- Reserved for radio common carrier	
											//					paging via dialogic lines.
											// 96 to 129	- Reserved for paging via com port	
											// 130			- Allocated to List Paging			
											// 131			- Covering extension temporary info 
											// 132			- Allocated for Multi-Paging        
											// 133			- Midnight paging                   
											//				- Call sign paging
											// 134			- Keyboard Paging					
											// 135			- test paging, from alt-menu

// ivr option for "referred to"
//
#define MAX_DIGITS_IN_REFERRED_TO_NUMBER	30


//  Specific definitions for radio interface  

#define RXBUFFLEN      1024
//#define TXBUFFLEN      1024

#define MAXPROMPTS      200
#define MAXDIGITS       300
#define MAXSTATUS       200

#define SETUP          0
#define ACTIVATE       1
#define DEACTIVATE     2

#define SERVICE1       '1'
#define SERVICE2       '2'

#define FUNCTION0      '0'
#define FUNCTION1      '1'
#define FUNCTION2      '2'
#define FUNCTION3      '3'
#define FUNCTION4      '4'
#define FUNCTION5      '5'
#define FUNCTION6      '6'
#define FUNCTION7      '7'
#define FUNCTION8      '8'
#define FUNCTION9      '9'


// keep in sync with "text" in xnpgcom.cpp
#define PRESET              0
#define PRESET1             1
#define PRESET2             2
#define PRESET3             3
#define PRESET4             4
#define PRESET5             5
#define PRESET6             6
#define PRESET7             7
#define PRESET8             8
#define PRESET9             9

#define PRESETA             10
#define PRESETB             11
#define PRESETC             12
#define PRESETD             13
#define PNOTREADY           14
#define PNOTREADYTIMEOUT    15
#define PDTRLOW             16
#define PDTRPAUSE           17
#define PDTRHIGH            18
#define PREADY              19

#define PSEND               20
#define PMESSAGE            21
#define PRESETJ             22	// reused 2009-12-08 kgill.
#define PTALKNOW            23
#define PMESSAGETIMEOUT     24
#define PPAGINGTIMEOUT      25
#define PTALKNOWTIMEOUT     26
#define PRESETTIMEOUT       27
#define PSEND2              28
#define PSEND3              29

#define PSEND4              30
#define PSEND5              31
#define PSEND6              32
#define PSEND7              33
#define PSEND8              34
#define PSEND9              35		// now used to keep track of multiple block TAP messages.
#define PSENDTIMEOUT        36
#define PWAIT1              37
#define PWAIT2              38
#define PWAIT3              39

#define PABORT              40
#define PABORT2             41
#define PABORT3             42
#define PABORT4             43
#define PABORT5             44
#define PABORT6             45
#define PTALKING            46
#define PSENDA              47
#define PSENDB              48
#define PPAGINGTIMEOUT2     49

#define xxxUNKNOWN          50
#define PPAGINGBUSY         51
#define PTALKNOWBUSY        52
#define PHANGUP             53
#define PHANG2              54
#define PHANG3              55
#define PHANG4              56
#define PHANG5              57
#define PHANG6              58
#define PREADY2             59

#define P_DELAY_BEFORE_DISCONNECT		60	
#define P_DISCONNECT_SEQUENCE			61
#define P_DELAY_BEFORE_DTR_LOW			62
#define ST_BEGIN_TAP_MESSAGE			63
#define ST_GET_NEXT_TAP_BLOCK			64
#define ST_CHECK_ERROR_COUNTS			65
#define ST_WAIT_FOR_BLOCK_BEGIN			66
#define ST_WAIT_FOR_BLOCK_END			67
#define ST_WAIT_FOR_CHECKSUM			68
#define ST_ANALYZE_CHECKSUM				69

#define ST_ANALYZE_BLOCK				70
#define ST_BEGIN_SIGNON					71
#define ST_SEND_GOAHEAD					72
#define ST_WAIT_FOR_LOGIN				73
#define ST_BAD_CHECKSUM					74
#define ST_GOOD_PAGERID					75
#define ST_BAD_PAGERID					76
#define ST_ANALYZE_TAP_MESSAGE			77
#define ST_DISCONNECT_AND_START_OVER	78

#define ST_ACK_AND_GET_NEXT_BLOCK		79

#define ST_NEW_1						80
#define ST_NEW_2						81
#define ST_NEW_3						82
#define ST_NEW_4						83

#define LAST_ONE						84
// remember to define new "text" in xnpgcom.cpp when adding new states.


#define PNOERROR				0
#define WRONGPAGETYPEERR        1
#define READYTIMEOUTERR			2
#define WRONGFUNCTIONTYPEERR    3
#define MESSAGETIMEOUTERR		4
#define PAGINGTIMEOUTERR		5
#define TALKNOWTIMEOUTERR       9


#define RPNOTREADY              0
#define RPREADY                 1
#define RPPAGING                2
#define RPVOICE                 3
//#define RPCOMM                  4
#define RPABORT                 5
#define RPWAITING               6
//#define RPTALKINGTIMEOUT        7
#define RPMESSAGE               8


#define ST_ONHK        0   // close the file and go onhook                  
#define ST_IDLE        1   // waiting for an incoming call                  
#define ST_OFFHK       2   // go offhook to accept call                     
#define ST_T1_PAUSE	   28	//	Pause After Taking T1 Line Off Hook
#define ST_HELLO_MSG   3   // play initial prompt                           
#define ST_GET_EXT     4   // get dtmf digits, extension information        
#define ST_NO_LOG_MSG  5   // no log file, beeper unavailable message       
#define ST_PLAY_PROMPT 6   // play: custom recorded greeting                
#define ST_CB_MSG      7   // play: enter callback number                   
							//		or Current Status is 
#define ST_CB_MSG2     8   // playing: Current Status is
#define ST_CB_MSG3     9   // play: enter callback number after status is promted
                           //         or forwarding number   
              
#define ST_IMPAIRMENT_MSG	10  // Play: custom impairment notification message
#define ST_GET_DIS			11  // get callback display information              
#define ST_GOODBYE_MSG		12  // call completed, play goodbye message          
#define ST_GOODBYE3_MSG		13	// call completed, play goodbye message          
#define ST_ADD_PAGE			14  // if file present, add record to beeper file    
#define ST_ANOTHER			15  // Page another extension                        
#define ST_ANOTHER_GET		16  // Get dtmfs                                     
#define ST_DID_GET_EXT		17  // Get extension for DID40 interface             
#define ST_DID_HKFLSH		18  // DID40 Hookflash                               
#define ST_EXCP_TIME		19  // play: exception time	
						   
//                                                                          
//     States for recording voice pages                                    
//                                                                          
#define ST_CB_VOICE    20  // play: speak now                               
#define ST_GET_VOICE   21  // record voice page information                 
#define ST_NO_VOICE    22  // play: Voice paging not available              
#define ST_VP_PAGE     25  // Ready to send voice                           
#define ST_VP_IDLE     26  // Idle voice port                               
#define ST_VOICE_DTMF  27  // Check dtmf's entered for voice message prior  
                           //  to speak now message                         
//                                                                          
//      States for List Paging                                              
//                                                                          
#define ST_LSTNG_MSG   30  // play: list # unavailable message              
#define ST_LSTOK_MSG   31  // play: paging list # or prompt                 
#define ST_LSTOK_VCE   32  // play: custom recorded list file               
#define ST_LST_REC_OK  33  // play: code completed                          
#define ST_LST_RECORD  34  // record list voice message                     
#define ST_LST_GET_DIS 35  // get numeric callback number                   
#define ST_LST_SPK_MSG 36  // play: speak now message                       

// States for covering doctors                                         
#define ST_GET_CVR_RES 40  // get response from page covering doc question 
#define ST_CVR_RECUR   41  // recursive coverage   
                        
#define ST_CHECK_IMPAIRED_VENDOR_ANN	49	// check whether to play the 
											//  impaired vendor announcement.

// Common Carrier Paging States
#define ST_ONHK_DELAY  50  // delay after going on hook ( 2 seconds )       
#define ST_CCPG_CALL   51  // call the paging service, wait for ring        
#define ST_CCPG_ERR    52  // call terminated, or no operator interupt      

#define ST_PLAY_ID        60  // No custom prompt file play id file            
#define ST_GOODBYE_PROMPT 61 // Confirm paged parties name prompt           
#define ST_GOODBYE_ID	  62  // Confirm paged parties id                      
#define ST_GOODBYE2_MSG   63  // Play Thankyou goodbye do not que page.     
#define ST_GREETING1_MSG  64  // Play custom greeting for status change     
#define ST_GREETING2_MSG  65  // Play custom greeting for page              

// CATS Paging States
#define ST_CATS_MSG    70  // Play initial CATS message                     
#define ST_CATS_REC_ON 71  // Record CATS message                           
#define ST_CATS_REC_OF 72  // Record CATS message                           
#define ST_CATS_V_ON   73  // Record CATS message                           
#define ST_CATS_PAGE   74  // Initiate a CATS page                          
#define ST_CATS_V_OFF  75  // Record CATS message                           
#define ST_CATS_CALL   76  // Initiate a CATS group call                    
#define ST_CATS_GET_DT 77  // # pressed during record, get DTMF's           
#define ST_CATS_PLAY   78  // play recorded prompt during group call        
#define ST_CATS_ONHK   79  // put phone on hook after cats group call      
#define ST_CATS_ABORT  80  // CATS call aborted by operator                 
#define ST_CATS_REC_NG 81  // Play Code aborted mesage                      
#define ST_CATS_REC_OK 82  // Play Code completed message                   
//                                                                         
//     Recorded voice radio page states                                    
//                                                                          
//                                                                          
//      Main menu states                                                    
//                                                                          
#define ST_MENU_STAT_2	89  // Play exception time                           
#define ST_MENU_PASS	90  // Play enter password message                   
#define ST_MENU_PSCHK	91  // Get password and check                        
#define ST_MENU_PSERR	92  // Play invalid password message                 
#define ST_MENU_STAT	93  // Play current status                           
#define ST_MENU_MSG		94  // Play main menu message                        
#define ST_MENU_CHK		95  // Check response from menu options   
#define ST_MENU_96		96	// Free
#define ST_MENU_HELLO	97  // Play Hello                                    
#define ST_MENU_V_PRMPT 98  // play custom voice prompt                     
#define ST_MENU_I_PRMPT 99  // play id number                               

#define ST_OP_TRANSFER  100 // Transfer to operator                          
#define ST_OP_TRANSFER2 101 // Transfer to operator                          
//                                                                          
//      status change states                                                
//                                                                          
#define ST_STAT_V_PRMPT 120 // play custom voice prompt                     
#define ST_STAT_I_PRMPT 121 // play id number                               
#define ST_STAT_HELLO	122 // play hello                                   
#define ST_STAT_CHANGE2 123 // select new current status 10-29, 1 or 2      
#define ST_STAT_MSG6   	124  // get ready to play custom coverage greeting   
#define ST_STAT_MSG7   	125  // play custom coverage greeting                
#define ST_STAT_MSG4   	126  // get ready to play pound sign option          
#define ST_STAT_MSG5   	127  // play pound sign option                      
#define ST_STAT_MSG    	128  // play current pager status // covered by      
#define ST_STAT_MSG2   	129  // set up for ST_STAT_MSG3                      
#define ST_STAT_MSG3   	130  // play current pager status                    
#define ST_STAT_NEXT   	131  // what next? hang-up, or change                
#define ST_STAT_PASS   	132  // check user password if available, else system
                             // password                                     
#define ST_STAT_PSCHK  	133
#define ST_STAT_PSERR  	134
#define ST_STAT_TYPES  	135  // play available page types                   
#define ST_STAT_CHANGE 	136  // select new current status                   
#define ST_STAT_WRITE  	137  // change pager status                         
						     // flows right into ST_STAT_MSG				

#define ST_STAT_C_MSG  	140  // play enter covering ext number               
#define ST_STAT_C_GET  	141  // get covering ext number via dtmf             
#define ST_STAT_C_NG1  	142  // play invalid recursive coverage               
#define ST_STAT_C_NG2  	143  // play invalid extension message               
#define ST_STAT_C_NG3	144  // play invalid extension status is            
#define ST_STAT_C_NG4   145  // play invalid extension status is             
#define ST_STAT_WRITE2  146  // change pager status                          
#define ST_STAT_C_NG5   147  // play "Impaired Paging Vendor" prompt.             

#define ST_GET_EMAIL_EXT  149
#define ST_STAT_F_MSG	150  // play enter forwarding number                 
#define ST_STAT_F_GET	151  // get forwarding number                        

#define ST_GET_FORWARD_EXT  152
#define ST_STAT_M_FORWARD	153	 // forward voice message
#define ST_STAT_M_EMAIL		154	 // email voice message

#define ST_STAT_M_MSG   155 // play select type of mesages to retieve       
#define ST_STAT_M_GET   156 // get TT type of messages to retrieve - delivered/undelivered          
#define ST_STAT_M_MSG2  157 // play please wait retriving messages          
#define ST_STAT_M_SRCH  158 // scan log files for messages                  
#define ST_STAT_M_NO    159 // play no messages                             
#define ST_STAT_M_DATE  160 // play message date                            
#define ST_STAT_M_DATE2 161 // play message date                            
#define ST_STAT_M_DATE3 162 // play message date                            
#define ST_STAT_M_DATE4 163 // play message date                            
#define ST_STAT_M_TALK  164 // play messages                                
#define ST_STAT_M_TALK2 165 // play messages                                
#define ST_STAT_M_VOICE 166 // play messages                                
#define ST_STAT_M_TEXT  167 // play messages                                
#define ST_STAT_M_MSG3  168 // play press 1,2,3: retrive next,current,previous
#define ST_STAT_M_GET3  169 // get TT type of message to retrieve           
#define ST_STAT_M_NO2   170 // play no more messages                        
#define ST_STAT_M_PASS  171 // check user password if available, else system
                            // password                                  
#define ST_STAT_M_PSCHK 172
#define ST_STAT_M_PSERR 173

#define ST_STAT_M_DATE5 174 // Play date for status change message          
#define ST_STAT_M_STAT  175 // Status change message                        
#define ST_STAT_M_DATE6 176 // Play date for coverage change message        
#define ST_STAT_M_CVR   177 // Coverage change message                      


#define ST_STAT_RFD_MSG 180 // prompt enter forwarding #                    
#define ST_STAT_RFD_GET 181 // get forwarding #                             
#define ST_STAT_RFD_NG  182 // invalid forwarding # status                  

//                                                                          
//      Voice prompt change states                                          
//                                                                          
#define ST_VCE_MSG      185 // play current pager status                    
#define ST_VCE_MSG2     186 // play current pager status                    
#define ST_VCE_MSG3     187 // play current pager status                    
#define ST_VCE_MSG4     188 // play current pager status                    
#define ST_VCE_NEXT     189 // what next? hang-up, or change                
#define ST_VCE_PASS     190 // check user password if available, else system
                            // password                                     
#define ST_VCE_PSCHK    191
#define ST_VCE_PSERR    192
#define ST_VCE_CHG_MSG  193 // play speak now                               
#define ST_VCE_CHANGE   194 // select new current pager                     
//                                                                          
//      Voice Paging States                                                 
//                                                                         
#define ST_VPAGE_TALK   196 // playing voice portion of page                
#define ST_VPAGE_DONE   197 // voice portion done                           
//                                                                          
//      Password change states                                              
//                                                                         
#define ST_PASS_MSG     200 // play current password                        
#define ST_PASS_NEXT    201 // what next? hang-up, or change                
#define ST_PASS_PASS    202 // check user password if available, else system
                            // password                                     
#define ST_PASS_PSCHK   203
#define ST_PASS_PSERR   204
#define ST_PASS_MSG2    205 // play enter password follwed by pound sign    
#define ST_PASS_GET     206 // get new password                             
#define ST_PASS_WRITE   207 // change password                              
//                                                                          
//     Self Park & Page States                                             
//                                                                         
#define ST_PARK_MSG     220 // play parking you now please wait             
#define ST_PARK_GET     221 // get dtmf to verify park & page               
#define ST_PARK_TMSG    222 // play your party is being paged message       
#define ST_PARK_TRSFR   223 // transfer caller to park meetme circut,       
#define ST_PARK_RECALL  224 // transfer caller back to dialogic board       
#define ST_PARK_NG_MSG  225 // play park line unavaialable                  
//                                                                          
//      Page By Name States                                                 
//                                                                          
#define ST_NAME_MSG     240 // play press the person's last name            
#define ST_NAME_GET     241 // get name dtmfs                               
#define ST_NAME_SELECT  242 // play press1 to page ...                      
#define ST_NAME_SELECT2 243 //                     ... custom voice prompt  
#define ST_NAME_CHOOSE  244 // get dtmf to select person to page            
#define ST_NAME_MSG2    245 // play id number #####                         
#define ST_NAME_MSG3    246 // matching name not found                      
//                                                                          
//      Exception Status Change States                                      
//                                                                         
#define ST_STAT_EX_MSG1	250	// play "enter month and day followed by #		
#define ST_STAT_EX_GET1 251 // Get month and day DTMFs						
#define ST_STAT_EX_MSG2 252 // play "enter time followed by #				
#define ST_STAT_EX_GET2 253 // Get time										
#define ST_STAT_EX_AMPM 254 // play "Enter 1 for AM 2 for PM"				
#define ST_STAT_EX_GET3 255 // Get AM or PM									
#define ST_STAT_EX_ERR  256 // error in setting schedule					
//
//		misc added in states
//
#define ST_STAT_C_NG6	260	//	play "invalid target for forwarding" message. 
#define ST_SMS_FS_MSG   261 //  1/12/2016 - DE20568 - SOguz -play "sorry unable to connect" 
							//              message for Spok Mobile type pagers in 
							//				fail safe mode only.
#define ST_ADD_ALL_DVCS_Q 262 // 6/24/2016 - SOguz - if all page devices is on then, add record to queue 
//
//		Transfer to Voice Mail States
//
#define ST_VM_TR_MSG	270	//	Transfer to voice mail message
#define ST_VM_TRANSFER	271	//	Transfer to voice mail

		
// definitions for the standard voice prompts contained in the .VAP files.
//  the prompts are defined a little strangely, the VFEdit.exe
//  audio file editor that we use defines the indexed file
//  with indexes starting at '1', the dx_play() function requires that
//  the indexes be '0' based.  for readability, the numbers are
//  defined with a '-1' appended so that the following list can
//  be printed and used easily without mentally subtracting 1 every
//  time, leave that job to the compiler.
//
//  note to the programmer: these symbolic constants will be
//  defined, typically as the first 2 words of the voice prompt.
//  they will be "worked into" the source code to make it more
//  readable and understandable as-needed.

// there are 3 voice prompt files defined for the PageServer.
//
//  internal	standard		
//  name		filename		notes
//	------		------------	---------------------------------------
//  DIGITS		DIGITS.VAP		hard-coded filename in the application
//  PROMPTS		XTPG2.VAP		PageServer config utility refers to this 
//								file as the "prompt file"
//  STATUS		XTPGSTAT.VAP	PageServer config utility refers to this
//								file as the "status file"

// DIGITS.VAP - 200 max  -  1 thru 60, 150 defined
//
//        constant name								index       standard recorded verbiage
//      --------------------------------			---------   -------------------------------	
#define VOX_DIGITS_zero								(  1 - 1)	//  zero
#define VOX_DIGITS_one								(  2 - 1)	//  one
#define VOX_DIGITS_two								(  3 - 1)	//  two
#define VOX_DIGITS_three							(  4 - 1)	//  three
#define VOX_DIGITS_four								(  5 - 1)	//  four
#define VOX_DIGITS_five								(  6 - 1)	//  five
#define VOX_DIGITS_six								(  7 - 1)	//  six
#define VOX_DIGITS_seven							(  8 - 1)	//  seven
#define VOX_DIGITS_eight							(  9 - 1)	//  eight
#define VOX_DIGITS_nine								( 10 - 1)	//  nine
#define VOX_DIGITS_ten								( 11 - 1)	//  ten
#define VOX_DIGITS_eleven							( 12 - 1)	//  eleven
#define VOX_DIGITS_twelve							( 13 - 1)	//  twelve
#define VOX_DIGITS_thirteen							( 14 - 1)	//  thirteen
#define VOX_DIGITS_fourteen							( 15 - 1)	//  fourteen
#define VOX_DIGITS_fifteen							( 16 - 1)	//  fifteen
#define VOX_DIGITS_sixteen							( 17 - 1)	//  sixteen
#define VOX_DIGITS_seventeen						( 18 - 1)	//  seventeen
#define VOX_DIGITS_eightteen						( 19 - 1)	//  eightteen
#define VOX_DIGITS_nineteen							( 20 - 1)	//  nineteen
#define VOX_DIGITS_twenty							( 21 - 1)	//  twenty
#define VOX_DIGITS_thirty							( 22 - 1)	//  thirty
#define VOX_DIGITS_forty							( 23 - 1)	//  forty
#define VOX_DIGITS_fifty							( 24 - 1)	//  fifty
#define VOX_DIGITS_sixty							( 25 - 1)	//  sixty
#define VOX_DIGITS_seventy							( 26 - 1)	//  seventy
#define VOX_DIGITS_eighty							( 27 - 1)	//  eighty
#define VOX_DIGITS_ninety							( 28 - 1)	//  ninety
#define VOX_DIGITS_hundred							( 29 - 1)	//  hundred
#define VOX_DIGITS_thousand							( 30 - 1)	//  thousand
#define VOX_DIGITS_million							( 31 - 1)	//  million
#define VOX_DIGITS_and								( 32 - 1)	//  and
#define VOX_DIGITS_a_m								( 33 - 1)	//  A.M.
#define VOX_DIGITS_p_m								( 34 - 1)	//  P.M.
#define VOX_DIGITS_sunday							( 35 - 1)	//  sunday
																//   first one. see #42
#define VOX_DIGITS_monday							( 36 - 1)	//  monday
#define VOX_DIGITS_tuesday							( 37 - 1)	//  tuesday
#define VOX_DIGITS_wednesday						( 38 - 1)	//  wednesday
#define VOX_DIGITS_thursday							( 39 - 1)	//  thursday
#define VOX_DIGITS_friday							( 40 - 1)	//  friday
#define VOX_DIGITS_saturday							( 41 - 1)	//  saturday
#define VOX_DIGITS_Sunday							( 42 - 1)	//  sunday
																//   again. see #35
#define VOX_DIGITS_january							( 43 - 1)	//  january
#define VOX_DIGITS_february							( 44 - 1)	//  february
#define VOX_DIGITS_march							( 45 - 1)	//  march
#define VOX_DIGITS_april							( 46 - 1)	//  april
#define VOX_DIGITS_may								( 47 - 1)	//  may
#define VOX_DIGITS_june								( 48 - 1)	//  june
#define VOX_DIGITS_july								( 49 - 1)	//  july
#define VOX_DIGITS_august							( 50 - 1)	//  august
#define VOX_DIGITS_september						( 51 - 1)	//  september
#define VOX_DIGITS_october							( 52 - 1)	//  october
#define VOX_DIGITS_november							( 53 - 1)	//  november
#define VOX_DIGITS_december							( 54 - 1)	//  december
#define VOX_DIGITS_dollars							( 55 - 1)	//  dollars
#define VOX_DIGITS_cents							( 56 - 1)	//  cents
#define VOX_DIGITS_message_arrived					( 57 - 1)	//  message arrived
#define VOX_DIGITS_at								( 58 - 1)	//  at
#define VOX_DIGITS_O								( 59 - 1)	//  'O'
																//   the letter 'O'
#define VOX_DIGITS_thank_you_goodbye				( 60 - 1)	//  thank-you, good-bye.
#define VOX_DIGITS_PROMPT_061						( 61 - 1)	//	"EMPTY"

#define VOX_DIGITS_Silent_Prompt					(150 - 1)	//  a "silent" prompt, all VAP files 
																//   have this number reserved.
																//   to be used when a prompt is to be supressed.

#define VOX_DIGITS_PROMPT_200						(200 - 1)	//	"EMPTY"


// XTPG2.VAP - 150 max  -  1 thru 109, and 150 defined

#define VOX_PROMPT_zero								(  1 - 1)	//  zero
#define VOX_PROMPT_one								(  2 - 1)	//  one
#define VOX_PROMPT_two								(  3 - 1)	//  two
#define VOX_PROMPT_three							(  4 - 1)	//  three
#define VOX_PROMPT_four								(  5 - 1)	//  four
#define VOX_PROMPT_five								(  6 - 1)	//  five
#define VOX_PROMPT_six								(  7 - 1)	//  six
#define VOX_PROMPT_seven							(  8 - 1)	//  seven
#define VOX_PROMPT_eight							(  9 - 1)	//  eight
#define VOX_PROMPT_nine								( 10 - 1)	//  nine
#define VOX_PROMPT_star								( 11 - 1)	//  star
#define VOX_PROMPT_pound_sign						( 12 - 1)	//  pound sign
#define VOX_PROMPT_pager_I_D						( 13 - 1)	//  ...pager I. D. ...
#define VOX_PROMPT_please_dial_your_				( 14 - 1)	//  please dial your callback  
																//   number followed by pound sign.
#define VOX_PROMPT_thank_you_good_bye				( 15 - 1)	//  thank-you, good-bye.
#define VOX_PROMPT_voice_paging_unavailable			( 16 - 1)	//  voice paging unavailable
#define VOX_PROMPT_followed_by_pound_sign			( 17 - 1)	//  ...followed by pound sign.
#define VOX_PROMPT_is_invalid_please				( 18 - 1)	//  ...is invalid, please re-enter 
																//   or press pound sign to quit.
#define VOX_PROMPT_invalid							( 19 - 1)	//  invalid...
#define VOX_PROMPT_i_d_number						( 20 - 1)	//  ...I. D. number...
#define VOX_PROMPT_please_enter						( 21 - 1)	//  please enter...
#define VOX_PROMPT_enter_month_and_day_				( 22 - 1)	//  enter month and day followed by 
																//   pound sign, for tomorrow, press 
																//   pound sign, for today press 
																//   star pound sign.
#define VOX_PROMPT_to_page							( 23 - 1)	//  ...to page...
#define VOX_PROMPT_press_star_to_change				( 24 - 1)	//  press star to change...
#define VOX_PROMPT_or_for_the_operator				( 25 - 1)	//  ...or for the operator press 
																//   zero
#define VOX_PROMPT_you_do_not_have_a_security_code	( 26 - 1)	//  you do not have a security 
																//   code...
#define VOX_PROMPT_or_press_						( 27 - 1)	//  ...or press...
#define VOX_PROMPT_operator_page					( 28 - 1)	//  operator page...
#define VOX_PROMPT_text_message						( 29 - 1)	//  ...text message...
#define VOX_PROMPT_enter_security_code_followed_	( 30 - 1)	//  enter security code followed 
																//   by pound sign
#define VOX_PROMPT_incorrect_security_				( 31 - 1)	//  incorrect security code
#define VOX_PROMPT_unavailable						( 32 - 1)	//  ...unavailable...
#define VOX_PROMPT_coverage_change					( 33 - 1)	//  ...coverage change...
#define VOX_PROMPT_4_seconds_of_beep_tone			( 34 - 1)	//  "4 seconds of beep tone, 1465 Hertz"
#define VOX_PROMPT_enter_time_followed_by_			( 35 - 1)	//  enter time followed by pound sign
#define VOX_PROMPT_critical_alert_line_				( 36 - 1)	//  critical alert line, indicate
																//   emergency and location now.
#define VOX_PROMPT_speak_now						( 37 - 1)	//  speak now
#define VOX_PROMPT_paging							( 38 - 1)	//  paging...
#define VOX_PROMPT_has_no_pager_					( 39 - 1)	//  ...has no pager at this time, dial 
																//   the operator for further assistance
#define VOX_PROMPT_press_one_to_play_previous_		( 40 - 1)	//  press one to play previous message
#define VOX_PROMPT_press_two_to_repeat_current_		( 41 - 1)	//  press two to repeat current message
#define VOX_PROMPT_press_three_to_play_next_		( 42 - 1)	//  press three to play next message
#define VOX_PROMPT_status_is_out_					( 43 - 1)	//  status is out, not available, 
																//   please hang up
#define VOX_PROMPT_room_number						( 44 - 1)	//  ...room number...
#define VOX_PROMPT_to_remain_on_the_line_			( 45 - 1)	//  ...to remain on the line while your 
																//   party is being paged
#define VOX_PROMPT_current							( 46 - 1)	//  current...
#define VOX_PROMPT_to_leave_a_voice_mail_			( 47 - 1)	//  ...to leave a voice mail message...
#define VOX_PROMPT_transferring_to_					( 48 - 1)	//  ...transferring to voice mail
#define VOX_PROMPT_extension						( 49 - 1)	//  extension
#define VOX_PROMPT_code_aborted						( 50 - 1)	//  code aborted
#define VOX_PROMPT_code_completed					( 51 - 1)	//  code completed
#define VOX_PROMPT_call								( 52 - 1)	//  call...
#define VOX_PROMPT_unable_to_set_schedule			( 53 - 1)	//  unable to set schedule
#define VOX_PROMPT_press							( 54 - 1)	//  press...
#define VOX_PROMPT_to_change						( 55 - 1)	//  ...to change...
#define VOX_PROMPT_to_page_by_name					( 56 - 1)	//  ...to page by name
#define VOX_PROMPT_page_status						( 57 - 1)	//  ...page status...
#define VOX_PROMPT_greeting							( 58 - 1)	//  ...greeting...
#define VOX_PROMPT_covered_by						( 59 - 1)	//  ...covered by...
#define VOX_PROMPT_is								( 60 - 1)	//  ...is...
#define VOX_PROMPT_list								( 61 - 1)	//  list...
#define VOX_PROMPT_please_call						( 62 - 1)	//  ...please call...
#define VOX_PROMPT_enter_covering					( 63 - 1)	//  enter covering...
#define VOX_PROMPT_already_covered_by				( 64 - 1)	//  ...already covered by...
#define VOX_PROMPT_coverage							( 65 - 1)	//  ...coverage...
#define VOX_PROMPT_recursive_coverage				( 66 - 1)	//  ...recursive coverage
#define VOX_PROMPT_to_repeat_menu					( 67 - 1)	//  ...to repeat menu
#define VOX_PROMPT_enter_new_security_				( 68 - 1)	//  enter new security code followed 
																//   by pound sign
#define VOX_PROMPT_press_							( 69 - 1)	//  ...press...
#define VOX_PROMPT_operator							( 70 - 1)	//  ...operator...
#define VOX_PROMPT_please_dial_your					( 71 - 1)	//  please dial your...
#define VOX_PROMPT_hello							( 72 - 1)	//  hello...
#define VOX_PROMPT_to_try_another_pager_i_d_press	( 73 - 1)	//  ...to try another pager I. D. press...
#define VOX_PROMPT_press_one_to_retrieve_			( 74 - 1)	//  press one to retrieve undelivered messages
#define VOX_PROMPT_press_two_to_retrieve_			( 75 - 1)	//  ...press two to retrieve all messages
#define VOX_PROMPT_no_messages						( 76 - 1)	//  ...no messages...
#define VOX_PROMPT_no_more_messages					( 77 - 1)	//  ...no more messages...
#define VOX_PROMPT_to_retrieve_messages				( 78 - 1)	//  ...to retrieve messages
#define VOX_PROMPT_security_code					( 79 - 1)	//  ...security code...
#define VOX_PROMPT_please_wait_retrieving_messages	( 80 - 1)	//  please wait, retrieving messages
#define VOX_PROMPT_text_messages_					( 81 - 1)	//  text messages available, dial zero to 
																//   transfer to operator.
																//  old message #80 moved to prompt #112
#define VOX_PROMPT_press_pound_sign_to_return_		( 82 - 1)	//  press pound sign to return to main menu
#define VOX_PROMPT_press_pound_sign_quit			( 83 - 1)	//  ...press pound sign to quit
#define VOX_PROMPT_speak_now_then					( 84 - 1)	//  speak now, then press pound
#define VOX_PROMPT_repeat_menu						( 85 - 1)	//  ...repeat menu
#define VOX_PROMPT_press_star_to_page_				( 86 - 1)	//  press star to page your party and remain on the line
#define VOX_PROMPT_please_remain_on_the_			( 87 - 1)	//  please remain on the line, your party is being paged
#define VOX_PROMPT_sorry_unable_to_connect_			( 88 - 1)	//  sorry, unable to connect your party, 
																//   dial operator for further assistance
#define VOX_PROMPT_sorry_unable_to_					( 89 - 1)	//  sorry, unable to connect your party...
#define VOX_PROMPT_enter_one_for_A_M_				( 90 - 1)	//  enter one for A.M., two for P.M.
#define VOX_PROMPT_home								( 91 - 1)	//  ...home...
#define VOX_PROMPT_back								( 92 - 1)	//  ...back...
#define VOX_PROMPT_emergency						( 93 - 1)	//  ...emergency...
#define VOX_PROMPT_number							( 94 - 1)	//  ...number...
#define VOX_PROMPT_operating_room					( 95 - 1)	//  ...operating room...
#define VOX_PROMPT_stat								( 96 - 1)	//  ...stat...
#define VOX_PROMPT_page								( 97 - 1)	//  ...page...
#define VOX_PROMPT_press_the_first					( 98 - 1)	//  press the first...
#define VOX_PROMPT_transferring_to_operator			( 99 - 1)	//  ...transferring to operator
#define VOX_PROMPT_silence_for_half_a_second		(100 - 1)	//  "silence for half a second"
#define VOX_PROMPT_to_connect_to					(101 - 1)	//  ...to connect to...
#define VOX_PROMPT_forwarding_number				(102 - 1)	//  ...forwarding number...
#define VOX_PROMPT_digits_of_the_persons_last_name	(103 - 1)	//  ...digits of the person's last name, 
																//   for Q or Z press zero.
#define VOX_PROMPT_PROMPT_104						(104 - 1)	//  "EMPTY"
#define VOX_PROMPT_PROMPT_105						(105 - 1)	//  "EMPTY"
#define VOX_PROMPT_PROMPT_106						(106 - 1)	//  "EMPTY"
#define VOX_PROMPT_PROMPT_107						(107 - 1)	//  "EMPTY"
#define VOX_PROMPT_enter_pager_I_D_					(108 - 1)	//  enter pager I. D. to page, followed by pound sign
#define VOX_PROMPT_for_page_by_name_				(109 - 1)	//  ...for page by name, enter...
#define VOX_PROMPT_PROMPT_110						(110 - 1)	//	"EMPTY"	
#define VOX_PROMPT_to_try_another_extension_press	(111 - 1)	//  ...to try another extension press...
#define VOX_PROMPT_please_call_page_operator_for_  	(112 - 1)	//  ...Please call page operator for assistance...
			
#define VOX_PROMPT_Silent_Prompt					(150 - 1)	//  a "silent" prompt, all VAP files 
																//   have this number reserved.
																//   to be used when a prompt is to be supressed.


// XTPGSTAT.VAP - 150 max  -  1 thru 119, and 150 defined 

#define VOX_STATUS_zero								(  1 - 1)	//  zero
#define VOX_STATUS_one								(  2 - 1)	//  one
#define VOX_STATUS_two								(  3 - 1)	//  two
#define VOX_STATUS_three							(  4 - 1)	//  three
#define VOX_STATUS_four								(  5 - 1)	//  four
#define VOX_STATUS_five								(  6 - 1)	//  five
#define VOX_STATUS_six								(  7 - 1)	//  six
#define VOX_STATUS_seven							(  8 - 1)	//  seven
#define VOX_STATUS_eight							(  9 - 1)	//  eight
#define VOX_STATUS_nine								( 10 - 1)	//  nine
#define VOX_STATUS_available_statuses_are			( 11 - 1)	//  available statuses are...
#define VOX_STATUS_enter_new_status_number_or		( 12 - 1)	//  enter new status number or...
#define VOX_STATUS_current_status_is				( 13 - 1)	//  current status is
#define VOX_STATUS_in_hospital_page					( 14 - 1)	//  in hospital page emergency only
#define VOX_STATUS_in_hospital_not					( 15 - 1)	//  in hospital not available
#define VOX_STATUS_out_of_hospital_on				( 16 - 1)	//  out of hospital on page
#define VOX_STATUS_out_of_hospital_page				( 17 - 1)	//  out of hospital page emergency only
#define VOX_STATUS_out_of_hospital_not				( 18 - 1)	//  out of hospital not available
#define VOX_STATUS_referred_to_other				( 19 - 1)	//  referred to other pager
#define VOX_STATUS_referred_to_office				( 20 - 1)	//  referred to office or answering service
#define VOX_STATUS_unavailable_hold					( 21 - 1)	//  unavailable, hold messages
#define VOX_STATUS_unavailable_call_page_			( 22 - 1)	//  unavailable, call page operator
#define VOX_STATUS_refer_calls_to_office			( 23 - 1)	//  refer calls to office
#define VOX_STATUS_refer_calls_to_answering_		( 24 - 1)	//  refer calls to answering service
#define VOX_STATUS_available_for_page				( 25 - 1)	//  available for page
#define VOX_STATUS_to_repeat_choices				( 26 - 1)	//  ...to repeat choices
#define VOX_STATUS_not_available					( 27 - 1)	//  not available
#define VOX_STATUS_avaialable_wide_area_			( 28 - 1)	//  avaialable wide area page
#define VOX_STATUS_unavailable_messages_			( 29 - 1)	//  unavailable, messages being stored
#define VOX_STATUS_available_emergency_only			( 30 - 1)	//  available emergency only
#define VOX_STATUS_not_avaialabe_messages_being		( 31 - 1)	//  not avaialabe, messages being stored
#define VOX_STATUS_invalid							( 32 - 1)	//  invalid
#define VOX_STATUS_unavailable						( 33 - 1)	//  unavailable
#define VOX_STATUS_in_hospital_on_page				( 34 - 1)	//  in hospital, on page
#define VOX_STATUS_in_surgery_messages_being		( 35 - 1)	//  in surgery, messages being stored
#define VOX_STATUS_available_on_pager				( 36 - 1)	//  available on pager
#define VOX_STATUS_unavailable_on_pager				( 37 - 1)	//  unavailable on pager
#define VOX_STATUS_reachable_at						( 38 - 1)	//  reachable at...
#define VOX_STATUS_calls_being_taken_by				( 39 - 1)	//  calls being taken by...
#define VOX_STATUS_available_by_overhead_paging		( 40 - 1)	//  available by overhead paging, call
																//   two, zero, one, four, three to page. 
#define VOX_STATUS_silence_for_half_a_second		( 41 - 1)	//  "silence for half a second"
#define VOX_STATUS_in_office_on_extension			( 42 - 1)	//  ...in office on extension...
#define VOX_STATUS_in_office_calls_to_voice_mail	( 43 - 1)	//  ...in office, all calls to voice mail
#define VOX_STATUS_in_office_calls_to_operator		( 44 - 1)	//  ...in office, all calls to operator
#define VOX_STATUS_unavailable_calls_to_voice_mail	( 45 - 1)	//  ...unavailable, all calls to voice mail...
#define VOX_STATUS_special_all_calls_to_operator	( 46 - 1)	//  ...special instructions, all calls to operator...
#define VOX_STATUS_home_office						( 47 - 1)	//  home office...
#define VOX_STATUS_press							( 48 - 1)	//  ...press...
#define VOX_STATUS_to_change_security_code			( 49 - 1)	//  ...to change security code...
#define VOX_STATUS_in_O_R_holding_messages			( 50 - 1)	//  in 'O' 'R' holding messages
#define VOX_STATUS_on_vacation						( 51 - 1)	//  on vacation
#define VOX_STATUS_referred_to_another_number		( 52 - 1)	//  referred to another number
#define VOX_STATUS_available_at_the_V_A_hospital	( 53 - 1)	//  available at the 'V' 'A' hospital
#define VOX_STATUS_status_change					( 54 - 1)	//  ...status change...
#define VOX_STATUS_press_pound_sign_to_leave		( 55 - 1)	//  ...press pound sign to leave unchanged
#define VOX_STATUS_n clinic_on_page					( 56 - 1)	//  ...in clinic on page...
#define VOX_STATUS_refer_to_phone_number			( 57 - 1)	//  ...refer to phone number or extension...
#define VOX_STATUS_out_of_hospital_at_home			( 58 - 1)	//  ...out of hospital, at home...
#define VOX_STATUS_in_surgery_not_available			( 59 - 1)	//  in surgery, not available
#define VOX_STATUS_out_of_hospital_wide_area		( 60 - 1)	//  out of hospital, wide area
#define VOX_STATUS_in_hospital_no_pager				( 61 - 1)	//  in hospital, no pager
#define VOX_STATUS_in_meeting_until_further_notice	( 62 - 1)	//  in meeting until further notice
#define VOX_STATUS_unavailable_call_page_operator_	( 63 - 1)	//  unavailable, call page operator at four two six two
#define VOX_STATUS_in_hospital_available_overhead	( 64 - 1)	//  in hospital, available by overhead paging
#define VOX_STATUS_in_page_emergency_only			( 65 - 1)	//  in page emergency only...
#define VOX_STATUS_out_on_page						( 66 - 1)	//  out on page
#define VOX_STATUS_out_page_emergency_only			( 67 - 1)	//  out page emergency only
#define VOX_STATUS_in_on_page						( 68 - 1)	//  ...in on page...
#define VOX_STATUS_available_for_paging_at_U_M_M_S	( 69 - 1)	//  available for paging at 'U' 'M' 'M' S'
#define VOX_STATUS_available_overhead_in_general_	( 70 - 1)	//  available overhead in general hospital
#define VOX_STATUS_available_overhead_in_shock		( 71 - 1)	//  available overhead in shock trauma
#define VOX_STATUS_available_overhead_in_V_A_		( 72 - 1)	//  available overhead in 'V' 'A' hospital
#define VOX_STATUS_unavailable_until				( 73 - 1)	//  ...unavailable until...
#define VOX_STATUS_not_on_call						( 74 - 1)	//  ...not on call...
#define VOX_STATUS_available_for_paging_at_the_V_A_	( 75 - 1)	//  ...available for paging at the 'V' 'A' hospital...
#define VOX_STATUS_being_covered_by					( 76 - 1)	//  ...being covered by...
#define VOX_STATUS_available_overhead_in_radiology	( 77 - 1)	//  ...available overhead in radiology...
#define VOX_STATUS_in_at_Weiss						( 78 - 1)	//  ...in at Weiss...
#define VOX_STATUS_page_emergency_only__stored		( 79 - 1)	//  ...page emergency only, message being stored. 
#define VOX_STATUS_in_at_Darien_or_Bolingbrook		( 80 - 1)	//  ...in, at Darien or Bolingbrook...
#define VOX_STATUS_out_long_range					( 81 - 1)	//  ...out, long range...
#define VOX_STATUS_messages_being_stored_not		( 82 - 1)	//  ...messages being stored, not available...
#define VOX_STATUS_page_emergency_only_message_		( 83 - 1)	//  page emergency only, message being stored. 
																//   if this is an emergency, hang up and dial 
																//    seven, zero, two, six, eight, zero, zero to page, 
																//   otherwise your message will be stored.
#define VOX_STATUS_not_available_no_messages		( 84 - 1)	//  not available, no messages being stored
#define VOX_STATUS_calls_are_being_referred_to		( 85 - 1)	//  calls are being referred to...
#define VOX_STATUS_referred_to						( 86 - 1)	//  ...referred to...
#define VOX_STATUS_page_emergency_only				( 87 - 1)	//  ...page emergency only...
#define VOX_STATUS_covered_by						( 88 - 1)	//  ...covered by...
#define VOX_STATUS_not_on_hospital_page				( 89 - 1)	//  not on hospital page
#define VOX_STATUS_available_at_Fairfax_hospital	( 90 - 1)	//  available at Fairfax hospital
#define VOX_STATUS_available_at_Fair_Oaks_hospital	( 91 - 1)	//  available at Fair Oaks hospital
#define VOX_STATUS_available_at_Mount_Vernon		( 92 - 1)	//  available at Mount Vernon hospital
#define VOX_STATUS_in_hospital_on_page_or__			( 93 - 1)	//  in hospital on page, or avaialable by overhead paging
#define VOX_STATUS_not_a_hospital_pager				( 94 - 1)	//  ...not a hospital pager...
#define VOX_STATUS_page_emergency_only_messages__	( 95 - 1)	//  ...page emergency only, messages being stored. 
																//   if this is an emergency, hang up and dial 
																//   seven, seven, three, seven, zero, two, 
																//    one, zero, zero, zero, 
																//   otherwise your message will be stored
#define VOX_STATUS_PROMPT_096						( 96 - 1)	//  "EMPTY"
#define VOX_STATUS_PROMPT_097						( 97 - 1)	//  "EMPTY"
#define VOX_STATUS_PROMPT_098						( 98 - 1)	//  "EMPTY"
#define VOX_STATUS_PROMPT_099						( 99 - 1)	//  "EMPTY"
#define VOX_STATUS_ten								(100 - 1)	//  ten
#define VOX_STATUS_eleven							(101 - 1)	//  eleven
#define VOX_STATUS_twelve							(102 - 1)	//  twelve
#define VOX_STATUS_thirteen							(103 - 1)	//  thirteen
#define VOX_STATUS_fourteen							(104 - 1)	//  fourteen
#define VOX_STATUS_fifteen							(105 - 1)	//  fifteen
#define VOX_STATUS_sixteen							(106 - 1)	//  sixteen
#define VOX_STATUS_seventeen						(107 - 1)	//  seventeen
#define VOX_STATUS_eighteen							(108 - 1)	//  eighteen
#define VOX_STATUS_nineteen							(109 - 1)	//  nineteen
#define VOX_STATUS_twenty							(110 - 1)	//  twenty
#define VOX_STATUS_star     						(111 - 1)	//  same as VOX_PROMPT_star - # 10.
#define VOX_STATUS_pound_sign						(112 - 1)	//  same as VOX_PROMPT_pound_sign - # 11.
#define VOX_STATUS_PROMPT_113						(113 - 1)	//  "EMPTY"
#define VOX_STATUS_PROMPT_114						(114 - 1)	//  "EMPTY"
#define VOX_STATUS_PROMPT_115						(115 - 1)	//  "EMPTY"
#define VOX_STATUS_PROMPT_116						(116 - 1)	//  "EMPTY"
#define VOX_STATUS_PROMPT_117						(117 - 1)	//  "EMPTY"
#define VOX_STATUS_PROMPT_118						(118 - 1)	//  "EMPTY"

#define VOX_STATUS_in_at_larabida					(119 - 1)	//  in at larabida.      
																//   "custom University of Chicago - UCH 2001-07-31"

#define VOX_STATUS_Available_at_Inova_Alexandria_	(120 - 1)	//	"Available at Inova Alexandria Hospital"	
																//   "custom for Inova - Falls Church, VA 2003-04-02"
			
#define VOX_STATUS_unavailable_on_pager_2			(121 - 1)	//	"Unavailable on page" same as #(37- 1)
																//   but with no pageserver voice-page logic attached.

#define VOX_STATUS_available_for_medical_staff_only	(122 - 1)	//	"Available for medical staff only
																//   added for customer CHOB, Boston.

#define VOX_STATUS_Available_at_Inova_Loudon_Hosp	(123 - 1)	//	"Available at Inova Loudon Hospital.
																//  "custom for Inova - Falls Church, VA 2006-05-24".

#define VOX_STATUS_Not_Pageable_Via_Sophia			(124 - 1)	//	


#define VOX_STATUS_Silent_Prompt					(150 - 1)	//  a "silent" prompt, all VAP files 
																//   have this number reserved.
																//   to be used when a prompt is to be supressed.
#endif //_defines_h_