/******************************************************************************
Module:  TreeControl.h
Notices: Copyright (c) 2014 Kostja Stern
******************************************************************************/

#pragma once


#include <windows.h>
#include <commctrl.h>
#include <tchar.h>

#include <map>
#include <vector>

#include "Debug.h"
#include "IWindow.h"
#include "ProcessInfo.h"
#include "TString.h"



using namespace std;

enum TreeItemType
{
	PROCESS    = 0,
	WINDOW     = 1,
	ROOTWINDOW = 2, 
};


struct TreeItem
{
	DWORD         vItem;  // item value
	TreeItemType  type;   // item type
};

class TreeControl
{
	public:
		TreeControl(HWND hWnd);
		~TreeControl();
		
		void      reBuildTree();
		TreeItem  getSelectedItem();


    private:
	    HWND hWnd;

		multimap<HWND, HWND> *hwndTree;

		BOOL       isPairExist(pair<HWND, HWND> p);
		HTREEITEM  insertItem(HTREEITEM parent, const TCHAR* label, int hasChild, LPARAM param);

		static BOOL CALLBACK addWndHandle(HWND hWnd, LPARAM lParam);
		static BOOL CALLBACK addChildWndHandle(HWND hWnd, LPARAM lParam);
		static BOOL addToWndTree(HWND hWnd, TreeControl *treeCtrl);

		BOOL addParentChildPairsToWndTree();
		void buildTree(HWND hWnd, HTREEITEM parent, int level);
};



