// StringReplacer.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "StringReplacer.h"

INT __declspec(dllexport) WINAPI replaceStringInCurrentProcess(LPCSTR needle, LPCSTR replacement)
{
	const std::string needleString{ needle };
	const std::string replacementString{ replacement };

	return replaceStringInCurrentProcess(needleString.begin(), needleString.end(), replacementString.begin(), replacementString.end());
}

template <typename Iter>
INT __declspec(dllexport) WINAPI replaceStringInCurrentProcess(Iter &&needleFirst, Iter &&needleLast, Iter &&replacementFirst, Iter &&replacementLast)
{
	auto hProcess = GetCurrentProcess();

	SYSTEM_INFO systemInfo;
	GetSystemInfo(&systemInfo);

	auto minAppAddress = (DWORD_PTR)systemInfo.lpMinimumApplicationAddress;
	auto maxAppAddress = (DWORD_PTR)systemInfo.lpMaximumApplicationAddress;

	CHAR haystack[4096];

	for (auto addr = minAppAddress; addr < maxAppAddress; addr += systemInfo.dwPageSize)
	{
		MEMORY_BASIC_INFORMATION mbi;
		VirtualQueryEx(hProcess, (LPCVOID)(addr), &mbi, sizeof(mbi));

		if (mbi.State == MEM_COMMIT && mbi.Type == MEM_IMAGE && mbi.Protect == PAGE_READWRITE)
		{
			ZeroMemory(haystack, sizeof(haystack));
			ReadProcessMemory(hProcess, (LPVOID)(addr), (LPVOID)haystack, sizeof(haystack), NULL);
			
			auto match{ std::search(std::begin(haystack), std::end(haystack), needleFirst, needleLast) };
			auto isMatch{ BOOL(std::distance(match, std::end(haystack))) };

			while (std::distance(match, std::end(haystack)) > 0)
			{
				auto needleLength{ std::distance(needleFirst, needleLast) };
				auto replacementLength{ std::distance(replacementFirst, replacementLast) };
					
				if (replacementLength > needleLength)
					std::copy_n(replacementFirst, needleLength, match);
				else
					std::copy(replacementFirst, replacementLast, match);
				
				match = std::search(match + needleLength, std::end(haystack), needleFirst, needleLast);
			}

			if (isMatch)
				WriteProcessMemory(hProcess, (LPVOID)addr, (LPCVOID)haystack, sizeof(haystack), NULL);
		}
	}

	return 0;
}
