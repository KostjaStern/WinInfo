/******************************************************************************
Module:  Debug.cpp
Notices: Copyright (c) 2014 Kostja Stern
******************************************************************************/

#include "Debug.h"



void Debug::printErrorMessage(DWORD dwError)
{
	HLOCAL hlocal = NULL;
    DWORD systemLocale = MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT);

    BOOL fOk = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, 
                             NULL, dwError, systemLocale, (PTSTR) &hlocal, 0, NULL);

    if (!fOk) 
	{
        // Is it a network-related error?
        HMODULE hDll = LoadLibraryEx(TEXT("netmsg.dll"), NULL, 
            DONT_RESOLVE_DLL_REFERENCES);

        if (hDll != NULL) {
            fOk = FormatMessage(
               FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS |
               FORMAT_MESSAGE_ALLOCATE_BUFFER,
               hDll, dwError, systemLocale,
               (PTSTR) &hlocal, 0, NULL);
            FreeLibrary(hDll);
        }
    }

    if (fOk && (hlocal != NULL)) {
        _tprintf(_T("%s\n"), (LPTSTR)hlocal);
        LocalFree(hlocal);
    } else {
		_tprintf(_T("%s\n"), _T("No text found for this error number."));
    }
}
