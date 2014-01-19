
#include "TreeControl.h"
#include "IWindow.h"
#include "tstring.h"

TreeControl::TreeControl(HWND hWnd)
{
	this->hWnd = hWnd;

	_tprintf(_T("TreeControl: hwnd = 0x%X\n"), hWnd);

	if(!EnumChildWindows(NULL, addWndHandle, (LPARAM)this)){
		DWORD dwError = GetLastError();
		_tprintf(_T("dwError = %i\n"), dwError);
		Helper::printErrorMessage(dwError);
	}

	_tprintf(_T("hwndTree.size() = %i\n"), hwndTree.size());
	
	BOOL res = addParentChildPairsToWndTree();
	_tprintf(_T("res = %i\n"), res);
	_tprintf(_T("hwndTree.size() = %i\n"), hwndTree.size());

	_tprintf(_T("hwndTree.count(0) = %i\n"), hwndTree.count(0));
	
	buildTree(0, NULL);

/*
    HTREEITEM hPrev = NULL; 
    // HTREEITEM hPrevRootItem = NULL; 
    // HTREEITEM hPrevLev2Item = NULL; 
    // HTREEITEM hti; 

    // tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM; 

	TVITEM tvi; 
	tvi.mask = TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN;
    tvi.pszText = _T("Test 1\nhello ...\nkostja"); 
    tvi.cchTextMax = sizeof(tvi.pszText)/sizeof(tvi.pszText[0]); 
	tvi.cChildren = 1;
    tvi.lParam = 1; 

	TVINSERTSTRUCT tvins; 
    tvins.item = tvi; 
    tvins.hInsertAfter = (HTREEITEM)TVI_LAST; 
	tvins.hParent = TVI_ROOT; 

    
    // Add the item to the tree-view control. 
    hPrev = (HTREEITEM)SendMessage(hWnd, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvins); 

    _tprintf(_T("hPrev = 0x%X\n"), hPrev);

	TVITEM tvi1;
	tvi1.mask = TVIF_TEXT | TVIF_PARAM;
    tvi1.pszText = _T("Test 2\nhello ...\npapa\nmama..."); 
    tvi1.cchTextMax = sizeof(tvi.pszText)/sizeof(tvi.pszText[0]); 
    tvi1.lParam = 2; 

	TVINSERTSTRUCT tvins1; 
    tvins1.item = tvi1; 
    tvins1.hInsertAfter = (HTREEITEM)TVI_LAST; 
	tvins1.hParent = hPrev; // TVI_ROOT; 

	HTREEITEM hPrev1 = (HTREEITEM)SendMessage(hWnd, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvins1);
	_tprintf(_T("hPrev1 = 0x%X\n"), hPrev1);


	TVITEM tvi2; 
	tvi2.mask = TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN;
    tvi2.pszText = _T("Test 3"); 
    tvi2.cchTextMax = sizeof(tvi2.pszText)/sizeof(tvi2.pszText[0]); 
	tvi2.cChildren = 0;
    tvi2.lParam = 3; 

	TVINSERTSTRUCT tvins2; 
    tvins2.item = tvi2; 
    tvins2.hInsertAfter = (HTREEITEM)TVI_LAST; 
	tvins2.hParent = TVI_ROOT; 

    
    // Add the item to the tree-view control. 
    HTREEITEM hPrev2 = (HTREEITEM)SendMessage(hWnd, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvins2); 
*/
}


TreeControl::~TreeControl()
{

}

