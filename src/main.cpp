

#include "main.h"

#define WND_TREE_INFO_ID  234

// http://msdn.microsoft.com/en-us/library/windows/desktop/ms646262%28v=vs.85%29.aspx (SetCapture function)
BOOL isMouseCapture = FALSE;
IWindow *lastWnd = NULL;

TreeControl *treeWindows = NULL;

HWND hTabControl = NULL;
HWND hWndSummInfo = NULL;
// TString *wndSummText = NULL;


// http://www.codeguru.com/cpp/controls/treeview/misc-advanced/article.php/c3993/Using-TreeControl-TreeView-under-Win32-API-No-MFC.htm
// http://winapi.foosyerdoos.org.uk/code/commoncntrls/htm/createtreeview.php
HWND hWndTreeInfo = NULL; 


ATOM RegClass(HINSTANCE hInstance, LPTSTR lpszClassName);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

void InitTree(HWND hwndTV);
void SetTextToEdit(HWND hWnd, IWindow *wnd);

// void AddTextToEdit(HWND hWnd, LPCTSTR format, ...);


TCHAR* GetTextFromEdit(HWND hWnd);

/*
void SelectWindow(HWND hWnd);
void DeselectWindow(HWND hWnd);
*/

void PrintErrorMessage(DWORD code);

/*
void GetWindowInfoByHWND(HWND hWnd, HWND hEdit);
void PrintWindowStyle(HWND hEdit, DWORD style);
void PrintWindowStyleEx(HWND hEdit, DWORD style);
*/

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE, PTSTR pszCmdLine, int nCmdShow)
{
	HWND hMainWnd;
	MSG  msg;
	LPCTSTR   lpszWndClassName = _T("MainWndClass");
	LPCTSTR   lpszAppName = _T("WinInfo");

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
	FILE *hf = _fdopen( _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), _O_TEXT | _O_WTEXT), "w");
    *stdout = *stderr = *hf;    /* enable using _tprintf  */

	// Sleep(1000);
	/*
	TString *str = new TString();
	str->append(_T("icc.dwSize = %i\n"), icc.dwSize);
	str->append(_T("icc.dwSize = %i\n"), icc.dwSize);
	str->append(_T("icc.dwICC = %i\n"),  icc.dwICC);
	_tprintf(str->getString());
	delete str;

	
	// delete lastWnd;
	lastWnd = new IWindow((HWND)0x000405CA);
	_tprintf(_T("lastWnd->getClassName() = %s\n"), lastWnd->getClassName());

	delete lastWnd;
	lastWnd = new IWindow((HWND)0x00010604);
	_tprintf(_T("lastWnd->getClassName() = %s\n"), lastWnd->getClassName());
	delete lastWnd;

	// delete lastWnd;
	_tprintf(_T("End ...\n"));
	Sleep(15000);
	*/

	hMainWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, NULL);
    SetWindowText(hMainWnd, lpszAppName);


	hTabControl = GetDlgItem(hMainWnd, IDC_TAB1);
	TCITEM ti;
	ti.mask = TCIF_TEXT; // I'm only having text on the tab
	ti.pszText = _T("Wnd Info");
	TabCtrl_InsertItem(hTabControl, 0, &ti);
    
	ti.pszText = _T("All Windows");
	TabCtrl_InsertItem(hTabControl, 1, &ti);
	TabCtrl_SetCurSel(hTabControl, 0);

	hWndSummInfo = CreateWindowEx(WS_EX_CONTROLPARENT,
								  WC_EDIT,
								  _T(""),
								  WS_BORDER | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_WANTRETURN | ES_READONLY,
								  5, 30,       // x, y upper-left corner of the window relative to the upper-left corner of the parent window's client area.
								  397, 283,    // nWidth , nHeight 
								  hTabControl, // A handle to the parent or owner window of the window being created.
								  NULL,        // For a child window, hMenu specifies the child-window identifier, an integer 
									          // value used by a dialog box control to notify its parent about events. 
									          // The application determines the child-window identifier; it must be unique for all child windows with the same parent window. 
							      hInstance,
								  NULL
							    );
	// _tprintf(_T("hWndSummInfo = 0x%X\n"), hWndSummInfo);
	if(hWndSummInfo == 0)
	{
		DWORD dwError = GetLastError();
		_tprintf(_T("dwError = %i\n"), dwError);
		PrintErrorMessage(dwError);
	}


	hWndTreeInfo = CreateWindowEx(0,
								  WC_TREEVIEW,
								  _T(""),
								  WS_BORDER | WS_CHILD | WS_HSCROLL | WS_VSCROLL | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES,
								  5, 30,                    // x, y upper-left corner of the window relative to the upper-left corner of the parent window's client area.
								  397, 283,                 // nWidth , nHeight 
								  hTabControl,              // A handle to the parent or owner window of the window being created.
								  (HMENU)WND_TREE_INFO_ID,  // For a child window, hMenu specifies the child-window identifier, an integer 
									                        // value used by a dialog box control to notify its parent about events. 
									                        // The application determines the child-window identifier; it must be unique for all child windows with the same parent window. 
							      hInstance,
								  NULL
							     );
	// treeWindows = new TreeControl(hWndTreeInfo);

	// _tprintf(_T("hWndSummInfo = 0x%X\n"), hWndSummInfo);
	if(hWndTreeInfo == 0)
	{
		DWORD dwError = GetLastError();
		_tprintf(_T("dwError = %i\n"), dwError);
		PrintErrorMessage(dwError);
	}

	InitTree(hWndTreeInfo);

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

	_tprintf(_T("end prog ..."));
	Sleep(1000);
	FreeConsole();
    return (int)msg.wParam;

