#include <stdio.h>
#include <string.h>
#include "malloc.h"


int main() {
    char* buffer = (char*) malloc(sizeof(char) * 27);
    if (buffer == NULL) {
        perror("error while allocating space");
        return -1;
    }

    char* str = "abcdefghijklmnopqrstuvwxyz";
    strcpy(buffer, str);
    char* new_buffer = realloc(buffer, 26 * 2 + 1);
    if (new_buffer == NULL) {
        perror("e");
        return -1;
    }
    printf("%p %p\n", buffer, new_buffer);
    printf("%s\n", new_buffer);
    strcpy(new_buffer, str);
    strcpy(&new_buffer[26], str);
    printf("%s\n", new_buffer);
    free(buffer);
}