
#include "ProcessInfo.h"


ProcessInfo::ProcessInfo(DWORD pID)
{
	this->PID = pID;

	// http://msdn.microsoft.com/en-us/library/windows/desktop/ms684880%28v=vs.85%29.aspx (Process Security and Access Rights)
	HANDLE hProcess = OpenProcess(READ_CONTROL | PROCESS_QUERY_INFORMATION | PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ, 
                                  FALSE,
								  pID);

	if(hProcess == NULL)
	{
		DWORD dwError = GetLastError();
		_tprintf(_T("OpenProcess: dwError = %i\n"), dwError);
		Debug::printErrorMessage(dwError);

		fileName = new TCHAR[1];
		fileName[0] = _T('\0');

		cmdParams = new TCHAR[1];
		cmdParams[0] = _T('\0');

		return;
	}


	fileName = new TCHAR[MAX_PATH + 1];
	if(!GetModuleFileNameEx((HMODULE)hProcess, NULL, fileName, MAX_PATH + 1))
	{
		DWORD dwError = GetLastError();
		_tprintf(_T("GetModuleFileNameEx: dwError = %i\n"), dwError);
		Debug::printErrorMessage(dwError);

		fileName[0] = _T('\0');
	}

	cmdParams = new TCHAR[MAX_PATH + 1];
	if(!GetProcessCmdLine(hProcess, cmdParams, MAX_PATH + 1)){
		cmdParams[0] = _T('\0');
	}

	CloseHandle(hProcess);
}


ProcessInfo::~ProcessInfo()
{
	delete [] fileName;
	delete [] cmdParams;
}


NTSTATUS ProcessInfo::_NtQueryInformationProcess(HANDLE hProcess, PROCESSINFOCLASS pic, PVOID pPI, ULONG cbSize, PULONG pLength)
{
    HMODULE hNtDll = LoadLibrary(_T("ntdll.dll"));
    if (hNtDll == NULL) {
        return(-1);
    }

    NTSTATUS lStatus = -1;  // error by default.

    // Note that function name is not UNICODE
    PFN_NTQUERYINFORMATIONPROCESS pfnNtQIP = (PFN_NTQUERYINFORMATIONPROCESS)GetProcAddress(hNtDll, "NtQueryInformationProcess");
    if (pfnNtQIP != NULL) {
        lStatus = pfnNtQIP(hProcess, pic, pPI, cbSize, pLength);
    }   
   
    FreeLibrary(hNtDll);
    return(lStatus);
}


BOOL ProcessInfo::GetProcessCmdLine(HANDLE hProcess, LPTSTR szCmdLine, DWORD Size) 
{
    // Sanity checks
    if ((hProcess == NULL) || (szCmdLine == NULL) || (Size == 0)){
        return FALSE;
    }

    // 0. Get the Process Environment Block address
    int   iReturn = 1;
    DWORD dwSize;
    SIZE_T size;

    PROCESS_BASIC_INFORMATION  pbi;
    // The PEB was supposed to always be at address 0x7ffdf000 in XP...
    // ... but, here is the "right" way to get it now in Vista.
    iReturn = ProcessInfo::_NtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(pbi), &dwSize);

    // NtQueryInformationProcess returns a negative value if it fails
    if (iReturn < 0){
		return FALSE;
	}

    // 1. Find the Process Environment Block
    __PEB PEB;
	size = dwSize;
    if (!ReadProcessMemory(hProcess, pbi.PebBaseAddress, &PEB, sizeof(PEB), &size)) 
	{
        DWORD dwError = GetLastError();
		_tprintf(_T("ReadProcessMemory (1): dwError = %i\n"), dwError);
		Debug::printErrorMessage(dwError);
        return FALSE;
    }

    // 2. From this PEB, get the address of the block containing 
    // a pointer to the CmdLine
    __INFOBLOCK Block;
    if (!ReadProcessMemory(hProcess, (LPVOID)PEB.InfoBlockAddress, &Block, sizeof(Block), &size)) 
	{
        DWORD dwError = GetLastError();
		_tprintf(_T("ReadProcessMemory (2): dwError = %i\n"), dwError);
		Debug::printErrorMessage(dwError);
        return FALSE;
    }

    // 3. Get the CmdLine
    wchar_t wszCmdLine[MAX_PATH + 1];
    if (!ReadProcessMemory(hProcess, (LPVOID)Block.wszCmdLineAddress, wszCmdLine, MAX_PATH*sizeof(wchar_t), &size)) 
	{
        DWORD dwError = GetLastError();
		_tprintf(_T("ReadProcessMemory (2): dwError = %i\n"), dwError);
		Debug::printErrorMessage(dwError);
        return FALSE;
    }

	// _tprintf(_T("wszCmdLine = %s\n"), wszCmdLine);

    // 4. Skip the application pathname
    //    it can be empty, "c:\...\app.exe" or c:\...\app.exe
    wchar_t* pPos = wszCmdLine;
    if (*pPos != L'\0') 
	{
        if (*pPos == L'"') {
            // Find the next " character
            pPos = wcschr(&pPos[1], L'"');
        } else {
            // Find the next SPACE character
            pPos = wcschr(&pPos[1], L' ');
        }

        // Skip it
        if (pPos != NULL){
            pPos++;
		}
    }

    // Copy it back
    if (pPos != NULL) 
	{
        if (*pPos != L'\0'){
#ifdef UNICODE
            // Both strings are in UNICODE.
            _tcscpy_s(szCmdLine, Size, pPos);   
#else
            // from UNICODE to ANSI
            MultiByteToWideChar(CP_ACP, 0, szCmdLine, Size, pPos, wcslen(pPos)); 
#endif
        }
        else {
            szCmdLine[0] = TEXT('\0');
	    }
    }
    else {
        szCmdLine[0] = TEXT('\0');
	}

    return TRUE;
}
