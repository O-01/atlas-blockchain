CC := gcc
CFLAGS := -Wall -Werror -Wextra -pedantic -Wno-deprecated-declarations
CPPFLAGS := -I. -Itransaction/ -I../../crypto
LDFLAGS := -L../../crypto
LDLIBS := -lhblk_crypto -lllist -lssl -lcrypto

OBJ_DIR := ./

SRC := $(wildcard ./*.c) $(wildcard transaction/*.c)
OBJ := $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

VPATH := $(sort $(dir $(SRC)))

all: libhblk_blockchain.a clean

$(OBJ_DIR)/%.o: %.c
	@$(CC) $(CFLAGS) -c $(CPPFLAGS) $(LDFLAGS) $(LDLIBS) $< -o $@

libhblk_blockchain.a: $(OBJ)
	@ar rcs $@ $^

clean:
	@$(RM) -rf *.o

.PHONY: clean
