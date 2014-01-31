
/*
    http://stackoverflow.com/questions/3246663/how-to-get-the-window-class-name-and-id-from-hwnd (How to get the Window Class Name and Id from HWND?)
    http://msdn.microsoft.com/en-us/library/windows/desktop/ms633584%28v=vs.85%29.aspx           (GetWindowLong function)
*/

#pragma once

#include <windows.h>
#include <psapi.h>

#include "Debug.h"
#include "tstring.h"


// http://msdn.microsoft.com/en-us/library/windows/desktop/ms633577%28v=vs.85%29.aspx (WNDCLASSEX structure)
// The maximum length for lpszClassName is 256. 
// If lpszClassName is greater than the maximum length, the RegisterClassEx function will fail.
#define  MAX_WND_CLASSNAME 256


class IWindow
{
	public:
	    IWindow(HWND hWnd);
		~IWindow();

		void selectWindow();
		void deselectWindow();

		HWND  getHWND() const { return hWnd; }
		HWND  getParentHWND() const { return hWndParent; }
		HWND  getRootHWND() const { return hWndRoot; }

		const TCHAR* getClassName() const { return className; }
		const TCHAR* getRealClassName() const { return realClassName; }
		const TCHAR* getText() const { return wndText; }
		
		int   getWidth() const { return wndWidth; }
		int   getHeight() const { return wndHeight; }
		LONG  getID() const { return wndID; }

		DWORD  getProcessID() const { return dwProcessID; }
		DWORD  getThreadID() const { return dwThreadID; }

		DWORD  getStyle() const { return wndInfo.dwStyle; }
		DWORD  getExStyle() const { return wndInfo.dwExStyle; }
		POINT  getWndPos() const { return wndPos; }

    private:
		HWND    hWnd;
		HWND    hWndParent;
		HWND    hWndRoot;

		LONG    wndID;     // the identifier of the window

		RECT    wndRect;
		POINT   wndPos;    /* left upper corner  */
		int     wndWidth;
		int     wndHeight;

		TCHAR   *className;
		TCHAR   *realClassName;

		TCHAR   *wndText;
		WINDOWINFO wndInfo;

		DWORD   dwProcessID;
	    DWORD   dwThreadID;

		/* uses for drawing and clearing the select rectangle */
		HBITMAP hWndBitmap;
		HDC     hWndDC;
		HDC     hBufferDC;
};

