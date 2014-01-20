
#ifndef HELPER_H_
#define HELPER_H_

#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#include "tstring.h"
#include "IWindow.h"

class Helper
{
    public:
		static void printErrorMessage(DWORD dwError);
		static void SetTextToEdit(HWND hWndOutput, HWND hWnd);

    private:
		static void addStyleInfo(DWORD style, TString *text);
		static void addStyleExInfo(DWORD style, TString *text);
};


#endif