
#include "Helper.h"


void Helper::SetTextToEdit(HWND hWndOutput, IWindow* wnd)
{
	if(wnd == NULL) return;

	TString *wndSummText = new TString();

	if(wnd->getRootHWND() != NULL)
	{
		wndSummText->append(_T(">>>> Root Window <<<<\r\n"));

		IWindow *rootWnd = new IWindow(wnd->getRootHWND());
		wndSummText->append(_T("Title: %s\r\n"), rootWnd->getText());
		wndSummText->append(_T("Class: %s\r\n"), rootWnd->getClassName());
		wndSummText->append(_T("Position: (left = %i, top = %i)\r\n"), rootWnd->getWndPos().x, rootWnd->getWndPos().y);
		wndSummText->append(_T("Size: (width = %i, height = %i)\r\n"), rootWnd->getWidth(), rootWnd->getHeight());
		wndSummText->append(_T("Style: 0x%X\r\n"), rootWnd->getStyle());
		Helper::addStyleInfo(rootWnd->getStyle(), wndSummText);
		wndSummText->append(_T("ExStyle: 0x%X\r\n"), rootWnd->getExStyle());
		Helper::addStyleExInfo(rootWnd->getExStyle(), wndSummText);
		wndSummText->append(_T("Handle: 0x%X\r\n"), rootWnd->getHWND());
		wndSummText->append(_T("\r\n"));

		delete rootWnd;
	}
	
	if(wnd->getParentHWND() != NULL && wnd->getParentHWND() != wnd->getRootHWND())
	{
		IWindow *parentWnd = new IWindow(wnd->getParentHWND());
		wndSummText->append(_T(">>>> Parent Window <<<<\r\n"));
		wndSummText->append(_T("Text: %s\r\n"), parentWnd->getText());
		wndSummText->append(_T("Class: %s\r\n"), parentWnd->getClassName());
		wndSummText->append(_T("Position: (left = %i, top = %i)\r\n"), parentWnd->getWndPos().x, parentWnd->getWndPos().y);
		wndSummText->append(_T("Size: (width = %i, height = %i)\r\n"), parentWnd->getWidth(), parentWnd->getHeight());
		wndSummText->append(_T("Style: 0x%X\r\n"), parentWnd->getStyle());
		Helper::addStyleInfo(parentWnd->getStyle(), wndSummText);
		wndSummText->append(_T("ExStyle: 0x%X\r\n"), parentWnd->getExStyle());
		Helper::addStyleExInfo(parentWnd->getExStyle(), wndSummText);
		wndSummText->append(_T("Handle: 0x%X\r\n"), parentWnd->getHWND());
		wndSummText->append(_T("\r\n"));

		delete parentWnd;
	}

	wndSummText->append(_T(">>>> Control <<<<\r\n"));
	wndSummText->append(_T("Text: %s\r\n"), wnd->getText());
	wndSummText->append(_T("Class: %s\r\n"), wnd->getClassName());
	wndSummText->append(_T("ID: %ld\r\n"), wnd->getID());
	wndSummText->append(_T("Position: (left = %i, top = %i)\r\n"), wnd->getWndPos().x, wnd->getWndPos().y);
	wndSummText->append(_T("Size: (width = %i, height = %i)\r\n"), wnd->getWidth(), wnd->getHeight());
	wndSummText->append(_T("Style: 0x%X\r\n"), wnd->getStyle());
	Helper::addStyleInfo(wnd->getStyle(), wndSummText);
	wndSummText->append(_T("ExStyle: 0x%X\r\n"), wnd->getExStyle());
	Helper::addStyleExInfo(wnd->getExStyle(), wndSummText);
	wndSummText->append(_T("Handle: 0x%X\r\n"), wnd->getHWND());
	wndSummText->append(_T("\r\n"));
	wndSummText->append(_T("\r\n"));
	wndSummText->append(_T("ThreadID: %i\r\n"), wnd->getThreadID());
	wndSummText->append(_T("ProcessID: %i\r\n"), wnd->getProcessID());

	SendMessage(hWndOutput, WM_SETTEXT, 0, (LPARAM)wndSummText->getString());
	// SendMessage(hWndOutput, WM_PAINT, 0, 0);
	delete wndSummText;
}

