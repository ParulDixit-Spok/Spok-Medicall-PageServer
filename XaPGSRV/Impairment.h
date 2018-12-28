
// Impairment.h

// page vendor impairment feature (MASCO, Boston, MA)


#if !defined(AFX_LOG_H__854EEF6C_06CB_4E54_A8EE_AF176FE49215__INCLUDED_)
#define AFX_LOG_H__854EEF6C_06CB_4E54_A8EE_AF176FE49215__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <atlsimpstr.h>
#include <atlstr.h>
#include "atlcoll.h"

typedef CAtlMap<CString,CString> CMapStringToString;

class Impairment
{
public:
	Impairment();
	~Impairment();
	void CheckImpairment(void);
	CString GetCustomGreetingProfileId(CString);
	bool IsImpairedVendor(CString);

	// "properties"
public:
	void setCheckInterval(int);
	void setProfileIdPattern(CString);
	void setStatusToEnable(CString);

private:
	// the impairment map maps PageDeviceType to AnnouncementProfileId
	CMapStringToString ImpairmentMap;

	void AddItemToMap(CString, CString);
	void ReadDatabaseItems(CString, CString);

	long m_iCheckInterval;
	SYSTEMTIME m_iImpairmentTimer;		// Timer for Impairment check
	CString m_csProfileIdPattern;
	CString m_csEnabledStatusCharacter;

	void ClearTheMap();
};


#endif // !defined(AFX_LOG_H__854EEF6C_06CB_4E54_A8EE_AF176FE49215__INCLUDED_)

