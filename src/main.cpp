

#include "main.h"

#define WND_SUMM_INFO_ID  2001
#define WND_TREE_INFO_ID  2002
#define WND_TREE_REFRESH_BUTTON_ID  2003

/*
    http://msdn.microsoft.com/en-us/library/ms632626%28v=vs.85%29.aspx (WM_GETMINMAXINFO message)

*/

// static variables
BOOL isMouseCapture = FALSE;
IWindow *lastWnd = NULL;
TreeControl *treeWindows = NULL;
HBITMAP hBitmapWnd = NULL;
HBITMAP hBitmapWndSight = NULL;
HCURSOR hSight = NULL;

INT_PTR CALLBACK MainDialogProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK CtrlInfoDialogProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK ProcessInfoDialogProc(HWND, UINT, WPARAM, LPARAM);



int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, PTSTR pszCmdLine, int nCmdShow)
{
	// Enabling the debug privilege allows the application to see
    // information about service applications
    CToolhelp::EnablePrivilege(SE_DEBUG_NAME, TRUE);
   
    // To get access to SACL.
    CToolhelp::EnablePrivilege(SE_SECURITY_NAME, TRUE);     

	hBitmapWndSight = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
    hBitmapWnd      = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BITMAP2));
    hSight          = LoadCursor(hInstance, MAKEINTRESOURCE(IDI_CURSOR1));

	// init "Common Control Library"
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icc.dwICC  = ICC_WIN95_CLASSES | ICC_TAB_CLASSES;
	InitCommonControlsEx(&icc);


    AllocConsole();	
	FILE *hf;
	hf = _fdopen( _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT | _O_WTEXT), "w");
	// hf = fopen("E:\\debug.txt", "a+");
	// fopen_s(&hf, "E:\\debug.txt", "a+");

    *stdout = *stderr = *hf;    /* enable using _tprintf  */


	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, MainDialogProc);


//	_tprintf(_T("end prog ..."));
//	Sleep(2000);

	if(treeWindows != NULL){
	    delete treeWindows;
	}

	if(lastWnd != NULL){
		delete lastWnd;
	}

    if(hBitmapWnd != NULL){
		DeleteObject(hBitmapWnd);
	}

	if(hBitmapWndSight != NULL){
		DeleteObject(hBitmapWndSight);
	}

    if(hSight != NULL){
        DeleteObject(hSight);
    }

	FreeConsole();
	CToolhelp::EnablePrivilege(SE_SECURITY_NAME, FALSE);     
    CToolhelp::EnablePrivilege(SE_DEBUG_NAME, FALSE);

	return 0;
}


