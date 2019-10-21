#ifndef APLUGINLIBRARY_LIBRARYLOADER_H
#define APLUGINLIBRARY_LIBRARYLOADER_H

#include <string>

namespace apl
{
    class LibraryLoader
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

#include "libraryloader.tpp"

#endif //APLUGINLIBRARY_LIBRARYLOADER_H
