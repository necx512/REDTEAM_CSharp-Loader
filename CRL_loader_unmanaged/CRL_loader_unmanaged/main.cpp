// code mostly stolen from pabloko's comment in https://gist.github.com/xpn/e95a62c6afcf06ede52568fcd8187cc2
//try opening dll with ildasm
// https://learn.microsoft.com/en-us/answers/questions/370576/ildasm-exe-wont-work
//  res = runtimeHost->ExecuteInDefaultAppDomain(L"C:\\Users\\sebastien.carre\\source\\repos\\CRL_loader_managed\\CRL_loader_managed\\bin\\Release\\net6.0\\CRL_loader_managed.dll", L"CLRHello1.Program", L"spotlessMethod", L"test", &pReturnValue);
//C:\Users\seb\Desktop\GIT\REDTEAM_CSharp-Loader\ATRIER\CRL_loader_managed\CRL_loader_managed\bin\Release\net6.0\CRL_loader_managed.dll


#include <iostream>
#include <metahost.h>
#include <corerror.h>
#pragma comment(lib, "mscoree.lib")

bool FileExists(LPCWSTR filePath) {
    DWORD attributes = GetFileAttributesW(filePath);
    return (attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY));
}

int main()
{
    ICLRMetaHost* metaHost = NULL;
    ICLRRuntimeInfo* runtimeInfo = NULL;
    ICLRRuntimeHost* runtimeHost = NULL;
    DWORD pReturnValue;
    LPCWSTR filePath = L"C:\\Users\\seb\\Desktop\\tmp\\Program.exe";

    HRESULT res = CLRCreateInstance(CLSID_CLRMetaHost, IID_ICLRMetaHost, (LPVOID*)&metaHost);
    if (res != S_OK)
        exit(1);

    res = metaHost->GetRuntime(L"v4.0.30319", IID_ICLRRuntimeInfo, (LPVOID*)&runtimeInfo);
    if (res != S_OK)
        exit(1);

    res = runtimeInfo->GetInterface(CLSID_CLRRuntimeHost, IID_ICLRRuntimeHost, (LPVOID*)&runtimeHost);
    if (res != S_OK)
        exit(1);

    res = runtimeHost->Start();
    if (res != S_OK)
        exit(1);

    // check if the dll file exists
    if (FileExists(filePath) == false) {
        std::cerr << "File does not exist" << std::endl;
        exit(1);
    }

    res = runtimeHost->ExecuteInDefaultAppDomain(filePath, L"CLRHello1.Program", L"spotlessMethod", L"test", & pReturnValue);
    if (res != S_OK)
        std::cout << "ExecuteInDefaultAppDomain failed with code : " << std::hex << res << std::endl;

    return 0;
}