INT_PTR MainDialog_OnInitDialog(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	_tprintf(_T("WM_INITDIALOG\n"));

	RECT wndRect;
    GetClientRect(hWnd, &wndRect);
	_tprintf(_T("wndRect.bottom = %i , wndRect.left = %i , wndRect.right = %i , wndRect.top = %i \n"), wndRect.bottom, wndRect.left, wndRect.right, wndRect.top);

	// _tprintf(_T("hBitmap = 0x%X\n"), hBitmap);
	HWND hFinderTool = GetDlgItem(hWnd, IDC_STATIC3);
	SendMessage(hFinderTool, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmapWndSight);

	HWND hTabControl = GetDlgItem(hWnd, IDC_TAB1);

	RECT tabRect;
    GetClientRect(hTabControl, &tabRect);
	_tprintf(_T("tabRect.bottom = %i , tabRect.left = %i , tabRect.right = %i , tabRect.top = %i \n"), tabRect.bottom, tabRect.left, tabRect.right, tabRect.top);

	int nTabHeight = tabRect.bottom; // 319
	int nTabWidth  = tabRect.right;  // 407

	TCITEM ti;
	ti.mask = TCIF_TEXT; // I'm only having text on the tab
	ti.pszText = _T("Wnd Info");
	TabCtrl_InsertItem(hTabControl, 0, &ti);
    
	ti.pszText = _T("All Windows");
	TabCtrl_InsertItem(hTabControl, 1, &ti);
	TabCtrl_SetCurSel(hTabControl, 0);

	_tprintf(_T("hTabControl = 0x%X\n"), hTabControl);

	HMODULE hModule = GetModuleHandle(NULL);

	HWND hWndSummInfo = CreateWindowEx(WS_EX_CONTROLPARENT,
								       WC_EDIT,
									   _T(""),
									   WS_BORDER | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN | ES_READONLY,
									   5, 30,       // x, y upper-left corner of the window relative to the upper-left corner of the parent window's client area.
									   nTabWidth - 10, nTabHeight - 36,    // nWidth , nHeight 
									   hTabControl, // A handle to the parent or owner window of the window being created.
									   (HMENU)WND_SUMM_INFO_ID,  // For a child window, hMenu specifies the child-window identifier, an integer 
												                 // value used by a dialog box control to notify its parent about events. 
												                 // The application determines the child-window identifier; it must be unique for all child windows with the same parent window. 
									   hModule,
									   NULL
									  );
	// _tprintf(_T("hWndSummInfo = 0x%X\n"), hWndSummInfo);
	if(hWndSummInfo == 0)
	{
		DWORD dwError = GetLastError();
		_tprintf(_T("hWndSummInfo: dwError = %i\n"), dwError);
		HPrint::printErrorMessage(dwError);
	}

	HWND hWndTreeInfo = CreateWindowEx(0,
								       WC_TREEVIEW,
								       _T(""),
								       WS_BORDER | WS_CHILD | WS_HSCROLL | WS_VSCROLL | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES, /* WS_CHILD   */
								       5, 30,                           // x, y upper-left corner of the window relative to the upper-left corner of the parent window's client area.
								       nTabWidth - 10, nTabHeight - 65, // nWidth , nHeight 
								       hTabControl,              // A handle to the parent or owner window of the window being created.
								       (HMENU)WND_TREE_INFO_ID,  // For a child window, hMenu specifies the child-window identifier, an integer 
									                             // value used by a dialog box control to notify its parent about events. 
									                             // The application determines the child-window identifier; it must be unique for all child windows with the same parent window. 
							           hModule,
								       NULL
							          );

	_tprintf(_T("hWndTreeInfo = 0x%X\n"), hWndTreeInfo);
	if(hWndTreeInfo == 0)
	{
		DWORD dwError = GetLastError();
		_tprintf(_T("hWndTreeInfo: dwError = %i\n"), dwError);
		HPrint::printErrorMessage(dwError);
	}

	HWND hBtn = CreateWindowEx(0,
							   WC_BUTTON,
							   _T("Refresh"),
							   WS_BORDER | WS_CHILD | BS_PUSHBUTTON, 
							   nTabWidth - 75, nTabHeight - 30,    // x, y upper-left corner of the window relative to the upper-left corner of the parent window's client area.
							   70, 25,                             // nWidth , nHeight 
							   hTabControl,                        // A handle to the parent or owner window of the window being created.
							   (HMENU)WND_TREE_REFRESH_BUTTON_ID,  // For a child window, hMenu specifies the child-window identifier, an integer 
									                                // value used by a dialog box control to notify its parent about events. 
									                                // The application determines the child-window identifier; 
																	// it must be unique for all child windows with the same parent window. 
							   hModule,
							   NULL
							  );
	if(hBtn == 0)
	{
		DWORD dwError = GetLastError();
		_tprintf(_T("hBtn: dwError = %i\n"), dwError);
		HPrint::printErrorMessage(dwError);
	}

	treeWindows = new TreeControl(hWndTreeInfo);
	treeWindows->reBuildTree();


	// HWND hWndtitle = GetDlgItem(hWnd, IDC_EDIT1);
	// SetFocus(hWndtitle); 
    return FALSE; 
}


