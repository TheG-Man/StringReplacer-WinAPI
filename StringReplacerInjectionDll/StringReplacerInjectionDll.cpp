// StringReplacerInjectionDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"

INT __declspec(dllimport) WINAPI replaceStringInCurrentProcess(LPCSTR needle, LPCSTR replacement);

const auto Needle{"economically"};
const auto Replacement{"frugally"};

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	replaceStringInCurrentProcess(Needle, Replacement);
	return TRUE;
}
