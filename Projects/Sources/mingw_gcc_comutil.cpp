#include "include.h"

#if defined(__MINGW32__)

#include <windows.h>
#include <stdio.h>

/* Support functions for _bstr_t in MinGW: Originally written by: Diaa Sami */
void __stdcall _com_issue_error(HRESULT hr)
{
 printf("_com_issue_error() called with parameter HRESULT = %lu", hr);
}

namespace _com_util
{
 char * __stdcall ConvertBSTRToString(BSTR bstr)
 {
 const unsigned int stringLength = lstrlenW(bstr);
 char *const ascii = new char [stringLength + 1];

 wcstombs(ascii, bstr, stringLength + 1);

 return ascii;
 }

 BSTR __stdcall ConvertStringToBSTR(const char *const ascii)
 {
 const unsigned int stringLength = lstrlenA(ascii);
 BSTR bstr = SysAllocStringLen(NULL, stringLength);

 mbstowcs(bstr, ascii, stringLength + 1);

 return bstr;
 }
}
#endif //defined(__MINGW32__)
