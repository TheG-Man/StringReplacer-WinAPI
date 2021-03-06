// StringReplacerDinamicImport.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

const auto Usage{ "Usage: <needle> <replacement>" };
const auto LibraryPath{"C:\\Users\\nikol\\iCloudDrive\\Programming\\C++\\Projects\\WinAPI\\StringReplacer-WinAPI\\StringReplacer\\Debug\\StringReplacer.dll"};
const auto LoadLibraryFailureMessage("LoadLibrary failed.");
const auto GetProcAddressFailureMessage("GetProcAddress failed.");
const auto DllFunctionName("?replaceStringInCurrentProcess@@YGHPBD0@Z");

using TReplaceStringInCurrentProcess = int (WINAPI*)(LPCSTR needle, LPCSTR replacement);

CHAR stringToBeReplaced[30];

INT main(INT argc, PCHAR *argv)
{
	if (argc < 3)
	{
		std::cerr << Usage << std::endl;
		return 1;
	}

	strcpy_s(stringToBeReplaced, argv[1]);
	
	auto hStringReplacerDll = LoadLibraryA(LibraryPath);
	if (hStringReplacerDll == NULL)
	{
		std::cerr << LoadLibraryFailureMessage << std::endl;
		return GetLastError();
	}

	auto replaceStringInCurrentProcess = (TReplaceStringInCurrentProcess)GetProcAddress(hStringReplacerDll, DllFunctionName);
	if (replaceStringInCurrentProcess == NULL)
	{
		std::cerr << GetProcAddressFailureMessage << std::endl;
		return GetLastError();
	}
	
	std::cout << "Before: " << stringToBeReplaced << " (" << &stringToBeReplaced << ")" << std::endl;

	if (auto result = replaceStringInCurrentProcess(argv[1], argv[2]) == 0)
	{
		std::cout << "The string was replaced successfully" << std::endl;
	}
	else
	{
		std::cerr << result << std::endl;
		return result;
	}

	std::cout << "After: " << stringToBeReplaced << std::endl;

	return 0;
}
