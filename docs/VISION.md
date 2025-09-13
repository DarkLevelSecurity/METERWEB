# METERWEB

## Overview
This will be a simple initial stager project.

## How it works
**Stager**
1. get the obfuscated (IPv4) payload from a web server.
2. decode it (IPv4).
3. Inject it to the firefox.exe process.
4. self destroy.
**payload**
1. reverse shell to a msfconsole listner.

## Tools
- C -> main stager.exe
- msfvenom -> the meterpreter shellcode generator
- msfconsole -> listner
- python3 -> for the `-m http.server`
- HellShell -> for the IPv4 en/decoding functions

## Lab
```txt
 ______________
| windows    *-------______> virtual network
|      ________|     |
|     |   *-----------
|     | kali.vm|
|_____|________|
```

## Journalying
Well all the code works well except one part, *Process Injection*.

My process injection code inject bytes but I need it to inject/load PE image, .EXE to be spicific
