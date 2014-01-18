
#ifndef TSTRING_H_
#define TSTRING_H_

#include <tchar.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef _UNICODE
#define _tsprintf_s  swprintf_s
#else
#define _tsprintf_s  sprintf_s
#endif

#ifdef _UNICODE
#define _tvsnprintf_s  _vsnwprintf_s
#else
#define _tvsnprintf_s  vsnprintf_s
#endif

#ifdef _UNICODE
#define _tvscprintf  _vscwprintf
#else
#define _tvscprintf  _vscprintf
#endif


class TString
{
    public:
		TString();
		TString(const TCHAR* str, ...);
		~TString();
		void append(const TCHAR* str, ...);
		void append(const TCHAR* str, va_list args);
		void append(TString str);
		const TCHAR* getString() const { return string; }
		int getStrLen() const { return length; }

    private:
	    TCHAR* string;
		int    length;
};


#endif