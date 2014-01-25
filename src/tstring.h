
#pragma once

#include <tchar.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef _UNICODE
	#ifndef _tsprintf_s
		#define _tsprintf_s  swprintf_s
		#else
		#define _tsprintf_s  sprintf_s
    #endif
#endif

#ifdef _UNICODE
    #ifndef _tvsnprintf_s
		#define _tvsnprintf_s  _vsnwprintf_s
		#else
		#define _tvsnprintf_s  vsnprintf_s
	#endif
#endif

#ifdef _UNICODE
	#ifndef _tvscprintf
		#define _tvscprintf  _vscwprintf
		#else
		#define _tvscprintf  _vscprintf
    #endif
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


