
/*
    http://stackoverflow.com/questions/3246663/how-to-get-the-window-class-name-and-id-from-hwnd (How to get the Window Class Name and Id from HWND?)
    http://msdn.microsoft.com/en-us/library/windows/desktop/ms633584%28v=vs.85%29.aspx           (GetWindowLong function)
*/

#ifndef IWINDOW_H_
#define IWINDOW_H_

#include <windows.h>
#include <psapi.h>

#include "tstring.h"

// http://msdn.microsoft.com/en-us/library/windows/desktop/ms633577%28v=vs.85%29.aspx (WNDCLASSEX structure)
// The maximum length for lpszClassName is 256. 
// If lpszClassName is greater than the maximum length, the RegisterClassEx function will fail.
#define  MAX_WND_CLASSNAME 256


class IWindow
{
    private:
		HWND    hWnd;
		RECT    wndRect;
		int     wndWidth;
		int     wndHeight;
		HBITMAP hWndBitmap;
		HDC     hWndDC;
		HDC     hBufferDC;

		TCHAR   *className;
		TCHAR   *realClassName;

		HWND    hWndRoot;
		RECT    wndRootRect;
		int     wndRootWidth;
		int     wndRootHeight;
		TCHAR   *rootClassName;
		TCHAR   *rootRealClassName;

		DWORD   dwProcessID;
	    DWORD   dwThreadID;
		

		TCHAR *wndText;
		WINDOWINFO wndInfo;

		TCHAR *rootWndTitle;
		WINDOWINFO rootWndInfo;

		TCHAR *fileName;

		int wndCtrlID;
		tString *wndInfoText;
		void setWindowStyle(DWORD style);
		void setWindowExStyle(DWORD style);

    public:
		IWindow();
	    IWindow(HWND hWnd);
		~IWindow();
		void selectWindow();
		void deselectWindow();
		const TCHAR* getWindowInfo();
		HWND  getWindowHWND();
};

#endif