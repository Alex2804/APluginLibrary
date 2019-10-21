#include "APluginLibrary/libraryloader.h"

#include <utility>

#if defined(__unix__) || defined(__APPLE__)
# include <dlfcn.h>
#elif defined(_WIN32)
# include "dlfcn-win32/dlfcn.h"
#endif

std::string apl::LibraryLoader::errorString = std::string();

void* apl::LibraryLoader::load(std::string path)
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
void* apl::LibraryLoader::load(std::string path, const std::string &suffix)
{
    void* handle = dlopen((std::move(path) += suffix).c_str(), RTLD_LAZY);
    char* error = dlerror();
    if(error != nullptr)
        errorString.append(error).append("\n");
    if (!handle)
        return nullptr;
    return handle;
}
void* apl::LibraryLoader::getSymbol(void *handle, const std::string &name)
{
    if(!handle)
        return nullptr;
    char* error;
    void* function = dlsym(handle, name.c_str());
    if ((error = dlerror()) != nullptr) {
        errorString.append(error).append("\n");
        return nullptr;
    }
    return function;
}
bool apl::LibraryLoader::unload(void *handle)
{
    if(!handle)
        return true;
    return !dlclose(handle);
}

const char* apl::LibraryLoader::getError()
{
    if(errorString.empty())
        return nullptr;
    return errorString.c_str();
}
void apl::LibraryLoader::clearError()
{
    errorString.clear();
}