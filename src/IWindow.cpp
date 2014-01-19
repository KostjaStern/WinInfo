
#include "IWindow.h"



IWindow::IWindow(HWND hWnd)
{
	this->hWnd = hWnd;

	
	hWndRoot   = GetAncestor(hWnd, GA_ROOT);
	hWndParent = GetAncestor(hWnd, GA_PARENT);
	
	// _tprintf(_T("hWndRoot = 0x%X\n"), hWndRoot);
	// _tprintf(_T("hWndParent = 0x%X\n"), hWndParent);

	int strLen = 0;

	strLen = (int)SendMessage(hWnd, WM_GETTEXTLENGTH, 0, 0) + 1;
    // _tprintf(_T("dwBuffSize = %i\n"), dwBuffSize);

	wndText = new TCHAR[strLen]; 
	SendMessage(hWnd, WM_GETTEXT, strLen, (LPARAM)wndText);

	wndInfo.cbSize = sizeof(WINDOWINFO);
	GetWindowInfo(hWnd, &wndInfo);

	GetWindowRect(hWnd, &wndRect);
	wndWidth  = wndRect.right - wndRect.left;
	wndHeight = wndRect.bottom - wndRect.top;
	wndPos.x  = wndRect.left;
	wndPos.y  = wndRect.top;

	// int  ctrlID = GetDlgCtrlID(hWnd);
	// LONG hInstance  = GetWindowLong(hWnd, GWL_HINSTANCE);
	wndID  = GetWindowLong(hWnd, GWL_ID);
	
	className = new TCHAR[MAX_PATH];
	strLen = GetClassName(hWnd, className, MAX_PATH);
	if(!strLen){
	    className[0] = _T('\0');
	}

	realClassName = new TCHAR[MAX_PATH];
	strLen = (int)RealGetWindowClass(hWnd, realClassName, MAX_PATH);
	if(!strLen){
	    realClassName[0] = _T('\0');
	}

	// _tprintf(_T("realClassNameLen = %i\n"), realClassNameLen);
	// _tprintf(_T("RealGetWindowClass: realClassName = 0x%X\n"), realClassName);


	dwThreadID = GetWindowThreadProcessId(hWnd, &dwProcessID);
	// _tprintf(_T("dwProcessID = %i \n"), dwProcessID);
	// _tprintf(_T("dwThreadID = %i \n"), dwThreadID);

//	wndInfoText->append(_T("\r\n"));
//	wndInfoText->append(_T("ProcessID: %i\r\n"), dwProcessID);
//	wndInfoText->append(_T("ThreadID:  %i\r\n"), dwThreadID);


	HANDLE hProcess = OpenProcess(READ_CONTROL | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, // http://msdn.microsoft.com/en-us/library/windows/desktop/ms684880%28v=vs.85%29.aspx (Process Security and Access Rights)
                                  FALSE,
								  dwProcessID);

	fileName = new TCHAR[MAX_PATH];
	strLen = (int)GetModuleFileNameEx((HMODULE)hProcess, NULL, fileName, MAX_PATH);
	if(!strLen){
		fileName[0] = _T('\0');
	}

	// _tprintf(_T("fileNameLen = %i \n"), fileNameLen);
	// _tprintf(_T("fileName = %s \n"), fileName);

	hWndDC = GetWindowDC(hWnd);
	hBufferDC = CreateCompatibleDC(hWndDC);
	hWndBitmap = CreateCompatibleBitmap(hWndDC, wndWidth, wndHeight);
	hWndBitmap = (HBITMAP)SelectObject(hBufferDC, hWndBitmap);

	if(!BitBlt(hBufferDC, 0, 0, wndWidth, wndHeight, hWndDC, 0, 0, SRCCOPY)){
		DWORD dwError = GetLastError();
		_tprintf(_T("dwError = %i \n"), dwError);
	}

}


IWindow::~IWindow()
{
	// _tprintf(_T("Call ~IWindow()\n"));

	delete [] wndText;
	delete [] className;
	delete [] realClassName;
	delete [] fileName;


	if(hWndBitmap != NULL && hBufferDC != NULL){
	    DeleteObject(SelectObject(hBufferDC, hWndBitmap));
		DeleteDC(hBufferDC);
	}

	if(hWndDC != NULL){
		ReleaseDC(hWnd, hWndDC);
	}
}

/**
 *   http://msdn.microsoft.com/en-us/library/windows/desktop/ms632600%28v=vs.85%29.aspx (Window Styles)
 */
