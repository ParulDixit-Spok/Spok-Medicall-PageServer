
#pragma once 

#include "common.h"

extern HWND XnCreateChild(LPSTR lpName);
extern HWND XnCreateStatus(LPSTR lpName, int x, int y, int cx, int cy);
extern void Log_Message(int fileHandle, unsigned long * msgPosition, char * message);


