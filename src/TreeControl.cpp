
#include "TreeControl.h"
#include "IWindow.h"

TreeControl::TreeControl(HWND hWnd)
{
	this->hWnd = hWnd;

	_tprintf(_T("TreeControl: hwnd = 0x%X\n"), hWnd);

	if(!EnumWindows(addWndHandle, (LPARAM)this)){
		DWORD dwError = GetLastError();
		_tprintf(_T("dwError = %i\n"), dwError);
	}
}


TreeControl::~TreeControl()
{

}


BOOL CALLBACK TreeControl::addWndHandle(HWND hwnd, LPARAM lParam)
{
	_tprintf(_T("hwnd = 0x%X\n"), hwnd);



/*
	TreeControl *treeControl = (TreeControl*)lParam;
	_tprintf(_T("treeControl->hWnd = 0x%X\n"), treeControl->hWnd);
*/

	return TRUE;
}


BOOL CALLBACK TreeControl::addChildWndHandle(HWND hwnd, LPARAM lParam)
{
	_tprintf(_T("child: hwnd = 0x%X\n"), hwnd);
	return TRUE;
}

BOOL TreeControl::isPairExist(pair<HWND, HWND> p)
{
	if(wndTree.count(p.first) == 0){
		return FALSE;
	}

	pair<multimap<HWND, HWND>::iterator, multimap<HWND, HWND>::iterator> range;
	range = wndTree.equal_range(p.first);

	
	for (multimap<HWND, HWND>::iterator it = range.first; it != range.second; ++it)
	{
		if(it->second == p.second){
			return TRUE;
		}
	}

	return FALSE;
}