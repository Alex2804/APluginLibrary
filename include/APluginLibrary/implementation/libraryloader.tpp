#ifndef APLUGINLIBRARY_LIBRARYLOADER_TPP
#define APLUGINLIBRARY_LIBRARYLOADER_TPP

template<typename T>
inline T apl::LibraryLoader::getSymbol(library_handle handle, const std::string &name)
{
    return reinterpret_cast<T>(getSymbol(handle, name)); // non template function get chosen
}

#endif //APLUGINLIBRARY_LIBRARYLOADER_TPP
