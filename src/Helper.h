/******************************************************************************
Module:  Helper.h
Notices: Copyright (c) 2014 Kostja Stern
******************************************************************************/

#pragma once


#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include "tstring.h"
#include "ProcessInfo.h"
#include "IWindow.h"
#include "TreeControl.h"

class Helper
{
    public:
		static void SetTextToEdit(HWND hWndOutput, TreeItem* treeTtem);
		static void SetTextToEdit(HWND hWndOutput, IWindow* wnd);

    private:
		static void addStyleInfo(DWORD style, TString *text);
		static void addStyleExInfo(DWORD style, TString *text);
};


