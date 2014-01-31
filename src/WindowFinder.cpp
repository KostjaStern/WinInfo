
#include "WindowFinder.h"

stack<HWND> WindowFinder::childWnd = stack<HWND>();

HWND WindowFinder::findWindow(HWND hWnd, POINT point)
{
    HWND root = GetAncestor(hWnd, GA_ROOT);
    HWND findWnd = NULL;
    int  wndSize = INT_MAX;


    EnumChildWindows(root, addChildWnd, 0);

    while(!childWnd.empty())
    {
        HWND testWnd = childWnd.top();
        childWnd.pop();

        RECT testWndRect;

        if(!GetWindowRect(testWnd, &testWndRect))
        {
            DWORD dwError = GetLastError();
            _tprintf(_T("GetWindowRect: dwError = %i\n"), dwError);
            HPrint::printErrorMessage(dwError);
            continue;
        }

        if(IsWindowVisible(testWnd) && PtInRect(&testWndRect, point))
        {
            int rectSize = abs((testWndRect.right - testWndRect.left) * (testWndRect.bottom - testWndRect.top));

            if(rectSize < wndSize)
            {
                findWnd = testWnd;
                wndSize = rectSize;
            }
        }
    }

    return findWnd;
}


BOOL WindowFinder::addChildWnd(HWND hWnd, LPARAM lParam)
{
    childWnd.push(hWnd); 
    return TRUE;
}