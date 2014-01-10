
/*
   http://www.rsdn.ru/forum/winapi/1354633.all (�������� �������� ���� ��������� �� �������, DLGWINDOWEXTRA)
   http://msdn.microsoft.com/en-us/library/windows/desktop/hh298366%28v=vs.85%29.aspx (How to Create a Tabbed Dialog Box)  

   http://msdn.microsoft.com/en-us/library/windows/desktop/aa366533%28v=vs.85%29.aspx (Comparing Memory Allocation Methods)


   HWND WINAPI GetDesktopWindow(void);
   BOOL RedrawWindow(
	  _In_  HWND hWnd,
	  _In_  const RECT *lprcUpdate,
	  _In_  HRGN hrgnUpdate,
	  _In_  UINT flags
  );


*/

#include "IWindow.h"

// #include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
// #include <tchar.h>
// #include <stdio.h>
// #include <psapi.h>

#include "resource.h"




// http://msdn.microsoft.com/en-us/library/windows/desktop/ms646262%28v=vs.85%29.aspx (SetCapture function)
static BOOL isMouseCapture = FALSE;
static IWindow  *lastWnd = new IWindow();


ATOM RegClass(HINSTANCE hInstance, LPTSTR lpszClassName);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


void SetTextToEdit(HWND hWnd, const TCHAR *text);

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
	LPCTSTR   lpszAppName = _T("App name");

	if(!RegClass(hInstance, (LPTSTR)lpszWndClassName)){
		MessageBox(NULL, _T("Cannot register class"), _T("Error"), MB_OK);
		return 0;
	}

	// init "Common Control Library"
	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icc.dwICC  = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);


    AllocConsole();	
	/*
	tString *str = new tString();
	str->append(_T("icc.dwSize = %i\n"), icc.dwSize);
	str->append(_T("icc.dwSize = %i\n"), icc.dwSize);
	str->append(_T("icc.dwICC = %i\n"),  icc.dwICC);
	_tprintf(str->getString());
	delete str;
	*/
	/*
	delete lastWnd;
	lastWnd = new IWindow((HWND)0x00030652);
	// _tprintf(lastWnd->getWindowInfo());

	delete lastWnd;
	lastWnd = new IWindow((HWND)0x000405E8);
	// _tprintf(lastWnd->getWindowInfo());
	delete lastWnd;

	// delete lastWnd;
	_tprintf(_T("End ...\n"));
	Sleep(15000);
	*/

	hMainWnd = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, NULL);
    SetWindowText(hMainWnd, lpszAppName);
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

	HANDLE hWndEdit = GetDlgItem(hWnd, IDC_EDIT5);
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
				
				if(hSearchWnd != NULL  && hSearchWnd != lastWnd->getWindowHWND() )
				{
					// _tprintf(_T("hSearchWnd = 0x%X\n"), hSearchWnd);



					if(lastWnd->getWindowHWND() != NULL){
						lastWnd->deselectWindow();
						// GetWindowInfoByHWND(hLastWindow, (HWND)hWndEdit);
					}
					
					delete lastWnd; //crash application ???
					lastWnd = new IWindow(hSearchWnd);
					// _tprintf(lastWnd->getWindowInfo());
					 SetTextToEdit((HWND)hWndEdit, lastWnd->getWindowInfo());
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

	    case WM_PAINT:
			/*
			hDC = BeginPaint(hWnd, &ps);

			GetClientRect(hWnd, &rect);
			DrawText(hDC, _T("Hello, Word!"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			EndPaint(hWnd, &ps);
			*/
		break;

		case WM_NOTIFY:
			// _tprintf(_T("WM_NOTIFY:  hWnd = %X , uMsg = %X , wParam = %X , lParam = %X\n"), hWnd, uMsg, wParam, lParam);
		break;

		
		//    http://msdn.microsoft.com/en-us/library/windows/desktop/ms647591%28v=vs.85%29.aspx (WM_COMMAND message)
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

void SetTextToEdit(HWND hWnd, const TCHAR *text)
{
	SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)text);
	SendMessage(hWnd, WM_PAINT, 0, 0);
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

