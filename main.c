#include <stdio.h>
#include "malloc.h"

int main() {
	char* cool_string = (char*) malloc(27);
	if (cool_string == NULL) {
		perror("error while allocating");
		return -1;
	}
	
	cool_string[0] = 'a';
	cool_string[12] = 'b';
	cool_string[26] = '\0';

	printf("%s\n", cool_string);
	free(cool_string);
}
