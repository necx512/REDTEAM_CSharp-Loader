#include <metahost.h>
#pragma comment(lib, "mscoree.lib")
int main()
{
    void InitializeCLR() {
        ICLRMetaHost* pMetaHost = nullptr;
        ICLRRuntimeInfo* pRuntimeInfo = nullptr;
        ICorRuntimeHost* pRuntimeHost = nullptr;

        HRESULT hr = CLRCreateInstance(CLSID_CLRMetaHost, IID_PPV_ARGS(&pMetaHost));
        if (FAILED(hr)) {
            std::cerr << "CLRCreateInstance failed" << std::endl;
            return;
        }

        hr = pMetaHost->GetRuntime(L"v4.0.30319", IID_PPV_ARGS(&pRuntimeInfo));
        if (FAILED(hr)) {
            std::cerr << "GetRuntime failed" << std::endl;
            pMetaHost->Release();
            return;
        }

        BOOL bLoadable;
        hr = pRuntimeInfo->IsLoadable(&bLoadable);
        if (FAILED(hr) || !bLoadable) {
            std::cerr << "CLR not loadable" << std::endl;
            pRuntimeInfo->Release();
            pMetaHost->Release();
            return;
        }

        hr = pRuntimeInfo->GetInterface(CLSID_CorRuntimeHost, IID_PPV_ARGS(&pRuntimeHost));
        if (FAILED(hr)) {
            std::cerr << "GetInterface failed" << std::endl;
            pRuntimeInfo->Release();
            pMetaHost->Release();
            return;
        }

        hr = pRuntimeHost->Start();
        if (FAILED(hr)) {
            std::cerr << "CLR start failed" << std::endl;
            pRuntimeHost->Release();
            pRuntimeInfo->Release();
            pMetaHost->Release();
            return;
        }

        std::cout << "CLR initialized successfully" << std::endl;

        // Release resources
        pRuntimeHost->Release();
        pRuntimeInfo->Release();
        pMetaHost->Release();
    }
}