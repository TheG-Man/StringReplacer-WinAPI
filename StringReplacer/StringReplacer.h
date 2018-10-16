#pragma once

#include "stdafx.h"
#include <iostream>
#include <string>
#include <algorithm>

INT __declspec(dllexport) WINAPI replaceStringInCurrentProcess(LPCSTR needle, LPCSTR replacement);

template <typename Iter>
INT __declspec(dllexport) WINAPI replaceStringInCurrentProcess(Iter &&needleFirst, Iter &&needleLast, Iter &&replacementFirst, Iter &&replacementLast);

