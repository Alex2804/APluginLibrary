#include <cstring>

#ifdef _WIN32
# define A_API  __declspec( dllexport )
#else
# define A_API
#endif

extern "C" {
	A_API double add(double x, double y) {
        return x + y;
    }
	A_API double sub(double x, double y) {
        return x - y;
    }

	A_API char* setToChar(char* str, char c, int s) {
        std::memset(str, c, s);
        return str;
    }
}