#ifndef APLUGINLIBRARY_ALIBRARYLOADER_H
#define APLUGINLIBRARY_ALIBRARYLOADER_H

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


    };
}

#include "alibraryloader.tpp"

#endif //APLUGINLIBRARY_ALIBRARYLOADER_H
