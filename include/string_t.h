#ifndef __STRING_T_TO_END_ALL_STRING_Ts__
#define __STRING_T_TO_END_ALL_STRING_Ts__

#include <string>

#if defined(_WIN32) || defined(WIN32)
#include <codecvt>

typedef std::wstring string_t;
typedef std::string internal_string_t;
#define TEXT(s) L ## s

inline std::wstring _s2ws(const std::string& str)
{
	typedef std::codecvt_utf8<wchar_t> convert_typeX;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

inline std::string _ws2s(const std::wstring& wstr)
{
	typedef std::codecvt_utf8<wchar_t> convert_typeX;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

#define s2ws _s2ws
#define ws2s _ws2s


#else

typedef std::string string_t;
typedef std::string internal_string_t;
#define TEXT(s) s
#define s2ws(s) s
#define ws2s(s) s
#endif

#endif //__STRING_T_TO_END_ALL_STRING_Ts__