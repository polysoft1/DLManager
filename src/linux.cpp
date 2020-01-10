#include "DLManager.h"
#include "../include/Exceptions.h"


#include <dlfcn.h>

using namespace Polysoft;

DLManager::DLManager() : handle(nullptr){}

DLManager::DLManager(const std::string& filename){
    open(filename);
}

DLManager::~DLManager(){
    close();
}

void DLManager::open(const std::string& filename){
    if(handle != nullptr){
        close();
    }
    //Clear previous errors
    dlerror();
    handle = dlopen(filename.c_str(), RTLD_LAZY);

    if(handle == nullptr){
        throw OpenLibraryException(dlerror());
    }
}

void DLManager::close(){
    if(handle == nullptr){
        return;
    }

    //Clear previous errors
    dlerror();
    int err = dlclose(handle);

    if(err != 0){
        throw CloseLibraryException(dlerror());
    }
    //Clear the dangling pointer
    handle = nullptr;
}

template<typename T>
void DLManager::getFunction(const std::string &name, T &func_dest){
    getFunction<T>(name.c_str(), func_dest);
}

template<typename T>
void DLManager::getFunction(const char *name, T &func_dest){
    if(handle == nullptr){
        throw NoLibraryOpenException("You need to call open() before calling getFunction()!");
    }

    //Clear previous errors
    dlerror();
    func_dest = reinterpret_cast<T>(dlsym(handle, name));

    if(func_dest == nullptr){
        throw NoSuchFunctionException(dlerror());
    }
}

template<typename T>
T DLManager::getFunction(const std::string &name){
    getFunction<T>(name.c_str());
}

template<typename T>
T DLManager::getFunction(const char *name){
    if(handle == nullptr){
        throw NoLibraryOpenException("You need to call open() before calling getFunction()!");
    }

    //Clear previous errors
    dlerror();
    T result = reinterpret_cast<T>(dlsym(handle, name));

    if(result == nullptr){
        throw NoSuchFunctionException(dlerror());
    }

    return result;
}

//Check for C++17 support
#if __cplusplus >= 201703L 
DLManager::DLManager(const std::filesystem::path& filepath){
    open(filepath);
}

void open(const std::filesystem::path& filepath){
    open(filepath.c_str());
}
#endif