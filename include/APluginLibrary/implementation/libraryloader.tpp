#ifndef APLUGINLIBRARY_LIBRARYLOADER_TPP
#define APLUGINLIBRARY_LIBRARYLOADER_TPP

/**
 * Loads a symbol from a shared library and casts it to the template type.
 *
 * @tparam T The type to cast to.
 *
 * @param handle The library handle from which the symbol should be loaded from.
 * @param name The name of the symbol to load.
 *
 * @return A pointer to the casted, loaded symbol.
 *
 * @see getSymbol(library_handle handle, const std::string& name)
 */
template<typename T>
inline T apl::LibraryLoader::getSymbol(library_handle handle, const std::string& name)
{
    return reinterpret_cast<T>(getSymbol(handle, name)); // non template function get chosen
}

#endif //APLUGINLIBRARY_LIBRARYLOADER_TPP