//	return 0;
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
	/*
	HDC hDC;
	PAINTSTRUCT ps;
	RECT rect;
	*/

	// HANDLE hWndEdit = GetDlgItem(hWnd, IDC_EDIT1);
	HANDLE hFinder  = GetDlgItem(hWnd, IDC_STATIC3);
	

	// const int BUFF_SIZE = 200;
	// TCHAR     buffer[BUFF_SIZE];

	// TCHAR *buffer;

	DWORD dwError;
	WORD whwParam, wlwParam;
	// int xPos, yPos;
	POINT point;
	HWND  hSearchWnd;

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
				    PrintErrorMessage(dwError);
				}
				isMouseCapture = FALSE;
			}

		break;

		// case WM_LBUTTONDOWN:
	    case WM_MOUSEMOVE:
			// The coordinate is relative to the upper-left corner of the client area. 
			// http://msdn.microsoft.com/en-us/library/windows/desktop/ms645616%28v=vs.85%29.aspx (WM_MOUSEMOVE message)
			// xPos = GET_X_LPARAM(lParam); 
            // yPos = GET_Y_LPARAM(lParam); 
			// _tprintf(_T("isMouseCapture = %i\n"), isMouseCapture);

			if(isMouseCapture)
			{
				if(!GetCursorPos(&point)){
					dwError = GetLastError();
				    _tprintf(_T("GetCursorPos: dwError = %i\n"), dwError);
				    PrintErrorMessage(dwError);
				}

				hSearchWnd = WindowFromPoint(point);
				

				if(hSearchWnd != NULL  && (lastWnd == NULL || (lastWnd != NULL && hSearchWnd != lastWnd->getHWND())))
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
					SetTextToEdit((HWND)hWndSummInfo, lastWnd);
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
							_tprintf(_T("NM_CLICK\n"));

							_tprintf(_T("hWndTreeInfo = 0x%X\n"), hWndTreeInfo);
							_tprintf(_T("lpNmHdr->hwndFrom = 0x%X\n"), lpNmHdr->hwndFrom);
							UpdateWindow(hWnd);
						break;
						case NM_DBLCLK:
							_tprintf(_T("NM_DBLCLK\n"));
						break;
						case TVN_SELCHANGED:
							_tprintf(_T("TVN_SELCHANGED\n"));
						break;
					}
				break;

				case IDC_TAB1:
					switch(lpNmHdr->code)
					{
						case TCN_SELCHANGING: // changed the tab selection (clicked on another tab)
						{
							int tabId = TabCtrl_GetCurSel(hTabControl);
					
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

			whwParam = HIWORD(wParam);
			wlwParam = LOWORD(wParam);

			// _tprintf(_T("WM_COMMAND:  hWnd = %X , uMsg = %X , whwParam = %X , wlwParam = %X , lParam = %X\n"), hWnd, uMsg, whwParam, wlwParam, lParam);

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

void SetTextToEdit(HWND hWnd, IWindow *wnd)
{
	if(wnd == NULL) return;

	TString *wndSummText = new TString();

	wndSummText->append(_T(">>>> Root Window <<<<\r\n"));

	IWindow *rootWnd = new IWindow(wnd->getRootHWND());
	wndSummText->append(_T("Title: %s\r\n"), rootWnd->getText());
	wndSummText->append(_T("Class: %s\r\n"), rootWnd->getClassName());
	wndSummText->append(_T("Position: (left = %i, top = %i)\r\n"), rootWnd->getWndPos().x, rootWnd->getWndPos().y);
	wndSummText->append(_T("Size: (width = %i, height = %i)\r\n"), rootWnd->getWidth(), rootWnd->getHeight());
	wndSummText->append(_T("Style: 0x%X\r\n"), rootWnd->getStyle());
	wndSummText->append(_T("ExStyle: 0x%X\r\n"), rootWnd->getExStyle());
	wndSummText->append(_T("Handle: 0x%X\r\n"), rootWnd->getHWND());
	wndSummText->append(_T("\r\n"));

	
	IWindow *parentWnd = new IWindow(wnd->getParentHWND());
	wndSummText->append(_T(">>>> Parent Window <<<<\r\n"));
	wndSummText->append(_T("Text: %s\r\n"), parentWnd->getText());
	wndSummText->append(_T("Class: %s\r\n"), parentWnd->getClassName());
	wndSummText->append(_T("Position: (left = %i, top = %i)\r\n"), parentWnd->getWndPos().x, parentWnd->getWndPos().y);
	wndSummText->append(_T("Size: (width = %i, height = %i)\r\n"), parentWnd->getWidth(), parentWnd->getHeight());
	wndSummText->append(_T("Style: 0x%X\r\n"), parentWnd->getStyle());
	wndSummText->append(_T("ExStyle: 0x%X\r\n"), parentWnd->getExStyle());
	wndSummText->append(_T("Handle: 0x%X\r\n"), parentWnd->getHWND());
	wndSummText->append(_T("\r\n"));

	wndSummText->append(_T(">>>> Control <<<<\r\n"));
	wndSummText->append(_T("Text: %s\r\n"), wnd->getText());
	wndSummText->append(_T("Class: %s\r\n"), wnd->getClassName());
	wndSummText->append(_T("ID: %ld\r\n"), wnd->getID());
	wndSummText->append(_T("Position: (left = %i, top = %i)\r\n"), wnd->getWndPos().x, wnd->getWndPos().y);
	wndSummText->append(_T("Size: (width = %i, height = %i)\r\n"), wnd->getWidth(), wnd->getHeight());
	wndSummText->append(_T("Style: 0x%X\r\n"), wnd->getStyle());
	wndSummText->append(_T("ExStyle: 0x%X\r\n"), wnd->getExStyle());
	wndSummText->append(_T("Handle: 0x%X\r\n"), wnd->getHWND());


	_tprintf(wndSummText->getString());

	SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)wndSummText->getString());
	SendMessage(hWnd, WM_PAINT, 0, 0);

	delete rootWnd;
	delete parentWnd;
	delete wndSummText;
}


