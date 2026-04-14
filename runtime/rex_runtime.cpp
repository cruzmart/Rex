#include <cstring>
#include <cstdint>

extern "C" {

// returns 1 if equal, 0 otherwise
int rex_string_eq(const char* a, const char* b) {
    if (a == nullptr || b == nullptr)
        return 0;

    return std::strcmp(a, b) == 0;
}

}