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
	std::cout << std::hex << hr << std::endl; 
		
	
	return 0;
}
