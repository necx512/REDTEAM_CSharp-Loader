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

# TODO
Si je desactive l'AV, le programme prend du temps a ce lancer. ETW??

# Minimal C# loader without check

```C#
#include "sebheaders.h"

#include <stdio.h>
#include <windows.h>
#include <mscoree.h>
#include <metahost.h>
#pragma comment(lib, "MSCorEE.lib")

#import "mscorlib.tlb" auto_rename
using namespace mscorlib;

bool readBinFile_mem_v1(const char fileName[], char*& bufPtr, DWORD& length) {
	if (FILE* fp = fopen(fileName, "rb")) {
		fseek(fp, 0, SEEK_END);
		length = ftell(fp);
		bufPtr = new char[length + 1];
		fseek(fp, 0, SEEK_SET);
		fread(bufPtr, sizeof(char), length, fp);
		return true;
	}
	else return false;
}


int from_memory_v1(int argc, wchar_t* argv[]) {
	PCHAR ptrBinary; 
	DWORD lenBinary;

	bstr_t bstrClassName("CLRHello1.Program");
	bstr_t bstrStaticMethodName(L"spotlessMethod");
	variant_t vtStringArg(L"test");
	_TypePtr spType = NULL;
	SAFEARRAY* psaStaticMethodArgs = NULL;
	SAFEARRAY* pSafeArray = NULL;
	variant_t vtEmpty;
	HRESULT hr;
	LONG index = 0;
	VARIANT _result;
	ICLRRuntimeInfo* pRuntimeInfo = NULL;
	ICorRuntimeHost* pRuntimeHost = NULL;
	ICLRMetaHost* pMetaHost = NULL;
	_AssemblyPtr pAssembly = NULL;
	void* pvData = NULL;
	IUnknownPtr pAppDomainThunk = NULL;
	_AppDomainPtr pDefaultAppDomain = NULL;

	readBinFile_mem_v1("C:/Users/seb/Desktop/GIT/REDTEAM_CSharp-Loader/Program.exe", ptrBinary, lenBinary);

	CLRCreateInstance(CLSID_CLRMetaHost, IID_ICLRMetaHost, (VOID**)&pMetaHost);
	pMetaHost->GetRuntime(L"v4.0.30319", IID_ICLRRuntimeInfo, (VOID**)&pRuntimeInfo);
	pRuntimeInfo->GetInterface(CLSID_CorRuntimeHost, IID_ICorRuntimeHost, (VOID**)&pRuntimeHost);
	pRuntimeHost->Start();
	
	// fetch the default domain
	pRuntimeHost->GetDefaultDomain(&pAppDomainThunk);
	pAppDomainThunk->QueryInterface(__uuidof(_AppDomain), (LPVOID*)&pDefaultAppDomain);

	
	pSafeArray = SafeArrayCreate(VT_UI1, 1, new SAFEARRAYBOUND{ lenBinary , 0 });
	SafeArrayAccessData(pSafeArray, &pvData);
	memcpy(pvData, LPBYTE(ptrBinary), lenBinary);
	SafeArrayUnaccessData(pSafeArray);
	pAssembly = pDefaultAppDomain->Load_3(pSafeArray);

	spType = pAssembly->GetType_2(bstrClassName);//ok
	
	psaStaticMethodArgs = SafeArrayCreateVector(VT_VARIANT, 0, 1);
	SafeArrayPutElement(psaStaticMethodArgs, &index, &vtStringArg);
    VariantInit(&_result);
	hr = spType->raw_InvokeMember_3(bstrStaticMethodName, static_cast<BindingFlags>(BindingFlags_InvokeMethod | BindingFlags_Static | BindingFlags_Public), NULL, vtEmpty, psaStaticMethodArgs, &_result);//OK
	std::cout << std::hex << hr << std::endl; // 0x80131512 COR_E_MISSINGMEMBER https://learn.microsoft.com/fr-fr/dotnet/api/system.missingmemberexception?view=net-8.0
		
	
	return 0;
}
#