#include "APluginLibrary/libraryloader.h"

#include <utility>

#if defined(__unix__) || defined(__APPLE__)
# include <dlfcn.h>
#elif defined(_WIN32)
# include "dlfcn-win32/dlfcn.h"
#endif

/**
 * @class apl::LibraryLoader
 *
 * @brief Class to load shared libraries and symbols platform independent.
 */

std::string apl::LibraryLoader::errorString = std::string();

/**
 * @return The file extension for shared libraries on this platform
 */
const char* apl::LibraryLoader::libExtension()
{
#ifdef __APPLE__
    return "dylib";
#elif __unix__
    return "so";
#elif _WIN32
    return "dll";
#else
# error "Unknown shared library extension for this platform!"
#endif
}

/**
 * Appends the platform specific file extension for shared libraries to @p path and loads the library.
 *
 * @param path The path to the library without the file extension.
 *
 * @return The handle to the library and an invalid one if the library doesn't exist.
 *
 * @see load(std::string path, const std::string& suffix)
 */
apl::library_handle apl::LibraryLoader::load(std::string path)
{
    return load(std::move(path), libExtension());
}
/**
 * Appends suffix to @p path and loads the library.
 *
 * @param path The path to the library without @p suffix.
 * @param suffix The suffix which should be appended to @p path.
 *
 * @return The handle to the library and a NULL handle if the library doesn't exist.
 */
apl::library_handle apl::LibraryLoader::load(std::string path, const std::string& suffix)
{
#if defined(__unix__) || defined(__APPLE__)
    if(!path.empty() && path.at(0) != '/')
        path.insert(0, "./");
#endif
    if(path.back() != '.')
        path += '.';
    library_handle handle = dlopen((path += suffix).c_str(), RTLD_LAZY);
    char* error = dlerror();
    if(error != nullptr)
        errorString.append(error).append("\n");
    if (!handle)
        return nullptr;
    return handle;
}

/**
 * Loads a symbol from a shared library.
 *
 * @param handle The library handle from which the symbol should be loaded from.
 * @param name The name of the symbol to load.
 *
 * @return A pointer to the loaded symbol.
 */
void* apl::LibraryLoader::getSymbol(library_handle handle, const std::string &name)
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

/**
 * Unloads the library if possible.
 *
 * @param handle The handle to the library to unload.
 *
 * @return True if the library was unloaded successfully, false if not.
 */
bool apl::LibraryLoader::unload(library_handle handle)
{
    if(handle == nullptr)
        return true;
    return !dlclose(handle);
}

/**
 * Get all errors, produced by library loading, unloading and symbol loading. Does not clear the error.
 *
 * @return The error code.
 */
const char* apl::LibraryLoader::getError()
{
    if(errorString.empty())
        return nullptr;
    return errorString.c_str();
}
/**
 * Clears the error string.
 */
void apl::LibraryLoader::clearError()
{
    errorString.clear();
}