void TreeControl::buildTree(HWND hWnd, HTREEITEM parent)
{
	int count = hwndTree.count(hWnd);
	if(count == 0){
		return;  
	}

	pair<multimap<HWND, HWND>::iterator, multimap<HWND, HWND>::iterator> ppp;
	 
    // equal_range(b) returns pair<iterator,iterator> representing the range
	// of element with key b
	ppp = hwndTree.equal_range(hWnd);
	
	HWND hw;
	IWindow *iWnd;   

	for (multimap<HWND, HWND>::iterator it = ppp.first; it != ppp.second; ++it)
	{
		hw = it->second;
		iWnd = new IWindow(hw);

		TVITEM tvi; 
	    tvi.mask = TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN;

		TString str;
		str.append(_T("HWND: 0x%X ; Class: %s ; Text: %s ; File: %s"), iWnd->getHWND(), iWnd->getClassName(), iWnd->getText(), iWnd->getExecutableFileName());

		tvi.pszText = (LPTSTR)str.getString(); 
        tvi.cchTextMax = sizeof(tvi.pszText)/sizeof(tvi.pszText[0]); 

		int nChildren = hwndTree.count(hw);
		if(nChildren){
			tvi.cChildren = 1;
		} else {
			tvi.cChildren = 0;
		}
        tvi.lParam = (LPARAM)hw; 

		TVINSERTSTRUCT tvins; 
		tvins.item = tvi; 
		tvins.hInsertAfter = (HTREEITEM)TVI_LAST; 

		if(parent == NULL){
		    tvins.hParent = TVI_ROOT; 
		} else {
			tvins.hParent = parent; 
		}
    
        // Add the item to the tree-view control. 
		HTREEITEM hPrev = (HTREEITEM)SendMessage(this->hWnd, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvins); 
		buildTree(hw, hPrev);

		delete iWnd;
	}
}


BOOL TreeControl::addParentChildPairsToWndTree()
{
	BOOL result = FALSE;

	for (multimap<HWND, HWND>::iterator it = hwndTree.begin(); it != hwndTree.end(); ++it)
	{
		// cout << "  [" << it->first << ", " << it->second << "]" << endl;
		// _tprintf(_T(" [(%i, %i), (%i, %i)]\n"), it->first->getM1(), it->first->getM2(), it->second->getM1(), it->second->getM2());
		HWND hWnd = it->first;

		if(hWnd != 0)
		{
			HWND hpWnd = GetAncestor(hWnd, GA_PARENT);
			pair<HWND, HWND> p(hpWnd, hWnd);

			if(!this->isPairExist(p))
			{
				_tprintf(_T(" (0x%X, 0x%X)\n"), hpWnd, hWnd);
				this->hwndTree.insert(p);
				result = TRUE;
			}
		}
	}

	if(result){
		result = addParentChildPairsToWndTree();
	} 
	
	return result; 
	
}


BOOL CALLBACK TreeControl::addWndHandle(HWND hWnd, LPARAM lParam)
{
//	_tprintf(_T("hwnd = 0x%X\n"), hwnd);

	addToWndTree(hWnd, (TreeControl*)lParam);

	if(!EnumChildWindows(hWnd, addChildWndHandle, lParam)){
		DWORD dwError = GetLastError();
//		_tprintf(_T("dwError = %i\n"), dwError);
//		Helper::printErrorMessage(dwError);
	}

	// Sleep(5000);
/*
	TreeControl *treeControl = (TreeControl*)lParam;
	_tprintf(_T("treeControl->hWnd = 0x%X\n"), treeControl->hWnd);
*/

	return TRUE;
}


BOOL CALLBACK TreeControl::addChildWndHandle(HWND hWnd, LPARAM lParam)
{
	addToWndTree(hWnd, (TreeControl*)lParam);
	return TRUE;
}


BOOL TreeControl::addToWndTree(HWND hWnd, TreeControl *treeCtrl)
{
	HWND hpWnd = GetAncestor(hWnd, GA_PARENT);
	pair<HWND, HWND> p(hpWnd, hWnd);

	if(!treeCtrl->isPairExist(p)){
		treeCtrl->hwndTree.insert(p);
		return TRUE;
	}

	return FALSE;
}

BOOL TreeControl::isPairExist(pair<HWND, HWND> p)
{
	if(hwndTree.count(p.first) == 0){
		return FALSE;
	}

	pair<multimap<HWND, HWND>::iterator, multimap<HWND, HWND>::iterator> range;
	range = hwndTree.equal_range(p.first);

	
	for (multimap<HWND, HWND>::iterator it = range.first; it != range.second; ++it)
	{
		if(it->second == p.second){
			return TRUE;
		}
	}

	return FALSE;
}