
// Registry43.h

#pragma once

extern unsigned long GetRegistryULong(CString, CString);
extern void SetRegistryULong(CString, CString, unsigned long);

extern CString GetRegistryStringValue(CString, CString);
extern void SetRegistryStringValue(CString, CString, CString);

