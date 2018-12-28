#if !defined(WINDEF__INCLUDED_)
#define WINDEF__INCLUDED_

#pragma once

#include <Windows.h>

void SendMessageToScreen(int MessageTypeMask, HWND hwndChildWinHandle, char * MessageText);
void SendMessageToLogfile(int MessageTypeMask, char * MessageText);
void PrintMessage(int MessageTypeMask, HWND hwndChildWinHandle, char * FormatString, ...);
void DumpMessage(int MessageTypeMask, HWND hwndChildWinHandle, char* msg);

#endif