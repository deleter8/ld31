#ifndef __STRING_T_TO_END_ALL_STRING_Ts__
#define __STRING_T_TO_END_ALL_STRING_Ts__

#include <string>
#include <sstream>

#if defined(_WIN32) || defined(WIN32)
#include <codecvt>
#include <wchar.h>

typedef std::wstring string_t;
typedef std::string internal_string_t;
typedef wchar_t char_t;
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
typedef char char_t;
#define TEXT(s) s
#define s2ws(s) s
#define ws2s(s) s
#endif

#if defined(_WIN32) || defined(WIN32)
#include <codecvt>
typedef std::wstringstream stringstream_t;
typedef std::stringstream  internal_stringstream_t;
#else
typedef std::stringstream stringstream_t;
typedef std::stringstream  internal_stringstream_t;
#endif

#ifndef LEAN_STRING_T
#include <algorithm>
#include <functional>
#include <cctype>
#include <locale>
#include <vector>

inline std::vector<string_t> &split(const string_t &s, char_t delim, std::vector<string_t> &elems) {
	stringstream_t ss(s);
	string_t item;

	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

inline std::vector<string_t> split(const string_t &s, char_t delim) {
	std::vector<string_t> elems;
	split(s, delim, elems);
	return elems;
}

#endif


#endif //__STRING_T_TO_END_ALL_STRING_Ts__