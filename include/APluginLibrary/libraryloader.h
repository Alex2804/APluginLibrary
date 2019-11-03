#ifndef APLUGINLIBRARY_LIBRARYLOADER_H
#define APLUGINLIBRARY_LIBRARYLOADER_H

#include "APluginLibrary/apluginlibrary_export.h"

#include <string>

namespace apl
{
    typedef void* library_handle;
    typedef const void* const_library_handle;

    class APLUGINLIBRARY_EXPORT LibraryLoader
    {
    public:
        static const char* libExtension();
        static library_handle load(std::string path);
        static library_handle load(std::string path, const std::string& suffix);
        static bool unload(library_handle handle);

        static void* getSymbol(library_handle handle, const std::string& name);
        template<typename T>
        static inline T getSymbol(library_handle handle, const std::string& name);

        static const char* getError();
        static void clearError();

    private:
        static std::string errorString;
    };
}

#include "implementation/libraryloader.tpp"

#endif //APLUGINLIBRARY_LIBRARYLOADER_H
