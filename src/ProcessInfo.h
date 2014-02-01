/******************************************************************************
Module:  ProcessInfo.h
Notices: Copyright (c) 2014 Kostja Stern
******************************************************************************/

#pragma once

#pragma comment (lib,"shlwapi.lib")
#pragma comment (lib,"shell32.lib")

#include <windows.h>
#include <winternl.h>  // NtQueryInformationProcess
#include <psapi.h>     // GetModuleFileNameEx
#include <tchar.h>

#include "Debug.h"

class ProcessInfo
{
    public:
		ProcessInfo(DWORD pID);
		~ProcessInfo();

		DWORD  getPID() const { return PID; }
		const TCHAR* getFileName() const { return fileName; }
		const TCHAR* getCmdParams() const { return cmdParams; }

    private:
		DWORD   PID;
		TCHAR   *fileName;
		TCHAR   *cmdParams;

		typedef struct
		{
		   DWORD Filler[4];
		   DWORD InfoBlockAddress;
		} __PEB;

		typedef struct
		{
		   DWORD Filler[17];
		   DWORD wszCmdLineAddress;
		} __INFOBLOCK;

		// NtQueryInformationProcess is declared in winternl.h
		typedef NTSTATUS (CALLBACK *PFN_NTQUERYINFORMATIONPROCESS)(
		   HANDLE ProcessHandle, 
		   PROCESSINFOCLASS ProcessInformationClass,
		   PVOID ProcessInformation,
		   ULONG ProcessInformationLength,
		   PULONG ReturnLength OPTIONAL
		);

		NTSTATUS _NtQueryInformationProcess(HANDLE hProcess, PROCESSINFOCLASS pic, PVOID pPI, ULONG cbSize, PULONG pLength);
		BOOL GetProcessCmdLine(HANDLE hProcess, LPTSTR szCmdLine, DWORD Size);
};