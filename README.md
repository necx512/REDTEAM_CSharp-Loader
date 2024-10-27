# REDTEAM_RedTeamNote
Pour ouvrir un projet vide dans vscode en C++, utiliser les projet qui ne sont pas tagué "UWquelquechose". "Projet vide" peut avoir ce tag et donc ne pas l'utiliser

Un PE qui contient du code CLR a un header CLR.
le tool 'ildasm' permet de desassembler le code intermediaire. 


# Compile
Pour le code CSharp: C:\Windows\Microsoft.NET\Framework\v4.0.30319\csc.exe Program.cs
Il y a C:\Windows\Microsoft.NET\Framework64\v4.0.30319\csc.exe mais je n'ai pas testé

Pour le code C, ouvrir la solution dans VSCode et compilé le profile Release/x64


# Ref
https://stackoverflow.com/questions/31258514/loading-assemblies-from-memory-when-hosting-the-clr-in-unmanaged-programs
https://stackoverflow.com/questions/56359368/appdomainptr-load-3-method-example

https://gist.github.com/Arno0x/386ebfebd78ee4f0cbbbb2a7c4405f74
https://gist.github.com/KINGSABRI/e2f7df7972fdb665972bc31b26ac1eb3
https://gist.github.com/aaaddress1/f351d0f75448ae26bcd6ee578536112b

https://0xpat.github.io/Malware_development_part_9

https://www.reddit.com/r/programminghelp/comments/12icrlv/cannot_load_net_assemblies_in_memory/?tl=fr
https://github.com/etormadiv/HostingCLR/issues/1

https://learn.microsoft.com/en-us/windows/win32/midl/com-dcom-and-type-libraries
https://learn.microsoft.com/en-us/answers/questions/215345/running-managed-executables-inside-an-unmanaged-ex