/*
void IWindow::setWindowStyle(DWORD style)
{
	if(style & WS_BORDER){
		wndInfoText->append(_T("    WS_BORDER\r\n"));
	}

	if(style & WS_CAPTION){
		wndInfoText->append(_T("    WS_CAPTION\r\n"));
	}

	if(style & WS_CHILD){
		wndInfoText->append(_T("    WS_CHILD\r\n"));
	}

	if(style & WS_CHILDWINDOW){
		wndInfoText->append(_T("    WS_CHILDWINDOW\r\n"));
	}

	if(style & WS_CLIPCHILDREN){
		wndInfoText->append(_T("    WS_CLIPCHILDREN\r\n"));
	}

	if(style & WS_CLIPSIBLINGS){
		wndInfoText->append(_T("    WS_CLIPSIBLINGS\r\n"));
	}

	if(style & WS_DISABLED){
		wndInfoText->append(_T("    WS_DISABLED\r\n"));
	}

	if(style & WS_DLGFRAME){
		wndInfoText->append(_T("    WS_DLGFRAME\r\n"));
	}

	if(style & WS_GROUP){
		wndInfoText->append(_T("    WS_GROUP\r\n"));
	}

	if(style & WS_HSCROLL){
		wndInfoText->append(_T("    WS_HSCROLL\r\n"));
	}

	if(style & WS_ICONIC){
		wndInfoText->append(_T("    WS_ICONIC\r\n"));
	}

	if(style & WS_MAXIMIZE){
		wndInfoText->append(_T("    WS_MAXIMIZE\r\n"));
	}

	if(style & WS_MAXIMIZEBOX){
		wndInfoText->append(_T("    WS_MAXIMIZEBOX\r\n"));
	}

	if(style & WS_MINIMIZE){
		wndInfoText->append(_T("    WS_MINIMIZE\r\n"));
	}

	if(style & WS_MINIMIZEBOX){
		wndInfoText->append(_T("    WS_MINIMIZEBOX\r\n"));
	}

	if(style & WS_OVERLAPPED){
		wndInfoText->append(_T("    WS_OVERLAPPED\r\n"));
	}

	if(style & WS_OVERLAPPEDWINDOW){
		wndInfoText->append(_T("    WS_OVERLAPPEDWINDOW\r\n"));
	}

	if(style & WS_POPUP){
		wndInfoText->append(_T("    WS_POPUP\r\n"));
	}

	if(style & WS_POPUPWINDOW){
		wndInfoText->append(_T("    WS_POPUPWINDOW\r\n"));
	}

	if(style & WS_SIZEBOX){
		wndInfoText->append(_T("    WS_SIZEBOX\r\n"));
	}

	if(style & WS_SYSMENU){
		wndInfoText->append(_T("    WS_SYSMENU\r\n"));
	}

	if(style & WS_TABSTOP){
		wndInfoText->append(_T("    WS_TABSTOP\r\n"));
	}

	if(style & WS_THICKFRAME){
		wndInfoText->append(_T("    WS_THICKFRAME\r\n"));
	}

	if(style & WS_TILED){
		wndInfoText->append(_T("    WS_TILED\r\n"));
	}

	if(style & WS_TILEDWINDOW){
		wndInfoText->append(_T("    WS_TILEDWINDOW\r\n"));
	}

	if(style & WS_VISIBLE){
		wndInfoText->append(_T("    WS_VISIBLE\r\n"));
	}

	if(style & WS_VSCROLL){
		wndInfoText->append(_T("    WS_VSCROLL\r\n"));
	}
}
*/

