CC := gcc
CFLAGS := -Werror -Wall -Wextra -pedantic -Wno-deprecated-declarations
INC := -I.

SRCDIR := .

SRC := $(wildcard $(SRCDIR)/*.c)
OBJ := $(SRC:%.c=%.o)

all: libhblk_crypto.a clean

%.o : %.c
	@$(CC) $(CFLAGS) -c $(INC) $< -o $@

libhblk_crypto.a: $(OBJ)
	@ar rcs $@ $^

clean:
	@$(RM) -rf *.o

.PHONY: clean
