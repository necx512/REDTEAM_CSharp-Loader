// code mostly stolen from pabloko's comment in https://gist.github.com/xpn/e95a62c6afcf06ede52568fcd8187cc2
//try opening dll with ildasm
// https://learn.microsoft.com/en-us/answers/questions/370576/ildasm-exe-wont-work
//  res = runtimeHost->ExecuteInDefaultAppDomain(L"C:\\Users\\sebastien.carre\\source\\repos\\CRL_loader_managed\\CRL_loader_managed\\bin\\Release\\net6.0\\CRL_loader_managed.dll", L"CLRHello1.Program", L"spotlessMethod", L"test", &pReturnValue);
//C:\Users\seb\Desktop\GIT\REDTEAM_CSharp-Loader\ATRIER\CRL_loader_managed\CRL_loader_managed\bin\Release\net6.0\CRL_loader_managed.dll


#include <iostream>
#include <windows.h>
#include "MSCorEE.h"
#include <metahost.h>
#include <corerror.h>

#pragma comment(lib, "mscoree.lib")
#pragma warning( disable:4996 )


#import "mscorlib.tlb" auto_rename
using namespace mscorlib;




bool FileExists(LPCWSTR filePath) {
    DWORD attributes = GetFileAttributesW(filePath);
    return (attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY));
}


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


bool readBinFile(const char fileName[], char*& bufPtr, DWORD& length) {
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
int from_memory()
{
    
    PCHAR ptrBinary; DWORD lenBinary;
    if (!readBinFile("C:/Users/seb/Desktop/tmp/Program.exe", ptrBinary, lenBinary))
        return -1;

    LPBYTE rawAssemblyByteArray = LPBYTE(ptrBinary);
    ULONG lenRawData = lenBinary;


    ICLRMetaHost* metaHost = NULL;
    CLRCreateInstance(CLSID_CLRMetaHost, IID_ICLRMetaHost, (LPVOID*)&metaHost);
    ICLRRuntimeInfo* runtimeInfo = NULL;
    metaHost->GetRuntime(L"v4.0.30319", IID_ICLRRuntimeInfo, (LPVOID*)&runtimeInfo);



    ICorRuntimeHost* pCorRuntimeHost = NULL;
    runtimeInfo->GetInterface(CLSID_CorRuntimeHost, IID_ICorRuntimeHost, (LPVOID*)&pCorRuntimeHost);

    IUnknown* appDomainThunk;
    pCorRuntimeHost->GetDefaultDomain(&appDomainThunk);
    _AppDomain* defaultAppDomain = NULL;
    appDomainThunk->QueryInterface(&defaultAppDomain);

    SAFEARRAYBOUND bounds[1];
    bounds[0].cElements = sizeof(rawAssemblyByteArray);
    bounds[0].lLbound = 0;
    SAFEARRAY* safeArray = SafeArrayCreate(VT_UI1, 1, bounds);
    SafeArrayLock(safeArray);
    memcpy(safeArray->pvData, rawAssemblyByteArray, sizeof(rawAssemblyByteArray));
    SafeArrayUnlock(safeArray);
    _AssemblyPtr managedAssembly = NULL;

    managedAssembly = defaultAppDomain->Load_3(safeArray); //pas Load_3

    _TypePtr managedType = NULL;
    _bstr_t managedClassName("CLRHello1.Program");
    managedType = managedAssembly->GetType_2(managedClassName);

    
    SAFEARRAY* managedArguments = SafeArrayCreateVector(VT_VARIANT, 0, 1);
    _variant_t argument(L"test");
    LONG index = 0;
    SafeArrayPutElement(managedArguments, &index, &argument);

    _bstr_t managedMethodName(L"spotlessMethod");
    _variant_t managedReturnValue;
    _variant_t empty;
    
    managedReturnValue = managedType->InvokeMember_3(managedMethodName, static_cast<BindingFlags>(BindingFlags_InvokeMethod | BindingFlags_Static | BindingFlags_Public), NULL, empty, managedArguments);


    return 0;



}


int main()
{
    return load_from_file();
}
