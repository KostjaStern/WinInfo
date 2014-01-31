

#pragma once

#include <windows.h>
#include <stack>


#include "Debug.h"

using namespace std;

class WindowFinder
{
    public:
        static HWND findWindow(HWND hWnd, POINT point);
        
    private:
        static BOOL CALLBACK addChildWnd(HWND hWnd, LPARAM lParam);
        static stack<HWND> childWnd;
};