void MainDialog_OnMouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    DWORD dwError;
	POINT point;
	HWND  hSearchWnd;

	if(isMouseCapture)
	{
		if(!GetCursorPos(&point)){
			dwError = GetLastError();
			_tprintf(_T("GetCursorPos: dwError = %i\n"), dwError);
			HPrint::printErrorMessage(dwError);
		}

		hSearchWnd = WindowFromPoint(point);
				

		if(hSearchWnd != NULL  && (lastWnd == NULL || hSearchWnd != lastWnd->getHWND()))
		{
			_tprintf(_T("hSearchWnd = 0x%X\n"), hSearchWnd);

			if(lastWnd != NULL){
				_tprintf(_T("lastWnd->deselectWindow()\n"));
				lastWnd->deselectWindow();

				_tprintf(_T("delete lastWnd\n"));
				delete lastWnd; 
			}
					
			_tprintf(_T("new IWindow(hSearchWnd)\n"));
			lastWnd = new IWindow(hSearchWnd);
			_tprintf(_T("lastWnd->getHWND() = 0x%X\n"), lastWnd->getHWND());
			_tprintf(_T("SetTextToEdit\n"));

			// _tprintf(_T("hWndSummInfo = 0x%X\n"), hWndSummInfo);
			// _tprintf(_T("hWndSummInfo1 = 0x%X\n"), GetDlgItem(hWnd, WND_SUMM_INFO_ID));
			IWindow rootWnd(lastWnd->getRootHWND());
			HWND hRootTitle = GetDlgItem(hWnd, IDC_EDIT1);
			HWND hRootClass = GetDlgItem(hWnd, IDC_EDIT2);
			SendMessage(hRootTitle, WM_SETTEXT, 0, (LPARAM)rootWnd.getText());
			SendMessage(hRootClass, WM_SETTEXT, 0, (LPARAM)rootWnd.getClassName());

			HWND hCtrlClass = GetDlgItem(hWnd, IDC_EDIT3);
			HWND hCtrlInst  = GetDlgItem(hWnd, IDC_EDIT4);
			SendMessage(hCtrlClass, WM_SETTEXT, 0, (LPARAM)lastWnd->getClassName());

			TString ctrlID;
			ctrlID.append(_T("%i"), lastWnd->getID());
			SendMessage(hCtrlInst, WM_SETTEXT, 0, (LPARAM)ctrlID.getString());

			HWND hTabControl = GetDlgItem(hWnd, IDC_TAB1);
            HWND hWndSummInfo = GetDlgItem(hTabControl, WND_SUMM_INFO_ID);
			Helper::SetTextToEdit(hWndSummInfo, lastWnd);
			lastWnd->selectWindow();
		}
	}
}

void MainDialog_OnNotify(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LPNMHDR lpNmHdr = (LPNMHDR)lParam;

	// _tprintf(_T("WM_NOTIFY:  hWnd = %X , uMsg = %X , wParam = %X , lParam = %X\n"), hWnd, uMsg, wParam, lParam);

	switch(LOWORD(wParam))
	{
		case WND_TREE_INFO_ID:
			switch(lpNmHdr->code)
			{
				case NM_CLICK:
					// _tprintf(_T("NM_CLICK\n"));
				break;
				case NM_DBLCLK:
				{
					// _tprintf(_T("NM_DBLCLK\n"));

					if(treeWindows != NULL)
					{
						TreeItem treeItem = treeWindows->getSelectedItem();

						if(treeItem.type == PROCESS){
							DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_PROCESS_INFO), hWnd, ProcessInfoDialogProc, (LPARAM)&treeItem);
						}
						else {
							DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CONTROL_INFO), hWnd, CtrlInfoDialogProc, (LPARAM)&treeItem);
						}
					}
				}
				break;
				case TVN_SELCHANGED:
					// _tprintf(_T("TVN_SELCHANGED\n"));
				break;
			}
		break;

		case IDC_TAB1: // tab control

			switch(lpNmHdr->code)
			{
				case TCN_SELCHANGING: // changed the tab selection (clicked on another tab)
				{
					HWND hTabControl = GetDlgItem(hWnd, IDC_TAB1);
                    HWND hWndSummInfo = GetDlgItem(hTabControl, WND_SUMM_INFO_ID);
					HWND hWndTreeInfo = GetDlgItem(hTabControl, WND_TREE_INFO_ID);
					HWND hWndTreeRefreshButton = GetDlgItem(hTabControl, WND_TREE_REFRESH_BUTTON_ID);
					
					int tabId = TabCtrl_GetCurSel(lpNmHdr->hwndFrom);
					
					_tprintf(_T("tabId = %i\n"), tabId);
					if(tabId == 0)
					{
						ShowWindow(hWndSummInfo, SW_HIDE);
						ShowWindow(hWndTreeInfo, SW_SHOW);
						ShowWindow(hWndTreeRefreshButton, SW_SHOW);
					} 
					else 
					{
						ShowWindow(hWndTreeInfo, SW_HIDE);
						ShowWindow(hWndTreeRefreshButton, SW_HIDE);
						ShowWindow(hWndSummInfo, SW_SHOW);
					}
				}
			}
		break;
	}
}


