#include "common.h"
#include "List2.h"
#include <stdlib.h>

#include <io.h>
#include <fcntl.h>
#include <share.h>
#include <sys\locking.h>	// needed by list2.c
#include <sys\stat.h>
#include <sys\types.h>

#include <cctype>
#include <iostream>
#include <sstream>
#include <exception>
#include <string>


// disable deprecation
#pragma warning(disable:4996)



// list2.c

// included in all 3 implementations of page server.

using namespace std;


string GetLookupDir(const string& fname)
{
	// this is used to map to the /n0/n1 subdirectory 
	//  to store / retrieve the "custom greeting" .vce file.
	//
	string::const_iterator pos(fname.end());
	string dir;

	for(pos -=2; pos != fname.end(); ++pos)
	{
		if(!(isdigit(*pos) || *pos >= 'A')) continue;

		dir += 'N';
		if(*pos >= 'A')
		{
			dir += static_cast<char>(((*pos - 'A') % 10) +'0');
		}
		else
		{
			dir += *pos;
		}
		dir += '\\';
	}
	return dir;
}


	void getfile(char* fnameIn, char* fileExtIn, string::size_type size)
	{
	string fname(fnameIn);
	ostringstream fullPath;

	//trim right
	fname.erase(fname.find_last_not_of(" \r\n\t") + 1 );
#if 1
	PrintMessage(MSGTYPE_IVR, 
					hwndD4xMsg, 
					"getfile() - received(1): [%s]:", 
					fname.c_str());
#endif
	fullPath << pageFileDir << GetLookupDir(fname) << fname << '\\' << fname << '.' << fileExtIn << ends;

	//if(fullPath.str().length() > size) throw length_error("Get filename buffer too small");

	string pathName(fullPath.str());


	PrintMessage(MSGTYPE_IVR, 
					hwndD4xMsg, 
					"getfile() - received(2): [%s]:", 
					pathName.c_str());


	if(pathName.length() > size)
		{
		PrintMessage(MSGTYPE_IVR, 
			hwndD4xMsg, 
			"getfile() - destination buffer too small\n[%s]", pathName.c_str());
		
		//throw length_error("Get filename buffer too small");
		pathName = pathName.erase(size - 1);
		}

	strncpy(fnameIn, pathName.c_str(), size);
	fnameIn[size - 1] = 0;

	PrintMessage(MSGTYPE_IVR, 
					hwndD4xMsg, 
					"getfile() - returned: [%s]:", 
					fnameIn);
	}



	void 
GetVceFile(char* fname, int size)
	{
	getfile(fname, "VCE", size);
	}


	int
