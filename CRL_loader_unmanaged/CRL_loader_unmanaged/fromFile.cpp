#include "sebheaders.h"

int load_from_file() {

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

    res = runtimeHost->ExecuteInDefaultAppDomain(filePath, L"CLRHello1.Program", L"spotlessMethod", L"test", &pReturnValue);
    if (res != S_OK)
        std::cout << "ExecuteInDefaultAppDomain failed with code : " << std::hex << res << std::endl;

    return 0;

}