void Helper::SetTextToEdit(HWND hWndOutput, TreeItem* treeTtem)
{
	if(treeTtem == NULL) return;

	switch(treeTtem->type)
	{
	    case PROCESS:
		{
			TString *wndSummText = new TString();
			ProcessInfo *processInfo = new ProcessInfo((DWORD)treeTtem->vItem);
			wndSummText->append(_T("PID: %i\r\n"), processInfo->getPID());
			wndSummText->append(_T("Cmd: %s %s\r\n"), processInfo->getFileName(), processInfo->getCmdParams());

			delete processInfo;

			SendMessage(hWndOutput, WM_SETTEXT, 0, (LPARAM)wndSummText->getString());
	        SendMessage(hWndOutput, WM_PAINT, 0, 0);
			delete wndSummText;
		}
		break;

		case WINDOW:
		case ROOTWINDOW:
		{
			IWindow *wnd = new IWindow((HWND)treeTtem->vItem);
		    SetTextToEdit(hWndOutput, wnd);
			delete wnd;
		}
	}
}


/**
 *   http://msdn.microsoft.com/en-us/library/windows/desktop/ff700543%28v=vs.85%29.aspx (Extended Window Styles)  
 */
void Helper::addStyleExInfo(DWORD style, TString *text)
{
	if(text == NULL){
		return;
	}

	if(style & WS_EX_ACCEPTFILES){
		text->append(_T("    WS_EX_ACCEPTFILES\r\n"));
	}

	if(style & WS_EX_APPWINDOW){
		text->append(_T("    WS_EX_APPWINDOW\r\n"));
	}

	if(style & WS_EX_CLIENTEDGE){
		text->append(_T("    WS_EX_CLIENTEDGE\r\n"));
	}

	if(style & WS_EX_COMPOSITED){
		text->append(_T("    WS_EX_COMPOSITED\r\n"));
	}

	if(style & WS_EX_CONTEXTHELP){
		text->append(_T("    WS_EX_CONTEXTHELP\r\n"));
	}

	if(style & WS_EX_CONTROLPARENT){
		text->append(_T("    WS_EX_CONTROLPARENT\r\n"));
	}

	if(style & WS_EX_DLGMODALFRAME){
		text->append(_T("    WS_EX_DLGMODALFRAME\r\n"));
	}

	if(style & WS_EX_LAYERED){
		text->append(_T("    WS_EX_LAYERED\r\n"));
	}

	if(style & WS_EX_LAYOUTRTL){
		text->append(_T("    WS_EX_LAYOUTRTL\r\n"));
	}

	if(style & WS_EX_LEFT){
		text->append(_T("    WS_EX_LEFT\r\n"));
	}

	if(style & WS_EX_LEFTSCROLLBAR){
		text->append(_T("    WS_EX_LEFTSCROLLBAR\r\n"));
	}

	if(style & WS_EX_LTRREADING){
		text->append(_T("    WS_EX_LTRREADING\r\n"));
	}

	if(style & WS_EX_MDICHILD){
		text->append(_T("    WS_EX_MDICHILD\r\n"));
	}

	if(style & WS_EX_NOACTIVATE){
		text->append(_T("    WS_EX_NOACTIVATE\r\n"));
	}

	if(style & WS_EX_NOINHERITLAYOUT){
		text->append(_T("    WS_EX_NOINHERITLAYOUT\r\n"));
	}

	if(style & 0x00200000L){ // WS_EX_NOREDIRECTIONBITMAP => 0x00200000L
		text->append(_T("    WS_EX_NOREDIRECTIONBITMAP\r\n"));
	} 

	if(style & WS_EX_OVERLAPPEDWINDOW){
		text->append(_T("    WS_EX_OVERLAPPEDWINDOW\r\n"));
	}

	if(style & WS_EX_PALETTEWINDOW){
		text->append(_T("    WS_EX_PALETTEWINDOW\r\n"));
	}

	if(style & WS_EX_RIGHT){
		text->append(_T("    WS_EX_RIGHT\r\n"));
	}

	if(style & WS_EX_RIGHTSCROLLBAR){
		text->append(_T("    WS_EX_RIGHTSCROLLBAR\r\n"));
	}

	if(style & WS_EX_RTLREADING){
		text->append(_T("    WS_EX_RTLREADING\r\n"));
	}

	if(style & WS_EX_STATICEDGE){
		text->append(_T("    WS_EX_STATICEDGE\r\n"));
	}

	if(style & WS_EX_TOOLWINDOW){
		text->append(_T("    WS_EX_TOOLWINDOW\r\n"));
	}

	if(style & WS_EX_TOPMOST){
		text->append(_T("    WS_EX_TOPMOST\r\n"));
	}

	if(style & WS_EX_TRANSPARENT){
		text->append(_T("    WS_EX_TRANSPARENT\r\n"));
	}

	if(style & WS_EX_WINDOWEDGE){
		text->append(_T("    WS_EX_WINDOWEDGE\r\n"));
	}
}


