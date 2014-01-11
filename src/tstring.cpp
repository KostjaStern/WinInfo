
#include "tstring.h"
// #include <windows.h>

tString::tString()
{
//	_tprintf(_T("tString::tString()\n"));

	string = new TCHAR[1];
	string[0] = _T('\0');
	length = 0;

	// Sleep(1000);
}

void tString::append(const TCHAR* str, ...)
{
//	_tprintf(_T("tString::tString(const TCHAR* str, ...)\n"));
	va_list args = NULL;
	va_start(args, str);

	append(str, args);

	va_end(args);
	return;
}

void tString::append(const TCHAR* str, va_list args)
{
//	_tprintf(_T("void tString::append(const TCHAR* str, va_list args) \n"));

	if(length == 0)
	{
		delete [] string;
		length = _tvscprintf(str, args);
		string = new TCHAR[length + 1];
		_tvsnprintf_s(string, length + 1, _TRUNCATE, str, args);
		return;
	}
	
	TCHAR *oldBuff = string;

	int newBuffSize = _tvscprintf(str, args);
	TCHAR *newBuff = new TCHAR[newBuffSize + 1];
	_tvsnprintf_s(newBuff, newBuffSize + 1, _TRUNCATE, str, args);

	length += newBuffSize;
	string = new TCHAR[length + 1];
	_tcscpy_s(string, length + 1, oldBuff);
	_tcscat_s(string, length + 1, newBuff);

	delete [] oldBuff;
	delete [] newBuff;
	return;
}

void tString::append(tString str)
{
	if(length == 0)
	{
		length = str.getStrLen();
		string = new TCHAR[length + 1];
		_tcscpy_s(string, length + 1, str.getString());
		return;
	}

	TCHAR* oldBuff = string;

	length += str.getStrLen();
	string = new TCHAR[length + 1];
	_tcscpy_s(string, length + 1, oldBuff);
	_tcscat_s(string, length + 1, str.getString());

	delete [] oldBuff;
	return;
}

tString::tString(const TCHAR* str, ...)
{
	length = 0;

	va_list args = NULL;
	va_start(args, str);

	append(str, args);

	va_end(args);
	return;
}

tString::~tString()
{
//	_tprintf(_T("tString::~tString()\n"));
	delete [] string;
	// Sleep(1000);
}

const TCHAR* tString::getString()
{
	return string;
}

int tString::getStrLen()
{
	return length;
}

