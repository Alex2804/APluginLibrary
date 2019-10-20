#include <cstdlib>

#ifdef _WIN32
# define A_API  __declspec( dllexport )
#else
# define A_API
#endif

extern "C" {
    A_API void* allocate(size_t size) {
        return std::malloc(size);
    }

    A_API void release(void* ptr) {
        std::free(ptr);
    }
}