/**
 *   http://msdn.microsoft.com/en-us/library/windows/desktop/ff700543%28v=vs.85%29.aspx (Extended Window Styles)  
 */
 /*
void IWindow::setWindowExStyle(DWORD style)
{
	if(style & WS_EX_ACCEPTFILES){
		wndInfoText->append(_T("    WS_EX_ACCEPTFILES\r\n"));
	}

	if(style & WS_EX_APPWINDOW){
		wndInfoText->append(_T("    WS_EX_APPWINDOW\r\n"));
	}

	if(style & WS_EX_CLIENTEDGE){
		wndInfoText->append(_T("    WS_EX_CLIENTEDGE\r\n"));
	}

	if(style & WS_EX_COMPOSITED){
		wndInfoText->append(_T("    WS_EX_COMPOSITED\r\n"));
	}

	if(style & WS_EX_CONTEXTHELP){
		wndInfoText->append(_T("    WS_EX_CONTEXTHELP\r\n"));
	}

	if(style & WS_EX_CONTROLPARENT){
		wndInfoText->append(_T("    WS_EX_CONTROLPARENT\r\n"));
	}

	if(style & WS_EX_DLGMODALFRAME){
		wndInfoText->append(_T("    WS_EX_DLGMODALFRAME\r\n"));
	}

	if(style & WS_EX_LAYERED){
		wndInfoText->append(_T("    WS_EX_LAYERED\r\n"));
	}

	if(style & WS_EX_LAYOUTRTL){
		wndInfoText->append(_T("    WS_EX_LAYOUTRTL\r\n"));
	}

	if(style & WS_EX_LEFT){
		wndInfoText->append(_T("    WS_EX_LEFT\r\n"));
	}

	if(style & WS_EX_LEFTSCROLLBAR){
		wndInfoText->append(_T("    WS_EX_LEFTSCROLLBAR\r\n"));
	}

	if(style & WS_EX_LTRREADING){
		wndInfoText->append(_T("    WS_EX_LTRREADING\r\n"));
	}

	if(style & WS_EX_MDICHILD){
		wndInfoText->append(_T("    WS_EX_MDICHILD\r\n"));
	}

	if(style & WS_EX_NOACTIVATE){
		wndInfoText->append(_T("    WS_EX_NOACTIVATE\r\n"));
	}

	if(style & WS_EX_NOINHERITLAYOUT){
		wndInfoText->append(_T("    WS_EX_NOINHERITLAYOUT\r\n"));
	}

	if(style & 0x00200000L){ // WS_EX_NOREDIRECTIONBITMAP => 0x00200000L
		wndInfoText->append(_T("    WS_EX_NOREDIRECTIONBITMAP\r\n"));
	} 

	if(style & WS_EX_OVERLAPPEDWINDOW){
		wndInfoText->append(_T("    WS_EX_OVERLAPPEDWINDOW\r\n"));
	}

	if(style & WS_EX_PALETTEWINDOW){
		wndInfoText->append(_T("    WS_EX_PALETTEWINDOW\r\n"));
	}

	if(style & WS_EX_RIGHT){
		wndInfoText->append(_T("    WS_EX_RIGHT\r\n"));
	}

	if(style & WS_EX_RIGHTSCROLLBAR){
		wndInfoText->append(_T("    WS_EX_RIGHTSCROLLBAR\r\n"));
	}

	if(style & WS_EX_RTLREADING){
		wndInfoText->append(_T("    WS_EX_RTLREADING\r\n"));
	}

	if(style & WS_EX_STATICEDGE){
		wndInfoText->append(_T("    WS_EX_STATICEDGE\r\n"));
	}

	if(style & WS_EX_TOOLWINDOW){
		wndInfoText->append(_T("    WS_EX_TOOLWINDOW\r\n"));
	}

	if(style & WS_EX_TOPMOST){
		wndInfoText->append(_T("    WS_EX_TOPMOST\r\n"));
	}

	if(style & WS_EX_TRANSPARENT){
		wndInfoText->append(_T("    WS_EX_TRANSPARENT\r\n"));
	}

	if(style & WS_EX_WINDOWEDGE){
		wndInfoText->append(_T("    WS_EX_WINDOWEDGE\r\n"));
	}
}
*/


void IWindow::selectWindow()
{
	/*
	_tprintf(_T("selectWindow() \n"));
	_tprintf(_T("wndWidth = %i \n"),  wndWidth);
	_tprintf(_T("wndHeight = %i \n"), wndHeight);
	_tprintf(_T("hWnd = 0x%X \n"), hWnd);
	_tprintf(_T("hWndDC = 0x%X \n"), hWndDC);
	_tprintf(_T("hBufferDC = 0x%X \n"), hBufferDC);
	_tprintf(_T("rect.left = %i , rect.right = %i , rect.top = %i , rect.bottom = %i\n "), wndRect.left, wndRect.right, wndRect.top, wndRect.bottom);
	*/

	HPEN hPen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	hPen = (HPEN)SelectObject(hWndDC, hPen);

	MoveToEx(hWndDC, 1, 1, NULL);
	LineTo(hWndDC, wndWidth - 1, 1);
	LineTo(hWndDC, wndWidth - 1, wndHeight - 1);
	LineTo(hWndDC, 1, wndHeight - 1);
	LineTo(hWndDC, 1, 1);

	DeleteObject(SelectObject(hWndDC, hPen));
}

void IWindow::deselectWindow()
{
	_tprintf(_T("deselectWindow() \n"));

	if(hBufferDC != NULL && hWndDC != NULL)
	{
		if(!BitBlt(hWndDC, 0, 0, wndWidth, wndHeight, hBufferDC, 0, 0, SRCCOPY)){
			DWORD dwError = GetLastError();
			_tprintf(_T("dwError = %i \n"), dwError);
		}
	}
}

