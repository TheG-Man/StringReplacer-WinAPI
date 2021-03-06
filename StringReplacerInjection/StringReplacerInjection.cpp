// StringReplacerInjection.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

const auto Usage{ "Usage: <process_name>" };
const auto DllFunctionName("LoadLibraryW");

const std::wstring libraryPath{ TEXT("C:\\Users\\nikol\\iCloudDrive\\Programming\\C++\\Projects\\WinAPI\\StringReplacer-WinAPI\\StringReplacer\\Debug\\StringReplacerInjectionDll.dll") };

DWORD GetProcessIdByName(std::wstring &processName);

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << Usage << std::endl;
		return 1;
	}
	auto guard = [](const void *result, const std::string &success, const std::string &failure) {
		if (!result)
		{
			std::cerr << failure << std::endl;
			ExitProcess(GetLastError());
		}
		else
		{
			if (success.length() > 0)
				std::cout << success << std::endl;
		}
	};

	std::wstring processName{ CommandLineToArgvW(GetCommandLine(), &argc)[1] };

	auto hRemoteProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetProcessIdByName(processName));
	guard(hRemoteProcess, "Remote process has been opened", "OpenProcess error");

	auto loadLibrarayW = (LPVOID)GetProcAddress(GetModuleHandle(TEXT("kernel32.dll")), DllFunctionName);
	guard(loadLibrarayW, "", "GetProcAddress error");

	auto argAddress = VirtualAllocEx(hRemoteProcess,  NULL, libraryPath.size()*2, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	guard(argAddress, "", "VirtualAllocEx error");

	auto processMemoryWritten = WriteProcessMemory(hRemoteProcess, argAddress, libraryPath.c_str(), libraryPath.size() * 2, NULL);
	guard(&processMemoryWritten, "", "WriteProcessMemory error");

	auto hRemoteThread = CreateRemoteThread(hRemoteProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibrarayW, argAddress, NULL, NULL);
	guard(hRemoteThread, "Remote thread has been created", "CreateRemoteThread error");

	CloseHandle(hRemoteProcess);

	return 0;
}

DWORD GetProcessIdByName(std::wstring &processName)
{
	auto processId = (DWORD)0;
	auto hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 processEntry;
	ZeroMemory(&processEntry, sizeof(processEntry));
	processEntry.dwSize = sizeof(processEntry);

	auto isFound = false;
	while (Process32Next(hSnapshot, &processEntry) && !isFound)
	{
		if (!processName.compare(processEntry.szExeFile))
		{
			processId = processEntry.th32ProcessID;
			isFound = true;
		}
	}

	CloseHandle(hSnapshot);

	return processId;
}
