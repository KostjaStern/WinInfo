

#include "main.h"

#define WND_SUMM_INFO_ID  2001
#define WND_TREE_INFO_ID  2002



// static variables
BOOL isMouseCapture = FALSE;
IWindow *lastWnd = NULL;
TreeControl *treeWindows = NULL;

HWND hTabControl = NULL;
HWND hWndSummInfo = NULL;
HWND hWndTreeInfo = NULL; 


ATOM RegClass(HINSTANCE hInstance, LPTSTR lpszClassName);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK CtrlInfoDialogProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK ProcessInfoDialogProc(HWND, UINT, WPARAM, LPARAM);
BOOL initGUI(HWND hWnd);

TCHAR* GetTextFromEdit(HWND hWnd);




int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, PTSTR pszCmdLine, int nCmdShow)
{
	HWND hMainWnd;
	MSG  msg;
	LPCTSTR   lpszWndClassName = _T("MainWndClass");
	// LPCTSTR   lpszAppName = _T("WinInfo");

	// Enabling the debug privilege allows the application to see
    // information about service applications
    CToolhelp::EnablePrivilege(SE_DEBUG_NAME, TRUE);
   
    // To get access to SACL.
    CToolhelp::EnablePrivilege(SE_SECURITY_NAME, TRUE);     


	if(!RegClass(hInstance, (LPTSTR)lpszWndClassName)){
		MessageBox(NULL, _T("Cannot register class"), _T("Error"), MB_OK);
		return 0;
	}

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


	hMainWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, NULL);

	if(hMainWnd == 0){
		DWORD dwError = GetLastError();
		_tprintf(_T("hMainWnd: dwError = %i\n"), dwError);
		HPrint::printErrorMessage(dwError);
		return 1;
	}

    // SetWindowText(hMainWnd, lpszAppName);
	
	initGUI(hMainWnd);


    ShowWindow(hMainWnd, nCmdShow);
    // UpdateWindow(hMainWnd);
	

	// LPCTSTR   lpszTestText = _T("Test text message ...\nHello message .....");
	// _tprintf(_T("hMainWnd = %X\n"), hMainWnd);


	// HANDLE hWndEdit = GetDlgItem(hMainWnd, IDC_EDIT5);
	// SendMessage((HWND)hWndEdit, WM_SETTEXT, 0, (LPARAM)lpszTestText);

    while(GetMessage(&msg, 0, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

//	_tprintf(_T("end prog ..."));
//	Sleep(2000);

	if(treeWindows != NULL){
	    delete treeWindows;
	}

	FreeConsole();
	CToolhelp::EnablePrivilege(SE_SECURITY_NAME, FALSE);     
    CToolhelp::EnablePrivilege(SE_DEBUG_NAME, FALSE);

    return (int)msg.wParam;

	// return 0;
}

ATOM RegClass(HINSTANCE hInstance, LPTSTR lpszClassName)
{
	WNDCLASSEX wc;

	wc.cbSize = sizeof(wc);
	wc.style  = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra  = 0;
	wc.cbWndExtra  = DLGWINDOWEXTRA;
	wc.hInstance   = hInstance;
	wc.hIcon       = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor     = LoadCursor(NULL, IDI_APPLICATION);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
	wc.lpszClassName = lpszClassName;
	wc.hIconSm       = LoadIcon(NULL, IDI_INFORMATION);

    return RegisterClassEx(&wc);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DWORD dwError;


	switch(uMsg)
	{
		case WM_CREATE:
            _tprintf(_T("WM_CREATE\n"));
		break;

		case WM_LBUTTONUP:
			if(isMouseCapture)
			{
				if(!ReleaseCapture()){
					dwError = GetLastError();
				    _tprintf(_T("ReleaseCapture: dwError = %i\n"), dwError);
				    HPrint::printErrorMessage(dwError);
				}
				isMouseCapture = FALSE;
			}

		break;

		// case WM_LBUTTONDOWN:
	    case WM_MOUSEMOVE:
		{
			// The coordinate is relative to the upper-left corner of the client area. 
			// http://msdn.microsoft.com/en-us/library/windows/desktop/ms645616%28v=vs.85%29.aspx (WM_MOUSEMOVE message)
			// xPos = GET_X_LPARAM(lParam); 
            // yPos = GET_Y_LPARAM(lParam); 
			// _tprintf(_T("isMouseCapture = %i\n"), isMouseCapture);
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


					Helper::SetTextToEdit(hWndSummInfo, lastWnd);
					lastWnd->selectWindow();
				}
			}

			/*
			buffer = GetTextFromEdit((HWND)hWndEdit);


			_tprintf(_T("buffer = %s\n"), buffer);

			if(!HeapFree(GetProcessHeap(), NULL, buffer)) {
				DWORD dwError = GetLastError();
				_tprintf(_T("dwError = %s\n"), dwError);
			}
			*/
		}
		break;

		case WM_NOTIFY:
		{
			LPNMHDR lpNmHdr = (LPNMHDR)lParam;
			/*
			_tprintf(_T("lpNmHdr->code = %i\n"), lpNmHdr->code);
			_tprintf(_T("lpNmHdr->hwndFrom = 0x%X\n"), lpNmHdr->hwndFrom);
			_tprintf(_T("lpNmHdr->idFrom = %i\n"), lpNmHdr->idFrom);
			*/

			// _tprintf(_T("hWndTreeInfo = 0x%X\n"), hWndTreeInfo);
			// _tprintf(_T("hWnd = 0x%X\n"), hWnd);

			switch(LOWORD(wParam))
			{
			    case WND_TREE_INFO_ID:
					switch(lpNmHdr->code)
					{
						case NM_CLICK:
							/*
							_tprintf(_T("NM_CLICK\n"));

							_tprintf(_T("hWndTreeInfo = 0x%X\n"), hWndTreeInfo);
							_tprintf(_T("lpNmHdr->hwndFrom = 0x%X\n"), lpNmHdr->hwndFrom);
							*/
						break;
						case NM_DBLCLK:
						{
							_tprintf(_T("NM_DBLCLK\n"));

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
							_tprintf(_T("TVN_SELCHANGED\n"));
						break;
					}
				break;

				case IDC_TAB1: // tab control

					// _tprintf(_T("hTabControl = 0x%X\n"), hTabControl);
					// _tprintf(_T("lpNmHdr->hwndFrom = 0x%X\n"), lpNmHdr->hwndFrom);

					switch(lpNmHdr->code)
					{
						case TCN_SELCHANGING: // changed the tab selection (clicked on another tab)
						{
							int tabId = TabCtrl_GetCurSel(lpNmHdr->hwndFrom);
					
							_tprintf(_T("tabId = %i\n"), tabId);
							if(tabId == 0){
								ShowWindow(hWndSummInfo, SW_HIDE);
								ShowWindow(hWndTreeInfo, SW_SHOW);
							} else {
								ShowWindow(hWndTreeInfo, SW_HIDE);
								ShowWindow(hWndSummInfo, SW_SHOW);
							}
						}
					}
				break;
			}

		}
		break;

		
		case WM_COMMAND:
		{
			WORD whwParam, wlwParam;
			whwParam = HIWORD(wParam);
			wlwParam = LOWORD(wParam);

			_tprintf(_T("WM_COMMAND:  hWnd = %X , uMsg = %X , whwParam = %X , wlwParam = %X , lParam = %X\n"), hWnd, uMsg, whwParam, wlwParam, lParam);

			HANDLE hFinder  = GetDlgItem(hWnd, IDC_STATIC3);

			// click in finder tool
			if(hFinder == (HANDLE)lParam){
				SetCapture(hWnd);
				isMouseCapture = TRUE;
			}

			// _tprintf(_T("hWndEdit = %X\n"), hWndEdit);
			// _tprintf(_T("hFinder = %X\n"), hFinder);

			
			//SendMessage((HWND)hWndEdit, WM_SETTEXT, 0, (LPARAM)lpszTestText);
			//SetWindowText((HWND)hWndEdit, lpszTestText);

			// menu command
			if(whwParam == 0 && lParam == 0)
			{
				switch(wlwParam)
				{
					case IDM_EXIT1:
						MessageBox(hWnd, _T("Exit menu click"), _T("Debug"), MB_OK);
					break;
					case IDM_ABOUT____1:
						MessageBox(hWnd, _T("About menu click"), _T("Debug"), MB_OK);
					break;
				}
			}
		}
		break;
		
		case WM_CLOSE:
			DestroyWindow(hWnd);
		break;

		case WM_DESTROY:
			delete lastWnd;
			PostQuitMessage(0);
		break;

		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	return 0;
}


BOOL CALLBACK CtrlInfoDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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


BOOL CALLBACK ProcessInfoDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
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


TCHAR* GetTextFromEdit(HWND hWnd)
{
    DWORD dwBuffSize = SendMessage(hWnd, WM_GETTEXTLENGTH, 0, 0) + 1;
    // _tprintf(_T("dwBuffSize = %i\n"), dwBuffSize);

	TCHAR* buffer =	(TCHAR*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwBuffSize * sizeof(TCHAR));


	SendMessage(hWnd, WM_GETTEXT, dwBuffSize, (LPARAM)buffer);

	return buffer;
}


BOOL initGUI(HWND hWnd)
{
	hTabControl = GetDlgItem(hWnd, IDC_TAB1);
	TCITEM ti;
	ti.mask = TCIF_TEXT; // I'm only having text on the tab
	ti.pszText = _T("Wnd Info");
	TabCtrl_InsertItem(hTabControl, 0, &ti);
    
	ti.pszText = _T("All Windows");
	TabCtrl_InsertItem(hTabControl, 1, &ti);
	TabCtrl_SetCurSel(hTabControl, 0);

	_tprintf(_T("hTabControl = 0x%X\n"), hTabControl);

	hWndSummInfo = CreateWindowEx(WS_EX_CONTROLPARENT,
								  WC_EDIT,
								  _T(""),
								  WS_BORDER | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN | ES_READONLY,
								  5, 30,       // x, y upper-left corner of the window relative to the upper-left corner of the parent window's client area.
								  397, 283,    // nWidth , nHeight 
								  hTabControl, // A handle to the parent or owner window of the window being created.
								  (HMENU)WND_SUMM_INFO_ID,  // For a child window, hMenu specifies the child-window identifier, an integer 
									          // value used by a dialog box control to notify its parent about events. 
									          // The application determines the child-window identifier; it must be unique for all child windows with the same parent window. 
							      GetModuleHandle(NULL),
								  NULL
							    );
	// _tprintf(_T("hWndSummInfo = 0x%X\n"), hWndSummInfo);
	if(hWndSummInfo == 0)
	{
		DWORD dwError = GetLastError();
		_tprintf(_T("hWndSummInfo: dwError = %i\n"), dwError);
		HPrint::printErrorMessage(dwError);
		return FALSE;
	}

	hWndTreeInfo = CreateWindowEx(0,
								  WC_TREEVIEW,
								  _T(""),
								  WS_BORDER | WS_CHILD | WS_HSCROLL | WS_VSCROLL | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES, /* WS_CHILD   */
								  5, 30,                    // x, y upper-left corner of the window relative to the upper-left corner of the parent window's client area.
								  397, 283,                 // nWidth , nHeight 
								  hTabControl,              // A handle to the parent or owner window of the window being created.
								  (HMENU)WND_TREE_INFO_ID,  // For a child window, hMenu specifies the child-window identifier, an integer 
									                        // value used by a dialog box control to notify its parent about events. 
									                        // The application determines the child-window identifier; it must be unique for all child windows with the same parent window. 
							      GetModuleHandle(NULL),
								  NULL
							     );

	_tprintf(_T("hWndTreeInfo = 0x%X\n"), hWndTreeInfo);
	if(hWndTreeInfo == 0)
	{
		DWORD dwError = GetLastError();
		_tprintf(_T("hWndTreeInfo: dwError = %i\n"), dwError);
		HPrint::printErrorMessage(dwError);
		return FALSE;
	}


	treeWindows = new TreeControl(hWndTreeInfo);
	treeWindows->reBuildTree();
	return TRUE;
}
