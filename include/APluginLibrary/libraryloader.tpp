#ifndef APLUGINLIBRARY_LIBRARYLOADER_TPP
#define APLUGINLIBRARY_LIBRARYLOADER_TPP

#include "libraryloader.h"

template<typename T>
inline T apl::LibraryLoader::getSymbol(void* handle, const std::string &name)
{
    return reinterpret_cast<T>(getSymbol(handle, name));
}

#endif //APLUGINLIBRARY_LIBRARYLOADER_TPP
