#include "../include/APluginLibrary/alibraryloader.h"

#include <iostream>
#include <utility>

#ifdef __unix__
# include <dlfcn.h>
#elif _WIN32
# include "dlfcn-win32/dlfcn.h"
#endif

namespace apl
{

    void *LibraryLoader::load(std::string path)
    {
#ifdef __linux__
        return load(std::move(path), ".so");
#elif __APPLE__
        return load(std::move(path), ".dylib");
#elif _WIN32
        return load(std::move(path), ".dll");
#else
# error "Unsupported platform for library opening!"
#endif
    }
    void *LibraryLoader::load(std::string path, const std::string &suffix)
    {
        void* handle = dlopen((path += suffix).c_str(), RTLD_LAZY);
        if (!handle) {
            std::cerr << dlerror() << std::endl;
            return nullptr;
        }
        dlerror();
        return handle;
    }

    void *LibraryLoader::getSymbol(void *handle, const std::string &name)
    {
        if(!handle)
            return nullptr;
        char *error;
        void *function = dlsym(handle, name.c_str());
        if ((error = dlerror()) != nullptr) {
            std::cerr << error << std::endl;
            return nullptr;
        }
        return function;
    }

    bool LibraryLoader::unload(void *handle)
    {
        if(!handle)
            return true;
        return !dlclose(handle);
    }

}