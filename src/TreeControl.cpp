
#include "TreeControl.h"


TreeControl::TreeControl(HWND hWnd)
{
	this->hWnd = hWnd;

	_tprintf(_T("TreeControl: hwnd = 0x%X\n"), hWnd);
}


TreeControl::~TreeControl()
{

}


void TreeControl::reBuildTree()
{
	_tprintf(_T("reBuildTree\n"));
	hwndTree = new multimap<HWND, HWND>();

	if(!EnumChildWindows(NULL, addWndHandle, (LPARAM)this)){
		DWORD dwError = GetLastError();
		_tprintf(_T("EnumChildWindows: dwError = %i\n"), dwError);
		HPrint::printErrorMessage(dwError);
	}

	_tprintf(_T("hwndTree.size() = %i\n"), hwndTree->size());
	
	BOOL res = addParentChildPairsToWndTree();
	_tprintf(_T("res = %i\n"), res);
	_tprintf(_T("hwndTree.size() = %i\n"), hwndTree->size());
	_tprintf(_T("hwndTree.count(0) = %i\n"), hwndTree->count(0));
	
	TreeView_DeleteAllItems(this->hWnd);
	buildTree(0, TVI_ROOT, 1);

	hwndTree->clear();
	delete hwndTree;
}



void TreeControl::buildTree(HWND hWnd, HTREEITEM parent, int level)
{
	int count = hwndTree->count(hWnd);
	if(count == 0){
		return;  
	}

	pair<multimap<HWND, HWND>::iterator, multimap<HWND, HWND>::iterator> range;
	 
	// representing the range of element with key hWnd
	range = hwndTree->equal_range(hWnd);
	
	HWND hw;
	IWindow *iWnd;   


	switch(level)
	{
	    case 1: // add root item
		{
			multimap<HWND, HWND>::const_iterator treeIter = hwndTree->find(0);
			if(treeIter == hwndTree->end()){
				return;
			}

			hw = treeIter->second;
			iWnd = new IWindow(hw);

			TString str;
			str.append(_T("HWND: 0x%X ; Class: %s"), iWnd->getHWND(), iWnd->getClassName());
			if(_tcscmp(iWnd->getText(), _T(""))){ // if not empty string
				str.append(_T(" ; Text: %s"), iWnd->getText());
			}

			ProcessInfo processInfo(iWnd->getProcessID());
			delete iWnd;

			if(_tcscmp(processInfo.getFileName(), _T(""))){ // if not empty string
				str.append(_T(" ; FileName : %s"), processInfo.getFileName());
			}
				
			HTREEITEM hPrev = insertItem(parent, str.getString(), hwndTree->count(hw), (LPARAM)hw);
			buildTree(hw, hPrev, 2);
		}
		break;

		case 2: // add process and windows which this process created as items tree
		{
			map<DWORD, HTREEITEM> processes;
			
			for (multimap<HWND, HWND>::iterator it = range.first; it != range.second; ++it)
			{
				hw = it->second;
				DWORD PID = NULL;
				GetWindowThreadProcessId(hw, &PID);
				HTREEITEM hpItem;

				if(processes.find(PID) == processes.end())
				{
					ProcessInfo processInfo(PID);
					hpItem = insertItem(parent, processInfo.getFileName(), 1, (LPARAM)PID);
				    processes.insert(pair<DWORD, HTREEITEM>(PID, hpItem));
				}
				else {
					hpItem = processes[PID];
				}

				iWnd = new IWindow(hw);

				TString str;
				str.append(_T("HWND: 0x%X ; Class: %s"), iWnd->getHWND(), iWnd->getClassName());
				if(_tcscmp(iWnd->getText(), _T(""))){ // if not empty string
					str.append(_T(" ; Text: %s"), iWnd->getText());
				}
				delete iWnd;

				int countChilds = hwndTree->count(hw);
				HTREEITEM hPrev = insertItem(hpItem, str.getString(), countChilds, (LPARAM)hw); 
				if(countChilds){
				    buildTree(hw, hPrev, 4);
				}
			}
		}
		break;

		default: // add window as another window child
			for (multimap<HWND, HWND>::iterator it = range.first; it != range.second; ++it)
			{
				hw = it->second;
				iWnd = new IWindow(hw);

				TString str;
				str.append(_T("HWND: 0x%X ; Class: %s"), iWnd->getHWND(), iWnd->getClassName());
				if(_tcscmp(iWnd->getText(), _T(""))){ // if not empty string
					str.append(_T(" ; Text: %s"), iWnd->getText());
				}
				delete iWnd;

				int countChilds = hwndTree->count(hw);
				HTREEITEM hPrev = insertItem(parent, str.getString(), countChilds, (LPARAM)hw); 
				if(countChilds){
				    buildTree(hw, hPrev, level + 1);
				}
			}
		break;
	}
	
}