void MainDialog_OnCommand(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WORD whwParam, wlwParam;
	whwParam = HIWORD(wParam);
	wlwParam = LOWORD(wParam);

	_tprintf(_T("WM_COMMAND:  hWnd = %X , uMsg = %X , whwParam = %X , wlwParam = %X , lParam = %X\n"), hWnd, uMsg, whwParam, wlwParam, lParam);

	switch(wlwParam)
	{
		case IDCANCEL:
			EndDialog(hWnd, uMsg);
		break;

		case WND_TREE_REFRESH_BUTTON_ID:
			_tprintf(_T("WND_TREE_REFRESH_BUTTON_ID click\n"));

		break;

        case IDC_STATIC3: // click in finder tool
        {
            SetCapture(hWnd);
		    isMouseCapture = TRUE;

            HWND hFinderTool = GetDlgItem(hWnd, IDC_STATIC3);
            SendMessage(hFinderTool, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmapWnd);
            hSight = SetCursor(hSight);
            _tprintf(_T("hSight = 0x%X\n"), hSight);
        }
        break;

		case IDM_EXIT1:
			// menu command
			if(whwParam == 0 && lParam == 0)
			{
				MessageBox(hWnd, _T("Exit menu click"), _T("Debug"), MB_OK);
			}
		break;

		case IDM_ABOUT____1:
			// menu command
			if(whwParam == 0 && lParam == 0)
			{
				MessageBox(hWnd, _T("About menu click"), _T("Debug"), MB_OK);
			}
		break;
	}

/*
	HANDLE hFinder  = GetDlgItem(hWnd, IDC_STATIC3);

	// click in finder tool
	if(hFinder == (HANDLE)lParam){
		SetCapture(hWnd);
		isMouseCapture = TRUE;
	}
*/
}


/*
   http://msdn.microsoft.com/en-us/library/ms997619.aspx (Design Specifications and Guidelines - Visual Design)

*/
void MainDialog_OnSize(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WORD newWindth = LOWORD((DWORD)lParam);
	WORD newHeight = HIWORD((DWORD)lParam);

/*
	_tprintf(_T("newWindth = %i , newHeight = %i\n"), newWindth, newHeight);

	int baseunitX = LOWORD(GetDialogBaseUnits()); // the horizontal dialog box base unit
	int baseunitY = HIWORD(GetDialogBaseUnits()); // the vertical dialog box base unit
	
	_tprintf(_T("baseunitX = %i \n"), baseunitX);
	_tprintf(_T("baseunitY = %i \n"), baseunitY);

	int pixelX = MulDiv(baseunitX, 280, 4);
    int pixelY = MulDiv(baseunitY, 300, 8);

	_tprintf(_T("pixelX = %i \n"), pixelX);
	_tprintf(_T("pixelY = %i \n"), pixelY);
*/

	HWND hTabControl = GetDlgItem(hWnd, IDC_TAB1);
	SetWindowPos(hTabControl, NULL, 5, 155, newWindth - 10, newHeight - 160, SWP_NOZORDER);

    HWND hWndSummInfo = GetDlgItem(hTabControl, WND_SUMM_INFO_ID);
	RECT wndSummInfoRect;
	GetClientRect(hWndSummInfo, &wndSummInfoRect);
	SetWindowPos(hWndSummInfo, NULL, 5, 27, newWindth - 20, newHeight - 195, SWP_NOZORDER);

	HWND hWndTreeInfo = GetDlgItem(hTabControl, WND_TREE_INFO_ID);
	SetWindowPos(hWndTreeInfo, NULL, 5, 27, newWindth - 20, newHeight - 221, SWP_NOZORDER);

	HWND hBtn = GetDlgItem(hTabControl, WND_TREE_REFRESH_BUTTON_ID);
	SetWindowPos(hBtn, NULL, newWindth - 85, newHeight - 190, 70, 25, SWP_NOZORDER);

}