WriteStatistics( char *Date, char *Hour )
	{
    int j;
	int fh;
    char StatFile[85];
    StatisticStruct StatBuf;

    if(DATA_SOURCE_LOCAL == DataSource)
		{
		return 0;
		}

    memset(StatFile, 0, sizeof(StatFile));
    sprintf(StatFile, "%s\\XTPG%4.4s.STA", param.AutoDir, Date);

    PrintMessage(MSGTYPE_SYSTEM,  
					hwndGeneralMsg, 
					"Writing statistics file: %s.", 
					StatFile);

    fh = _sopen(StatFile, 
				_O_CREAT | _O_WRONLY | _O_BINARY, 
				_SH_DENYWR, 
				_S_IREAD | _S_IWRITE);

    if(fh != -1)
		{
        _lseek(fh, 0L, SEEK_END);

        memset( &StatBuf, 0, sizeof( StatBuf ) );
        sprintf( StatBuf.Date, "%6.6s", Date );
        sprintf( StatBuf.Time, "%2.2s00", Hour );
        sprintf( StatBuf.PSId, "%3.3s", param.stationId );

        for(j = 0; j < totalD4xChannels; j++)
			{
            if( port[ j ].Attempts > 0 )
				{
                sprintf( StatBuf.Type, "D%2.2i", j );
                StatBuf.bucket[ 0 ] = (unsigned short) port[ j ].Attempts;
                StatBuf.bucket[ 1 ] = (unsigned short) port[ j ].External;
                StatBuf.bucket[ 2 ] = (unsigned short) port[ j ].Errors;
                StatBuf.bucket[ 3 ] = (unsigned short) port[ j ].Cancels;
                StatBuf.bucket[ 4 ] = 0;
                StatBuf.bucket[ 5 ] = 0;
                StatBuf.bucket[ 6 ] = 0;
                StatBuf.bucket[ 7 ] = 0;
                _write( fh, &StatBuf, sizeof( StatBuf ) );
				}
            port[ j ].Attempts = 0;
            port[ j ].External = 0;
            port[ j ].Errors = 0;
            port[ j ].Cancels = 0;
			}

       for(j = 0; j < MAX_QUEUE_FILES; j++)
			{
            if( Queue[ j ].Attempts > 0 )
				{
                sprintf( StatBuf.Type, "%3.3s", Queue[ j ].Id );
                StatBuf.bucket[ 0 ] = (unsigned short) Queue[ j ].Attempts;
                StatBuf.bucket[ 1 ] = (unsigned short) Queue[ j ].External;
                StatBuf.bucket[ 2 ] = (unsigned short) Queue[ j ].Errors;
                StatBuf.bucket[ 3 ] = (unsigned short) Queue[ j ].Cancels;
                StatBuf.bucket[ 4 ] = 0;
                StatBuf.bucket[ 5 ] = 0;
                StatBuf.bucket[ 6 ] = 0;
                StatBuf.bucket[ 7 ] = 0;
                _write( fh, &StatBuf, sizeof( StatBuf ) );
				}
            Queue[ j ].Attempts = 0;
            Queue[ j ].External = 0;
            Queue[ j ].Errors = 0;
            Queue[ j ].Cancels = 0;
			}

        for(j = 0; j < MAX_COMMPORTS; j++)
			{
            if( ComPort[ j ].Attempts > 0 )
				{
                sprintf( StatBuf.Type, "C%2.2i", j+1 );
                StatBuf.bucket[ 0 ] = (unsigned short) ComPort[ j ].Attempts;
                StatBuf.bucket[ 1 ] = (unsigned short) ComPort[ j ].External;
                StatBuf.bucket[ 2 ] = (unsigned short) ComPort[ j ].Errors;
                StatBuf.bucket[ 3 ] = (unsigned short) ComPort[ j ].Cancels;
                StatBuf.bucket[ 4 ] = 0;
                StatBuf.bucket[ 5 ] = 0;
                StatBuf.bucket[ 6 ] = 0;
                StatBuf.bucket[ 7 ] = 0;
                _write( fh, &StatBuf, sizeof( StatBuf ) );
				}
            ComPort[ j ].Attempts = 0;
            ComPort[ j ].External = 0;
            ComPort[ j ].Errors = 0;
            ComPort[ j ].Cancels = 0;
			}

        _close( fh );
		}
    else
		{
        PrintMessage(MSGTYPE_SYSTEM,  
						hwndGeneralMsg, 
						"ERROR !!! Unable to sopen statistics file.");
        return( 1 );
		}
    return( 0 );
	}


	// when all requirements are met, returns ST_PARK_GET
	//  otherwise, returns ST_PARK_NG_MSG.
	//
	int
