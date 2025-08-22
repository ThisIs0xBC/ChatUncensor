#include "pch.h"
#include <utils.h>


std::wstring utf8ToWstring(const std::string& str) {
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}


std::string wToString(wchar_t* string) {
	std::wstring ws(string);
	std::string str(ws.begin(), ws.end());
	return str;
}

std::string toLower(std::string str) {
	std::transform(str.begin(), str.end(), str.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return str;
}

// Creating these in hooks will cause memory leak as no way to destroy it, so only used it for testing, can be removed at this point really
FString FS(const std::string& s) {
	wchar_t* p = new wchar_t[s.size() + 1];
	for (std::string::size_type i = 0; i < s.size(); ++i)
		p[i] = s[i];

	p[s.size()] = '\0';
	return FString(p);
}