/**
 *   http://msdn.microsoft.com/en-us/library/windows/desktop/ms632600%28v=vs.85%29.aspx (Window Styles)
 */
void Helper::addStyleInfo(DWORD style, TString *text)
{
	if(style & WS_BORDER){
		text->append(_T("    WS_BORDER\r\n"));
	}

	if(style & WS_CAPTION){
		text->append(_T("    WS_CAPTION\r\n"));
	}

	if(style & WS_CHILD){
		text->append(_T("    WS_CHILD\r\n"));
	}

	if(style & WS_CHILDWINDOW){
		text->append(_T("    WS_CHILDWINDOW\r\n"));
	}

	if(style & WS_CLIPCHILDREN){
		text->append(_T("    WS_CLIPCHILDREN\r\n"));
	}

	if(style & WS_CLIPSIBLINGS){
		text->append(_T("    WS_CLIPSIBLINGS\r\n"));
	}

	if(style & WS_DISABLED){
		text->append(_T("    WS_DISABLED\r\n"));
	}

	if(style & WS_DLGFRAME){
		text->append(_T("    WS_DLGFRAME\r\n"));
	}

	if(style & WS_GROUP){
		text->append(_T("    WS_GROUP\r\n"));
	}

	if(style & WS_HSCROLL){
		text->append(_T("    WS_HSCROLL\r\n"));
	}

	if(style & WS_ICONIC){
		text->append(_T("    WS_ICONIC\r\n"));
	}

	if(style & WS_MAXIMIZE){
		text->append(_T("    WS_MAXIMIZE\r\n"));
	}

	if(style & WS_MAXIMIZEBOX){
		text->append(_T("    WS_MAXIMIZEBOX\r\n"));
	}

	if(style & WS_MINIMIZE){
		text->append(_T("    WS_MINIMIZE\r\n"));
	}

	if(style & WS_MINIMIZEBOX){
		text->append(_T("    WS_MINIMIZEBOX\r\n"));
	}

	if(style & WS_OVERLAPPED){
		text->append(_T("    WS_OVERLAPPED\r\n"));
	}

	if(style & WS_OVERLAPPEDWINDOW){
		text->append(_T("    WS_OVERLAPPEDWINDOW\r\n"));
	}

	if(style & WS_POPUP){
		text->append(_T("    WS_POPUP\r\n"));
	}

	if(style & WS_POPUPWINDOW){
		text->append(_T("    WS_POPUPWINDOW\r\n"));
	}

	if(style & WS_SIZEBOX){
		text->append(_T("    WS_SIZEBOX\r\n"));
	}

	if(style & WS_SYSMENU){
		text->append(_T("    WS_SYSMENU\r\n"));
	}

	if(style & WS_TABSTOP){
		text->append(_T("    WS_TABSTOP\r\n"));
	}

	if(style & WS_THICKFRAME){
		text->append(_T("    WS_THICKFRAME\r\n"));
	}

	if(style & WS_TILED){
		text->append(_T("    WS_TILED\r\n"));
	}

	if(style & WS_TILEDWINDOW){
		text->append(_T("    WS_TILEDWINDOW\r\n"));
	}

	if(style & WS_VISIBLE){
		text->append(_T("    WS_VISIBLE\r\n"));
	}

	if(style & WS_VSCROLL){
		text->append(_T("    WS_VSCROLL\r\n"));
	}
}