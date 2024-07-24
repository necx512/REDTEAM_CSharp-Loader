// code mostly stolen from pabloko's comment in https://gist.github.com/xpn/e95a62c6afcf06ede52568fcd8187cc2
#include <iostream>
#include <metahost.h>
#include <corerror.h>
#include <vector>
#pragma comment(lib, "mscoree.lib")

int main()
{
    ICLRMetaHost* metaHost = NULL;
    ICLRRuntimeInfo* runtimeInfo = NULL;
    ICLRRuntimeHost* runtimeHost = NULL;
    DWORD pReturnValue;

    HRESULT hr;

    hr = CLRCreateInstance(CLSID_CLRMetaHost, IID_ICLRMetaHost, (LPVOID*)&metaHost);
    if (hr != S_OK) {
        std::cout << "Error CLRCreateInstance" << std::endl;
        return -1;
    }
    else {
        std::cout << "CLRCreateInstance OK" << std::endl;
    }
    
    hr = metaHost->GetRuntime(L"v4.0.30319", IID_ICLRRuntimeInfo, (LPVOID*)&runtimeInfo);
    if (hr != S_OK) {
        std::cout << "Error GetRuntime" << std::endl;
    }
    else {
        std::cout << "GetRuntime OK" << std::endl;
    }
   
    hr=runtimeInfo->GetInterface(CLSID_CLRRuntimeHost, IID_ICLRRuntimeHost, (LPVOID*)&runtimeHost);
    if (hr != S_OK) {
        std::cout << "Error GetInterface" << std::endl;
    }
    else {
        std::cout << "GetInterface OK" << std::endl;
    }



    hr = runtimeHost->Start();
    if (hr != S_OK) {
        std::cout << "Error Start" << std::endl;
    }
    else {
        std::cout << "Start OK" << std::endl;
    }


    hr = runtimeHost->ExecuteInDefaultAppDomain(L"C:\\Users\\seb\\source\\repos\\RedTeamNote\\Managed\\bin\\Release\\net8.0\\Managed.dll", L"CLRHello1.Program", L"spotlessMethod", L"test", &pReturnValue);
    if (hr == S_OK)
    {
        std::cout << "CLR executed successfully\n";
    }
    else {
        switch (hr)
        {
        case COR_E_FILENOTFOUND:
            wprintf(L"File not found.\n");
            break;
        case COR_E_BADIMAGEFORMAT:
            wprintf(L"Bad image format.\n");
            break;
        case COR_E_FILELOAD:
            wprintf(L"File load error.\n");
            break;
        default:
            wprintf(L"Unknown error.\n");
            break;
        }
    }


    runtimeInfo->Release();
    metaHost->Release();
    runtimeHost->Release();
    return 0;
}