TreeItem  TreeControl::getSelectedItem()
{
	HTREEITEM item = TreeView_GetSelection(hWnd);
	HTREEITEM itemParent = item;

	int level = 0;
	while(itemParent != NULL){
		itemParent = TreeView_GetParent(hWnd, itemParent);
		level++;
	}

	_tprintf(_T("level = %i\n"), level);

	TVITEM tvItem;
    tvItem.mask = TVIF_PARAM | TVIF_HANDLE;
	tvItem.hItem = item;

	TreeView_GetItem(hWnd, &tvItem);
	
	TreeItem treeItem;
	treeItem.vItem = (DWORD)tvItem.lParam;

	switch(level)
	{
	    case 1:
			treeItem.type = ROOTWINDOW;
		break;

	    case 2:
			treeItem.type = PROCESS;
		break;

	    default:
			treeItem.type = WINDOW;
		break;
	}

	return treeItem;
}

HTREEITEM  TreeControl::insertItem(HTREEITEM parent, const TCHAR* label, int hasChild, LPARAM param)
{
	TVITEM tvi; 
	tvi.mask = TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN;
	tvi.pszText = (LPTSTR)label; 
	tvi.cchTextMax = sizeof(tvi.pszText)/sizeof(tvi.pszText[0]); 
	if(hasChild){
		tvi.cChildren = 1;
	} else {
		tvi.cChildren = 0;
	}
	tvi.lParam = param; 

	TVINSERTSTRUCT tvins; 
	tvins.item = tvi; 
	tvins.hInsertAfter = (HTREEITEM)TVI_LAST; 
	tvins.hParent = parent;

	return (HTREEITEM)SendMessage(this->hWnd, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvins); 
}

BOOL TreeControl::addParentChildPairsToWndTree()
{
	BOOL result = FALSE;

	for (multimap<HWND, HWND>::iterator it = hwndTree->begin(); it != hwndTree->end(); ++it)
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
				this->hwndTree->insert(p);
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
	addToWndTree(hWnd, (TreeControl*)lParam);

	if(!EnumChildWindows(hWnd, addChildWndHandle, lParam)){
//		DWORD dwError = GetLastError();
//		_tprintf(_T("dwError = %i\n"), dwError);
//		Helper::printErrorMessage(dwError);
	}

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
		treeCtrl->hwndTree->insert(p);
		return TRUE;
	}

	return FALSE;
}

BOOL TreeControl::isPairExist(pair<HWND, HWND> p)
{
	if(hwndTree == NULL || hwndTree->count(p.first) == 0){
		return FALSE;
	}

	pair<multimap<HWND, HWND>::iterator, multimap<HWND, HWND>::iterator> range;
	range = hwndTree->equal_range(p.first);

	
	for (multimap<HWND, HWND>::iterator it = range.first; it != range.second; ++it)
	{
		if(it->second == p.second){
			return TRUE;
		}
	}

	return FALSE;
}