void MainDialog_OnParentNotify(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(LOWORD(wParam) == WM_LBUTTONDOWN && treeWindows != NULL)
	{
		HWND hBtn = GetDlgItem( GetDlgItem(hWnd, IDC_TAB1), WND_TREE_REFRESH_BUTTON_ID);

		WINDOWINFO rootWnd;
		GetWindowInfo(hWnd, &rootWnd);

		WINDOWINFO btnWnd;
		GetWindowInfo(hBtn, &btnWnd);

		RECT btnRect;
		btnRect.bottom = btnWnd.rcClient.bottom - rootWnd.rcClient.top;
		btnRect.left   = btnWnd.rcClient.left - rootWnd.rcClient.left;
		btnRect.right  = btnWnd.rcClient.right - rootWnd.rcClient.left;
		btnRect.top    = btnWnd.rcClient.top - rootWnd.rcClient.top;

		int xCoord = LOWORD(lParam);
		int yCoord = HIWORD(lParam);

		POINT point = {xCoord, yCoord};
				
		if(PtInRect(&btnRect, point) && IsWindowVisible(hBtn))
		{
			treeWindows->reBuildTree();
		}
	}
}

INT_PTR CALLBACK MainDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DWORD dwError;

	// uMsg = 528 (0x210)
	// _tprintf(_T("MainDialogProc: hWnd = 0x%X , uMsg = %i , wParam = %i , lParam = %i \n"), hWnd, uMsg, wParam, lParam);
	
	switch(uMsg)
	{
	    case WM_INITDIALOG:
			return MainDialog_OnInitDialog(hWnd, uMsg, wParam, lParam);

		case WM_LBUTTONUP:
			if(isMouseCapture)
			{
				if(!ReleaseCapture()){
					dwError = GetLastError();
				    _tprintf(_T("ReleaseCapture: dwError = %i\n"), dwError);
				    HPrint::printErrorMessage(dwError);
				}
				isMouseCapture = FALSE;

                HWND hFinderTool = GetDlgItem(hWnd, IDC_STATIC3);
                SendMessage(hFinderTool, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)hBitmapWndSight);
                hSight = SetCursor(hSight);
                _tprintf(_T("hSight = 0x%X\n"), hSight);
			}
			return TRUE;
		break;

		case WM_SIZE:
			MainDialog_OnSize(hWnd, uMsg, wParam, lParam);
			return TRUE;
		break;

		case WM_GETMINMAXINFO:
			{
				MINMAXINFO *minMaxInfo = (MINMAXINFO*)lParam;

				// set the minimum tracking width (x member) and the minimum tracking height (y member) of the window. 
				minMaxInfo->ptMinTrackSize.x = 435;
				minMaxInfo->ptMinTrackSize.y = 400;
			}
		    return TRUE;
		break;

		// case WM_LBUTTONDOWN:
	    case WM_MOUSEMOVE:
		    MainDialog_OnMouseMove(hWnd, uMsg, wParam, lParam);
			return TRUE;
		break;

		case WM_NOTIFY:
			MainDialog_OnNotify(hWnd, uMsg, wParam, lParam);
			return TRUE;
		break;

		case WM_PARENTNOTIFY:
			MainDialog_OnParentNotify(hWnd, uMsg, wParam, lParam);
			return TRUE;
		break;

		case WM_COMMAND:
			// _tprintf(_T("WM_COMMAND: hWnd = 0x%X , uMsg = %i , wParam = %i , lParam = %i \n"), hWnd, uMsg, wParam, lParam);
			MainDialog_OnCommand(hWnd, uMsg, wParam, lParam);
			return TRUE;
		break;

