//try opening dll with ildasm
// https://learn.microsoft.com/en-us/answers/questions/370576/ildasm-exe-wont-work
//  res = runtimeHost->ExecuteInDefaultAppDomain(L"C:\\Users\\sebastien.carre\\source\\repos\\CRL_loader_managed\\CRL_loader_managed\\bin\\Release\\net6.0\\CRL_loader_managed.dll", L"CLRHello1.Program", L"spotlessMethod", L"test", &pReturnValue);
//C:\Users\seb\Desktop\GIT\REDTEAM_CSharp-Loader\ATRIER\CRL_loader_managed\CRL_loader_managed\bin\Release\net6.0\CRL_loader_managed.dll

#include "sebheaders.h"





bool FileExists(LPCWSTR filePath) {
    DWORD attributes = GetFileAttributesW(filePath);
    return (attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY));
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



int main(int argc, wchar_t* argv[])
{
    //return load_from_file();
    //return from_memory();
    return from_memory_v1(argc, argv);

        HRESULT hr;

        ICLRMetaHost* pMetaHost = NULL;
        ICLRRuntimeInfo* pRuntimeInfo = NULL;

        // ICorRuntimeHost and ICLRRuntimeHost are the two CLR hosting interfaces
        // supported by CLR 4.0. Here we demo the ICorRuntimeHost interface that 
        // was provided in .NET v1.x, and is compatible with all .NET Frameworks. 
        ICorRuntimeHost* pCorRuntimeHost = NULL;

        IUnknownPtr spAppDomainThunk = NULL;
        _AppDomainPtr spDefaultAppDomain = NULL;

        // The .NET assembly to load.
        //bstr_t bstrAssemblyName(pszAssemblyName);
        _AssemblyPtr spAssembly = NULL;

        // The .NET class to instantiate.
        bstr_t bstrClassName(L"CLRHello1.Program");
        _TypePtr spType = NULL;
        variant_t vtObject;
        variant_t vtEmpty;

        // The static method in the .NET class to invoke.
        bstr_t bstrStaticMethodName(L"spotlessMethod");
        SAFEARRAY* psaStaticMethodArgs = NULL;
        variant_t vtStringArg(L"test");
        variant_t vtLengthRet;

        // The instance method in the .NET class to invoke.
        bstr_t bstrMethodName(L"test");
        SAFEARRAY* psaMethodArgs = NULL;
        variant_t vtStringRet;

        // 
        // Load and start the .NET runtime.
        // 

        wprintf(L"Load and start the .NET runtime %s \n", L"v4.0.30319");

        hr = CLRCreateInstance(CLSID_CLRMetaHost, IID_PPV_ARGS(&pMetaHost));
        if (FAILED(hr))
        {
            wprintf(L"CLRCreateInstance failed w/hr 0x%08lx\n", hr);
            exit(1);
        }

        // Get the ICLRRuntimeInfo corresponding to a particular CLR version. It 
        // supersedes CorBindToRuntimeEx with STARTUP_LOADER_SAFEMODE.
        hr = pMetaHost->GetRuntime(L"v4.0.30319", IID_PPV_ARGS(&pRuntimeInfo));
        if (FAILED(hr))
        {
            wprintf(L"ICLRMetaHost::GetRuntime failed w/hr 0x%08lx\n", hr);
            exit(1);
        }

        // Check if the specified runtime can be loaded into the process. This 
        // method will take into account other runtimes that may already be 
        // loaded into the process and set pbLoadable to TRUE if this runtime can 
        // be loaded in an in-process side-by-side fashion. 
        BOOL fLoadable;
        hr = pRuntimeInfo->IsLoadable(&fLoadable);
        if (FAILED(hr))
        {
            wprintf(L"ICLRRuntimeInfo::IsLoadable failed w/hr 0x%08lx\n", hr);
            exit(1);
        }

        if (!fLoadable)
        {
            wprintf(L".NET runtime %s cannot be loaded\n", L"v4.0.30319");
            exit(1);
        }

        // Load the CLR into the current process and return a runtime interface 
        // pointer. ICorRuntimeHost and ICLRRuntimeHost are the two CLR hosting  
        // interfaces supported by CLR 4.0. Here we demo the ICorRuntimeHost 
        // interface that was provided in .NET v1.x, and is compatible with all 
        // .NET Frameworks. 
        hr = pRuntimeInfo->GetInterface(CLSID_CorRuntimeHost,
            IID_PPV_ARGS(&pCorRuntimeHost));
        if (FAILED(hr))
        {
            wprintf(L"ICLRRuntimeInfo::GetInterface failed w/hr 0x%08lx\n", hr);
            exit(1);
        }

        // Start the CLR.
        hr = pCorRuntimeHost->Start();
        if (FAILED(hr))
        {
            wprintf(L"CLR failed to start w/hr 0x%08lx\n", hr);
            exit(1);
        }

        // 
        // Load the NET assembly. Call the static method GetStringLength of the 
        // class CSSimpleObject. Instantiate the class CSSimpleObject and call 
        // its instance method ToString.
        // 

        // The following C++ code does the same thing as this C# code:
        // 
        //   Assembly assembly = AppDomain.CurrentDomain.Load(pszAssemblyName);
        //   object length = type.InvokeMember("GetStringLength", 
        //       BindingFlags.InvokeMethod | BindingFlags.Static | 
        //       BindingFlags.Public, null, null, new object[] { "HelloWorld" });
        //   object obj = assembly.CreateInstance("CSClassLibrary.CSSimpleObject");
        //   object str = type.InvokeMember("ToString", 
        //       BindingFlags.InvokeMethod | BindingFlags.Instance | 
        //       BindingFlags.Public, null, obj, new object[] { });

        // Get a pointer to the default AppDomain in the CLR.
        hr = pCorRuntimeHost->GetDefaultDomain(&spAppDomainThunk);
        if (FAILED(hr))
        {
            wprintf(L"ICorRuntimeHost::GetDefaultDomain failed w/hr 0x%08lx\n", hr);
            exit(1);
        }

        hr = spAppDomainThunk->QueryInterface(IID_PPV_ARGS(&spDefaultAppDomain));
        if (FAILED(hr))
        {
            wprintf(L"Failed to get default AppDomain w/hr 0x%08lx\n", hr);
            exit(1);
        }

        // Load the .NET assembly.
        wprintf(L"Load the assembly %s\n", L"MainTest.exe");
        // Load the .NET assembly.
        FILE* pFile;
        long lSize;
        char* buffer;
        size_t result;

        pFile = fopen("C:\\Users\\seb\\Desktop\\tmp\\Program.exe", "rb");
        if (pFile == NULL) { fputs("File error", stderr); exit(1); }

        // obtain file size:
        fseek(pFile, 0, SEEK_END);
        lSize = ftell(pFile);
        rewind(pFile);

        // allocate memory to contain the whole file:
        buffer = (char*)malloc(sizeof(char) * lSize);
        if (buffer == NULL) { fputs("Memory error", stderr); exit(2); }

        // copy the file into the buffer:
        result = fread(buffer, 1, lSize, pFile);
        SAFEARRAYBOUND bounds = { lSize, 0 };

        SAFEARRAY* psa = SafeArrayCreate(VT_UI1, 1, &bounds);
        void* data;
        SafeArrayAccessData(psa, &data);
        CopyMemory(data, buffer, lSize);
        SafeArrayUnaccessData(psa);
        hr = spDefaultAppDomain->raw_Load_3(psa, &spAssembly);
        if (FAILED(hr))
        {
            wprintf(L"Failed to load the assembly w/hr 0x%08lx\n", hr);
            exit(1);
        }

        /* hr = spDefaultAppDomain->Load_2(bstrAssemblyName, &spAssembly);
         if (FAILED(hr))
         {
             wprintf(L"Failed to load the assembly w/hr 0x%08lx\n", hr);
             goto Cleanup;
         }*/

         // Get the Type of CSSimpleObject.
        hr = spAssembly->raw_GetType_2(bstrClassName, &spType);
        if (FAILED(hr))
        {
            wprintf(L"Failed to get the Type interface w/hr 0x%08lx\n", hr);
            exit(1);
        }

        // Call the static method of the class: 
        //   public static int GetStringLength(string str);

        // Create a safe array to contain the arguments of the method. The safe 
        // array must be created with vt = VT_VARIANT because .NET reflection 
        // expects an array of Object - VT_VARIANT. There is only one argument, 
        // so cElements = 1.
        psaStaticMethodArgs = SafeArrayCreateVector(VT_VARIANT, 0, 1);
        LONG index = 0;
        hr = SafeArrayPutElement(psaStaticMethodArgs, &index, &vtStringArg);
        if (FAILED(hr))
        {
            wprintf(L"SafeArrayPutElement failed w/hr 0x%08lx\n", hr);
            exit(1);
        }

        // Invoke the "GetStringLength" method from the Type interface.
        hr = spType->raw_InvokeMember_3(bstrStaticMethodName, static_cast<BindingFlags>(
            BindingFlags_InvokeMethod | BindingFlags_Static | BindingFlags_Public),
            NULL, vtEmpty, psaStaticMethodArgs, &vtLengthRet);
        if (FAILED(hr))
        {
            wprintf(L"Failed to invoke GetStringLength w/hr 0x%08lx\n", hr);
            exit(1);
        }

        // Print the call result of the static method.
        wprintf(L"Call %s.%s(\"%s\") => %d\n",
            static_cast<PCWSTR>(bstrClassName),
            static_cast<PCWSTR>(bstrStaticMethodName),
            static_cast<PCWSTR>(vtStringArg.bstrVal),
            vtLengthRet.lVal);

        // Instantiate the class.
        hr = spAssembly->raw_CreateInstance(bstrClassName, &vtObject);
        if (FAILED(hr))
        {
            wprintf(L"Assembly::CreateInstance failed w/hr 0x%08lx\n", hr);
            exit(1);
        }

        // Call the instance method of the class.
        //   public string ToString();

        // Create a safe array to contain the arguments of the method.
        psaMethodArgs = SafeArrayCreateVector(VT_VARIANT, 0, 0);

        // Invoke the "ToString" method from the Type interface.
        hr = spType->raw_InvokeMember_3(bstrMethodName, static_cast<BindingFlags>(
            BindingFlags_InvokeMethod | BindingFlags_Instance | BindingFlags_Public),
            NULL, vtObject, psaMethodArgs, &vtStringRet);
        if (FAILED(hr))
        {
            wprintf(L"Failed to invoke ToString w/hr 0x%08lx\n", hr);
            exit(1);
        }

        // Print the call result of the method.
        wprintf(L"Call %s.%s() => %s\n",
            static_cast<PCWSTR>(bstrClassName),
            static_cast<PCWSTR>(bstrMethodName),
            static_cast<PCWSTR>(vtStringRet.bstrVal));

    
    
}
