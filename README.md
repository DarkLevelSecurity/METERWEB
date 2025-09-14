# METERWEB


I have no energy to explain check the VISION.md.

## How it works

- set up your kali or any linux that contains metasploit framework.
- generate a windows/x64/shell_reverse payload in the raw format, don't forget to insert your machine IP and the intended port
- run the exploit/multi/handler in msfconsole
- run an HTTP server, `python3 -m http.server`
- insert you url of the generated file and the intended process to inject the code to in the `meterweb.h` file
- compile and run the code.

## Targets
I only tested it on windows 10 x64 to the notepad.exe and the code ran without being detected.

## Disclaimer (i think)
This for malware analysits and if you wanna do something illegal please fuck off and check the thousands of malware available there this is just a begineer project.0/[.,]
> Don't use it the bad way
