#pragma once

#include <iostream>
#include <windows.h>
#include "MSCorEE.h"
#include <metahost.h>
#include <corerror.h>

#pragma comment(lib, "mscoree.lib")
#pragma warning( disable:4996 )


#import "mscorlib.tlb" auto_rename
using namespace mscorlib;

int load_from_file();
bool FileExists(LPCWSTR filePath);
int from_memory_v1(int argc, wchar_t* argv[]);
