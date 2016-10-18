#pragma once
#include<string>
#include<sstream>

class StringUtils {
public:
	static string ws2s(const std::wstring& ws)
	{
		std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
		setlocale(LC_ALL, "chs");
		const wchar_t* _Source = ws.c_str();
		size_t _Dsize = 2 * ws.size() + 1;
		size_t actualSize = 0;
		char *_Dest = new char[_Dsize];
		memset(_Dest, 0, _Dsize);
		wcstombs_s(&actualSize, _Dest,  _Dsize, _Source, _TRUNCATE);
		std::string result = _Dest;
		delete[]_Dest;
		setlocale(LC_ALL, curLocale.c_str());
		return result;
	}

	static wstring s2ws(const std::string& s)
	{
		setlocale(LC_ALL, "chs");
		const char* _Source = s.c_str();
		size_t _Dsize = s.size() + 1;
		size_t actualSize = 0;
		wchar_t *_Dest = new wchar_t[_Dsize];
		wmemset(_Dest, 0, _Dsize);
		mbstowcs_s(&actualSize, _Dest,  _Dsize, _Source, _TRUNCATE);
		std::wstring result = _Dest;
		delete[]_Dest;
		setlocale(LC_ALL, "C");
		return result;
	}

	static string i2s(int value) {
		stringstream ss;
		string buff;
		ss << value;
		ss >> buff;
		return buff;
	}
};