TCHAR* GetTextFromEdit(HWND hWnd)
{
    DWORD dwBuffSize = SendMessage(hWnd, WM_GETTEXTLENGTH, 0, 0) + 1;
    // _tprintf(_T("dwBuffSize = %i\n"), dwBuffSize);

	TCHAR* buffer =	(TCHAR*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwBuffSize * sizeof(TCHAR));


	SendMessage(hWnd, WM_GETTEXT, dwBuffSize, (LPARAM)buffer);

	return buffer;
}

void PrintErrorMessage(DWORD dwError)
{
    HLOCAL hlocal = NULL;
    DWORD systemLocale = MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT);

    BOOL fOk = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, 
                             NULL, dwError, systemLocale, (PTSTR) &hlocal, 0, NULL);

    if (!fOk) 
	{
        // Is it a network-related error?
        HMODULE hDll = LoadLibraryEx(TEXT("netmsg.dll"), NULL, 
            DONT_RESOLVE_DLL_REFERENCES);

        if (hDll != NULL) {
            fOk = FormatMessage(
               FORMAT_MESSAGE_FROM_HMODULE | FORMAT_MESSAGE_IGNORE_INSERTS |
               FORMAT_MESSAGE_ALLOCATE_BUFFER,
               hDll, dwError, systemLocale,
               (PTSTR) &hlocal, 0, NULL);
            FreeLibrary(hDll);
        }
    }

    if (fOk && (hlocal != NULL)) {
        _tprintf(_T("%s\n"), (LPTSTR)hlocal);
        LocalFree(hlocal);
    } else {
		_tprintf(_T("%s\n"), _T("No text found for this error number."));
    }
}


