/*COPYRIGHY BY LEXA
* 
  DEVELOPED BY LEXA

  SINCE 2020

  C++ & C# DEV
  
  16 YO

  DLL INJECTOR
*/

#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>

DWORD getProcess(char* processName) {
	HANDLE hPID = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 procEntry;
	procEntry.dwSize = sizeof(procEntry);

	do {
		if (!strcmp(procEntry.szExeFile, processName)) {
			DWORD dwPID = procEntry.th32ProcessID;
			CloseHandle(hPID);

			return dwPID;
		}
	} while (Process32Next(hPID, &procEntry));
}

int main() {
	// declare variables
	char process[MAX_PATH];
	char dll[MAX_PATH];

	DWORD dwProcess;
	char myDLL[MAX_PATH];

	// ask for input
	printf("process: ");
	scanf_s("%259s", process, MAX_PATH - 1);

	printf("dll: ");
	scanf_s("%259s", dll, MAX_PATH - 1);

	// get path of DLL
	GetFullPathNameA(dll, MAX_PATH, myDLL, NULL);

	// get process
	dwProcess = getProcess(process);

	// open handle and allocate mem
	HANDLE hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, dwProcess);
	LPVOID allocatedMem = VirtualAllocEx(hProcess, NULL, sizeof(myDLL), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	// "inject" DLL into process mem
	WriteProcessMemory(hProcess, allocatedMem, myDLL, sizeof(myDLL), NULL);

	// launch DLL
	CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibrary, allocatedMem, 0, 0);

	// close handle
	CloseHandle(hProcess);

	return 0;
}