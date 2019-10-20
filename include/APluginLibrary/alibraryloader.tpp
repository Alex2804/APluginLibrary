#ifndef APLUGINLIBRARY_ALIBRARYLOADER_TPP
#define APLUGINLIBRARY_ALIBRARYLOADER_TPP

namespace apl
{
    template<typename T>
    inline T ALibraryLoader::getSymbol(void* handle, const std::string &name)
    {
        return reinterpret_cast<T>(getSymbol(handle, name));
    }
}


#endif //APLUGINLIBRARY_ALIBRARYLOADER_TPP