// Adds items to a tree-view control. 
// Returns the handle to the newly added item. 
// hwndTV - handle to the tree-view control. 
// lpszItem - text of the item to add. 
// nLevel - level at which to add the item. 
//
// g_nClosed, and g_nDocument - global indexes of the images.

void InitTree(HWND hwndTV)
{
	_tprintf(_T("InitTree\n"));

    HTREEITEM hPrev = NULL; 
    // HTREEITEM hPrevRootItem = NULL; 
    // HTREEITEM hPrevLev2Item = NULL; 
    // HTREEITEM hti; 

    /* tvi.mask = TVIF_TEXT | TVIF_IMAGE 
               | TVIF_SELECTEDIMAGE | TVIF_PARAM; */

	TVITEM tvi; 
    
	tvi.mask = TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN;

    // Set the text of the item. 
    tvi.pszText = _T("Test 1"); 
    tvi.cchTextMax = sizeof(tvi.pszText)/sizeof(tvi.pszText[0]); 
	tvi.cChildren = 1;
    tvi.lParam = 1; 

	TVINSERTSTRUCT tvins; 
    tvins.item = tvi; 
    tvins.hInsertAfter = (HTREEITEM)TVI_FIRST; 
	tvins.hParent = TVI_ROOT; 

    

    // Add the item to the tree-view control. 
    hPrev = (HTREEITEM)SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvins); 

    _tprintf(_T("hPrev = 0x%X\n"), hPrev);

	TVITEM tvi1;
	tvi1.mask = TVIF_TEXT | TVIF_PARAM;
    tvi1.pszText = _T("Test 2"); 
    tvi1.cchTextMax = sizeof(tvi.pszText)/sizeof(tvi.pszText[0]); 
    tvi1.lParam = 2; 

	TVINSERTSTRUCT tvins1; 
    tvins1.item = tvi1; 
    tvins1.hInsertAfter = (HTREEITEM)TVI_FIRST; 
	tvins1.hParent = hPrev; // TVI_ROOT; 

	HTREEITEM hPrev1 = (HTREEITEM)SendMessage(hwndTV, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvins1);
	_tprintf(_T("hPrev1 = 0x%X\n"), hPrev1);

    return; 
} 

// Extracts heading text and heading levels from a global 
// array and passes them to a function that adds them as
// parent and child items to a tree-view control. 
// Returns TRUE if successful, or FALSE otherwise. 
// hwndTV - handle to the tree-view control. 
/*
BOOL InitTreeViewItems(HWND hwndTV)
{ 
    HTREEITEM hti;

    // g_rgDocHeadings is an application-defined global array of 
    // the following structures: 
    //     typedef struct 
    //       { 
    //         TCHAR tchHeading[MAX_HEADING_LEN]; 
    //         int tchLevel; 
    //     } Heading; 
    for (int i = 0; i < ARRAYSIZE(g_rgDocHeadings); i++) 
    { 
        // Add the item to the tree-view control. 
        hti = AddItemToTree(hwndTV, g_rgDocHeadings[i].tchHeading, 
            g_rgDocHeadings[i].tchLevel); 

        if (hti == NULL)
            return FALSE;
    } 
           
    return TRUE; 
}
*/