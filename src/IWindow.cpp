
#include "IWindow.h"
#include "ProcessInfo.h"


IWindow::IWindow(HWND hWnd)
{
	DWORD dwError = 0;
	this->hWnd = hWnd;

	if(hWnd == 0)
	{
		hWndRoot = 0;
		hWndParent = 0;

		wndText = new TCHAR[1]; 
		wndText[0] = _T('\0');

		wndInfo = WINDOWINFO();
		// _tprintf(_T("wndInfo.cbSize = %i\n"), wndInfo.cbSize);
		// _tprintf(_T("wndInfo.dwStyle = %i\n"), wndInfo.dwStyle);

		wndWidth = 0;
		wndHeight = 0;
		wndPos = POINT();
		wndID = 0;

		className = new TCHAR[1];
		className[0] = _T('\0');
		realClassName = new TCHAR[1];
		realClassName[0] = _T('\0');

		dwThreadID = 0;
		dwProcessID = 0;
		hWndDC = NULL;
	    hBufferDC = NULL;
	    hWndBitmap = NULL;
	    hWndBitmap = NULL;
		return;
	}

	hWndRoot   = GetAncestor(hWnd, GA_ROOT);
	hWndParent = GetAncestor(hWnd, GA_PARENT);
	
	// _tprintf(_T("hWndRoot = 0x%X\n"), hWndRoot);
	// _tprintf(_T("hWndParent = 0x%X\n"), hWndParent);

	int strLen = (int)SendMessage(hWnd, WM_GETTEXTLENGTH, 0, 0) + 1;
    // _tprintf(_T("dwBuffSize = %i\n"), dwBuffSize);

	wndText = new TCHAR[strLen]; 
	SendMessage(hWnd, WM_GETTEXT, strLen, (LPARAM)wndText);

	wndInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hWnd, &wndInfo);

	GetWindowRect(hWnd, &wndRect);
	wndWidth  = wndRect.right - wndRect.left;
	wndHeight = wndRect.bottom - wndRect.top;

    if(hWndRoot == NULL)
    {
	    wndPos.x  = wndRect.left;
	    wndPos.y  = wndRect.top;
    }
    else
    {
        /*
        RECT rootWndRect;
        GetWindowRect(hWndRoot, &rootWndRect);
        wndPos.x  = wndRect.left - rootWndRect.left;
        wndPos.y  = wndRect.top - rootWndRect.top;
        */

        WINDOWINFO rootWndInfo = {sizeof(WINDOWINFO)};
        GetWindowInfo(hWndRoot, &rootWndInfo);
        wndPos.x  = wndRect.left - rootWndInfo.rcClient.left;
        wndPos.y  = wndRect.top - rootWndInfo.rcClient.top;
    }

	// int  ctrlID = GetDlgCtrlID(hWnd);
	// LONG hInstance  = GetWindowLong(hWnd, GWL_HINSTANCE);
	wndID  = GetWindowLong(hWnd, GWL_ID);
	
	className = new TCHAR[MAX_WND_CLASSNAME + 1];
	if(!GetClassName(hWnd, className, MAX_WND_CLASSNAME + 1))
	{
		DWORD dwError = GetLastError();
		_tprintf(_T("GetClassName: dwError = %i \n"), dwError);

	    className[0] = _T('\0');
	}

	realClassName = new TCHAR[MAX_WND_CLASSNAME + 1];
	if(!RealGetWindowClass(hWnd, realClassName, MAX_WND_CLASSNAME + 1))
	{
		DWORD dwError = GetLastError();
		_tprintf(_T("RealGetWindowClass: dwError = %i \n"), dwError);

	    realClassName[0] = _T('\0');
	}

	// _tprintf(_T("realClassNameLen = %i\n"), realClassNameLen);
	// _tprintf(_T("RealGetWindowClass: realClassName = 0x%X\n"), realClassName);

	dwThreadID = GetWindowThreadProcessId(hWnd, &dwProcessID);

	// _tprintf(_T("dwProcessID = %i \n"), dwProcessID);
	// _tprintf(_T("dwThreadID = %i \n"), dwThreadID);

}


IWindow::~IWindow()
{
	// _tprintf(_T("Call ~IWindow()\n"));

	delete [] wndText;
	delete [] className;
	delete [] realClassName;

	if(hWndBitmap != NULL && hBufferDC != NULL){
	    DeleteObject(SelectObject(hBufferDC, hWndBitmap));
		DeleteDC(hBufferDC);
	}

	if(hWndDC != NULL){
		ReleaseDC(hWnd, hWndDC);
	}
}



void IWindow::selectWindow()
{
    hWndDC = GetWindowDC(hWnd);
	hBufferDC = CreateCompatibleDC(hWndDC);
	hWndBitmap = CreateCompatibleBitmap(hWndDC, wndWidth, wndHeight);
	hWndBitmap = (HBITMAP)SelectObject(hBufferDC, hWndBitmap);

	if(!BitBlt(hBufferDC, 0, 0, wndWidth, wndHeight, hWndDC, 0, 0, SRCCOPY)){
		DWORD dwError = GetLastError();
		_tprintf(_T("BitBlt: dwError = %i \n"), dwError);
	}

	if(hWndDC != NULL)
	{
		HPEN hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
		hPen = (HPEN)SelectObject(hWndDC, hPen);

		MoveToEx(hWndDC, 1, 1, NULL);
		LineTo(hWndDC, wndWidth - 1, 1);
		LineTo(hWndDC, wndWidth - 1, wndHeight - 1);
		LineTo(hWndDC, 1, wndHeight - 1);
		LineTo(hWndDC, 1, 1);

		DeleteObject(SelectObject(hWndDC, hPen));
	}
}

void IWindow::deselectWindow()
{
	if(hBufferDC != NULL && hWndDC != NULL)
	{
		if(!BitBlt(hWndDC, 0, 0, wndWidth, wndHeight, hBufferDC, 0, 0, SRCCOPY)){
			DWORD dwError = GetLastError();
			_tprintf(_T("BitBlt: dwError = %i \n"), dwError);
		}
	}
}

