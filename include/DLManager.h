#ifndef __DL_MANAGER_H__
#define __DL_MANAGER_H__

//Check for C++17 support
#if __cplusplus >= 201703L
#include <filesystem>
#endif

#include <string>
#include "Exceptions.h"

#ifdef __linux__
#include "DLManager_linux.h"
#elif _WIN32
//TODO Windows header
#endif

#endif