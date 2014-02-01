/******************************************************************************
Module:  Debug.h
Notices: Copyright (c) 2014 Kostja Stern
******************************************************************************/

#pragma once


#include <windows.h>
#include <tchar.h>
#include <stdio.h>


class Debug
{
    public:
		static void printErrorMessage(DWORD dwError);

};

