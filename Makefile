CC := gcc

CPPFLAGS := -Iinclude -MMD -MP
CFLAGS := -Wall -O3


OBJ_DIR = obj


SRC := block.c heap.c memcopy.c malloc.c
OBJ := $(SRC:%.c=$(OBJ_DIR)/%.o)


malloc.so: $(OBJ)
	$(CC) -shared -o $@ $^

test: $(OBJ) $(OBJ_DIR)/tests/main.o
	$(CC) -o $@ $^

clean:
	rm -r $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