GetParkNumber(int PagePointer, int chan)
	{
    int ReturnValue = ST_PARK_NG_MSG;
    int fh;
    USHORT NextRecord;
    USHORT RecordCount;
    int offset;
	char fileName[60];
	int FileOperationStatus;
	char string2[200];

	// in failsafe mode... no park-meet is possible.
	//  return status indicating that all lines are in-use
	//  actually feature is unavailable, but to the user...
	//  the net effect is the same.
	//  
	if(bCmdLineFailSafeMode)
		{
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg,
						"FailSafe mode... Self-Park-And-Page is not available");
		return ReturnValue;
		}

	// the only currently implemented method for park-meet-me
	//  is the flat-file method.
	if(!param.NPark)
		{
		return ReturnValue;
		}

	// use MEETME.DAT flat-file.
	memset(fileName, 0, sizeof(fileName));
	sprintf(fileName, "%sMEETME.DAT", pageFileDir);

	fh = _sopen(fileName, _O_RDWR | _O_BINARY, _SH_DENYNO);

	PrintMessage(MSGTYPE_SYSTEM, 
					hwndGeneralMsg,
					"Open MEETME.DAT File: [%s], fh [%d]",
					fileName,
					fh);
	if(-1 != fh)
		{
		// file opened successfully.
		BOOL bLockedOK = FALSE;

		__try
			{
			_lseek(fh, 2L, SEEK_SET);
			if(0 == _locking(fh, LK_LOCK, 4L))
				{
				// bytes locked successfully.
				PrintMessage(MSGTYPE_SYSTEM, 
								hwndGeneralMsg, 
								"MEETME.DAT Locked");

				bLockedOK = TRUE;

				FileOperationStatus = _lseek(fh, 2L, SEEK_SET);

				if(FileOperationStatus != -1)
					{
					PrintMessage(MSGTYPE_SYSTEM, 
									hwndGeneralMsg,
									"file seek OK [%d]",
									FileOperationStatus);
					}

				FileOperationStatus = _read( fh, &NextRecord, 2 );

				if(FileOperationStatus != -1)
					{
					PrintMessage(MSGTYPE_SYSTEM, 
									hwndGeneralMsg,
									"file read NextRecord OK [%d]",
									FileOperationStatus);
					}

				FileOperationStatus = _read( fh, &RecordCount, 2 );

				if(FileOperationStatus != -1)
					{
					PrintMessage(MSGTYPE_SYSTEM, 
									hwndGeneralMsg,
									"file read RecordCount OK [%d]",
									FileOperationStatus);
					}

				PrintMessage(MSGTYPE_SYSTEM, 
								hwndGeneralMsg,
								"NextRecord [%u], RecordCount[%u]",
								NextRecord,
								RecordCount);

				if(RecordCount >= 1)
					{
					// there is an available line-pair for us to use.

					// get the record containing information about
					//  "transfer to" and "page message" numbers.
					offset = 46 + ( NextRecord * 46 );
					_lseek( fh, offset, SEEK_SET );
					memset(page[PagePointer].tempStr, 0, sizeof(page[PagePointer].tempStr));
					_read(fh, page[PagePointer].tempStr, 23);
					_read(fh, page[PagePointer].DisplayInfo, 23);

					_lseek(fh, offset, SEEK_SET);
					memset(string2, ' ', sizeof(string2));
					_write(fh, string2, 46);

					NextRecord++;
					NextRecord %= 48;	// keep in range 0 thru 47 using modulus function

					RecordCount--;

					_lseek(fh, 2L, SEEK_SET);
					_write(fh, &NextRecord, 2);
					_write(fh, &RecordCount, 2);

					Set_Null(page[PagePointer].tempStr,
								sizeof(page[PagePointer].tempStr));

					Set_Null(page[PagePointer].DisplayInfo,
								sizeof(page[PagePointer].DisplayInfo));

					PrintMessage(MSGTYPE_SYSTEM,  
								hwndGeneralMsg,
								"Park number A [%s], B [%s]",
								page[PagePointer].tempStr,
								page[PagePointer].DisplayInfo);

					if(page[PagePointer].DisplayInfo[0] == 0 || port[chan].digsave[0] == 0)
						{
						// no information in "transfer to" or "page message" fields.
						//  no sense to page anyone or transfer to a
						//  nonsense or non-existant number.
						PrintMessage(MSGTYPE_SYSTEM, 
										hwndGeneralMsg, 
										"'Transfer To' or 'Page Message' is empty");
						}
					else
						{
						// all ok, retrieved "transfer to" number,
						//  and "page message"
						PrintMessage(MSGTYPE_SYSTEM, 
										hwndGeneralMsg, 
										"GetParkNumber(): Success!");

						ReturnValue = ST_PARK_GET;
						}
					}
				else
					{
					PrintMessage(MSGTYPE_SYSTEM, 
									hwndGeneralMsg, 
									"No Park-Meet-Me lines available");
					}
				}
			else
				{
				// unable to lock.
				}

			PrintMessage(MSGTYPE_SYSTEM, 
							hwndGeneralMsg, 
							"Unable to Lock MEETME.DAT");
			}
		__finally
			{
			if(bLockedOK)
				{
				// unlock the locked bytes.
				bLockedOK = FALSE;

				// always unlock the file
				_lseek(fh, 2L, SEEK_SET);
				_locking(fh, LK_UNLCK, 4L);

				PrintMessage(MSGTYPE_SYSTEM, 
								hwndGeneralMsg, 
								"MEETME.DAT Unlocked");				
				}
			}
		}
	else
		{
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"Unable to open MEETME.DAT");
		}

	// close the file if found to be open.
	if(-1 != fh)
		{
		// file is open, close it now.
		PrintMessage(MSGTYPE_SYSTEM, 
						hwndGeneralMsg, 
						"MEETME.DAT Closed");				
		_close(fh);
		}

	return ReturnValue;
	}