// 		default:
//			_tprintf(_T("MainDialogProc: hWnd = 0x%X , uMsg = %i , wParam = %i , lParam = %i \n"), hWnd, uMsg, wParam, lParam);
	}

	return FALSE;
}


INT_PTR CALLBACK CtrlInfoDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hWndSel = NULL; 


	switch (uMsg) 
    { 
	    case WM_INITDIALOG:
	    {	
			// _tprintf(_T("WM_INITDIALOG\n"));

			hWndSel = (HWND)((TreeItem*)lParam)->vItem;
			// _tprintf(_T("hWndSel = 0x%X\n"), hWndSel);

			HWND hPopupEdit = GetDlgItem(hwndDlg, IDC_EDIT1);

			// IWindow *wndInfo = new IWindow((HWND)lParam);
			// SetTextToEdit(hPopupEdit, wndInfo);

			// ShowWindow(hPopupCtrlInfo, SW_SHOW);
			// delete wndInfo;

			Helper::SetTextToEdit(hPopupEdit, (TreeItem*)lParam);

            SetFocus(hPopupEdit); 
            return FALSE; 

		}
		break;

	    case WM_COMMAND:
			
			switch(LOWORD(wParam))
			{
			    case IDCANCEL:
					EndDialog(hwndDlg, 0);
					return TRUE;
				break;

				case IDC_BUTTON1:
				{
					// _tprintf(_T("IDC_BUTTON1: hwndDlg = 0x%X , uMsg = %i , wParam = %i , lParam = %i \n"), hwndDlg, uMsg, wParam, lParam);
					// _tprintf(_T("hWndSel = 0x%X\n"), hWndSel);
					if(hWndSel != NULL)
					{
						IWindow *wndInfo = new IWindow(hWndSel);
						DWORD currThreadID = GetCurrentThreadId();

						if(!AttachThreadInput(wndInfo->getThreadID(), currThreadID, TRUE))
						{
							DWORD dwError = GetLastError();
		                    _tprintf(_T("AttachThreadInput: dwError = %i \n"), dwError);
							HPrint::printErrorMessage(dwError);
						}
						else
						{
							HWND prevWnd = SetFocus(wndInfo->getHWND());
							_tprintf(_T("prevWnd = 0x%X\n"), prevWnd);
							wndInfo->selectWindow();
						}

						delete wndInfo;
					}
				}
				break;
			}

			// _tprintf(_T("CtrlInfoDialogProc: WM_COMMAND\n"));
			// EndDialog(hwndDlg, 0);
			return FALSE;
		break;

        // Place message cases here. 
        default: 
            return FALSE; 
    } 
}


INT_PTR CALLBACK ProcessInfoDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) 
    { 
	    case WM_INITDIALOG:
	    {	
			_tprintf(_T("WM_INITDIALOG\n"));
			_tprintf(_T("hwndDlg = 0x%X\n"), hwndDlg);
			_tprintf(_T("lParam = 0x%X\n"), lParam);
			_tprintf(_T("\n"));

			HWND hPopupEdit = GetDlgItem(hwndDlg, IDC_EDIT1);
			// IWindow *wndInfo = new IWindow((HWND)lParam);
			// SetTextToEdit(hPopupEdit, wndInfo);

			// ShowWindow(hPopupCtrlInfo, SW_SHOW);
			// delete wndInfo;

			Helper::SetTextToEdit(hPopupEdit, (TreeItem*)lParam);

            SetFocus(hPopupEdit); 
            return FALSE; 

		}
		break;

	    case WM_COMMAND:
			switch(LOWORD(wParam))
			{
			    case IDCANCEL:
					EndDialog(hwndDlg, 0);
					return TRUE;
				break;
			}

			// _tprintf(_T("CtrlInfoDialogProc: WM_COMMAND\n"));
			// EndDialog(hwndDlg, 0);
			return FALSE;
		break;

        // Place message cases here. 
        default: 
            return FALSE; 
    } 
}


