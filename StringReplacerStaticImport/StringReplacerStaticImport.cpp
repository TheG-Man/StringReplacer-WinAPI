// StringReplacerStaticImport.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

INT __declspec(dllimport) WINAPI replaceStringInCurrentProcess(LPCSTR needle, LPCSTR replacement);

template <typename Iter>
INT __declspec(dllimport) WINAPI replaceStringInCurrentProcess(Iter &&needleFirst, Iter &&needleLast, Iter &&replacementFirst, Iter &&replacementLast);

const auto Usage{"Usage: <needle> <replacement>"};

CHAR stringToBeReplaced[30];

INT main(INT argc, PCHAR *argv)
{
	if (argc < 3)
	{
		std::cerr << Usage << std::endl;
		return 1;
	}

	strcpy_s(stringToBeReplaced, argv[1]);
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