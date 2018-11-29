
// list2.c

// included in all 3 implementations of page server.
#pragma once

#include <string>


	// finds the directory the file is in. fname2 must be e.g 2000.log
//void getfile(char *fname2, char *fileExtn, int size=500);
void getfile(char *fname2, char *fileExtn, std::string::size_type size);

int WriteStatistics( char *Date, char *Hour );


	// when all requirements are met, returns ST_PARK_GET
	//  otherwise, returns ST_PARK_NG_MSG.
	//
int GetParkNumber(int PagePointer, int chan);


std::string GetLookupDir(const std::string& fname);

void GetVceFile(char* fname, int size);