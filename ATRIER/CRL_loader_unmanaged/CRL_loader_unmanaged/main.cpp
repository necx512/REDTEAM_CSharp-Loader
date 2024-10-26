// code mostly stolen from pabloko's comment in https://gist.github.com/xpn/e95a62c6afcf06ede52568fcd8187cc2
#include <iostream>
#include <metahost.h>
#include <corerror.h>
#pragma comment(lib, "mscoree.lib")

int main()
{
    ICLRMetaHost* metaHost = NULL;
    ICLRRuntimeInfo* runtimeInfo = NULL;
    ICLRRuntimeHost* runtimeHost = NULL;
    DWORD pReturnValue;

    HRESULT res = CLRCreateInstance(CLSID_CLRMetaHost, IID_ICLRMetaHost, (LPVOID*)&metaHost);
    if (res == S_OK)
    {
        std::cout << "CLRCreateInstance......................................OK" << std::endl;
    }
    else
    {
        std::cout << "Failed CLRCreateInstance" << std::endl;
    }





    res = metaHost->GetRuntime(L"v4.0.30319", IID_ICLRRuntimeInfo, (LPVOID*)&runtimeInfo);
    if (res == S_OK)
    {
        std::cout << "GetRuntime......................................OK" << std::endl;
    }
    else
    {
        std::cout << "Failed GetRuntime" << std::endl;
    }









    res = runtimeInfo->GetInterface(CLSID_CLRRuntimeHost, IID_ICLRRuntimeHost, (LPVOID*)&runtimeHost);
    if (res == S_OK)
    {
        std::cout << "GetInterface......................................OK" << std::endl;
    }
    else
    {
        std::cout << "Failed GetInterface" << std::endl;
    }


    res = runtimeHost->Start();
    if (res == S_OK)
    {
        std::cout << "Start......................................OK" << std::endl;
    }

    else
    {
        std::cout << "Failed Start" << std::endl;
    }
    


    //try opening dll with ildasm
    // https://learn.microsoft.com/en-us/answers/questions/370576/ildasm-exe-wont-work
    //  res = runtimeHost->ExecuteInDefaultAppDomain(L"C:\\Users\\sebastien.carre\\source\\repos\\CRL_loader_managed\\CRL_loader_managed\\bin\\Release\\net6.0\\CRL_loader_managed.dll", L"CLRHello1.Program", L"spotlessMethod", L"test", &pReturnValue);

    res = runtimeHost->ExecuteInDefaultAppDomain(L"C:\\Users\\sebastien.carre\\source\\repos\\CRL_loader_managed\\CRL_loader_managed\\bin\\Release\\net6.0\\publish\\CRL_loader_managed.dll", L"CLRHello1.Program", L"spotlessMethod", L"test", &pReturnValue);
    if (res == S_OK)
    {
        std::cout << "ExecuteInDefaultAppDomain......................................OK" << std::endl;
    }
    else if (res == HOST_E_CLRNOTAVAILABLE)
    {
        std::cout << "Failed ExecuteInDefaultAppDomain : HOST_E_CLRNOTAVAILABLE" << std::endl;
    }
    else if (res == HOST_E_TIMEOUT)
    {
        std::cout << "Failed ExecuteInDefaultAppDomain : HOST_E_TIMEOUT " << std::endl;
    }
    else if (res == HOST_E_NOT_OWNER)
    {
        std::cout << "Failed ExecuteInDefaultAppDomain : HOST_E_NOT_OWNER" << std::endl;
    }
    else if (res == HOST_E_ABANDONED)
    {
        std::cout << "Failed ExecuteInDefaultAppDomain : HOST_E_ABANDONED" << std::endl;
    }
    else if (res == E_FAIL)
    {
        std::cout << "Failed ExecuteInDefaultAppDomain : E_FAIL" << std::endl;
    }
    else
    {
        std::cout << "Failed ExecuteInDefaultAppDomain : OTHER" << std::endl;
        printf("%llx", res);
    }

    runtimeInfo->Release();
    metaHost->Release();
    runtimeHost->Release();
    return 0;
}