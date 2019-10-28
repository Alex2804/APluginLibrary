#ifndef APLUGINLIBRARY_LIBRARYLOADER_H
#define APLUGINLIBRARY_LIBRARYLOADER_H

#include "APluginLibrary/apluginlibrary_export.h"

#include <string>

namespace apl
{
    class APLUGINLIBRARY_EXPORT LibraryLoader
    {
    public:
        static void* load(std::string path);
        static void* load(std::string path, const std::string &suffix);
        static bool unload(void* handle);

        static void* getSymbol(void* handle, const std::string& name);
        template<typename T>
        static inline T getSymbol(void* handle, const std::string& name);

        static const char* getError();
        static void clearError();

    private:
        static std::string errorString;
    };
}

#include "implementation/libraryloader.tpp"

#endif //APLUGINLIBRARY_LIBRARYLOADER_H
