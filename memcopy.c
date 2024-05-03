#include "memcopy.h"
#include <stdint.h>

void memcopy(void* from, void* to, size_t size) {
    for (size_t i=0; i < size; i++) {
        ((uint8_t*) to)[i] = ((uint8_t*) from)[i];
    }
}