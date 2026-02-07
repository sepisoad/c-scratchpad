# LD_PRELOAD=/opt/gcc/15.2.0/lib64/libasan.so ./out

UNAME_S := $(shell uname -s)

SEPI_LIBS := -I/home/sepi/Projects/sepi/sqv/src/deps
CC := gcc
GCC_WARNS := -Wall -Wextra -Wno-override-init -Wno-unused-local-typedefs
GCC_SAN := -fsanitize=address,undefined,leak -fno-omit-frame-pointer -static-libasan
ifeq ($(UNAME_S), Darwin)
	GCC_SAN := -fsanitize=address,undefined -fno-omit-frame-pointer
	SEPI_LIBS := -I/Users/sepi/Projects/sepi/sqv/src/deps
endif
GCC_FLAGS := -std=c11 -g3 -O0  -DDEBUG $(GCC_WARNS) $(GCC_SAN) $(SEPI_LIBS)
FILC_FLAGS := -std=c11 -g3 -O0  -DDEBUG $(GCC_WARNS) $(SEPI_LIBS)
OUT := out

all: 04

san:
	@$(CC) $(GCC_FLAGS) -o $(OUT) 02.c

filc:
	@/opt/filc/build/bin/filcc $(FILC_FLAGS) -o $(OUT) 02.c

macro:
	@$(CC) $(GCC_FLAGS) -o $(OUT) MACRO.c
	@./out

01:
	@$(CC) $(GCC_FLAGS) -o $(OUT) 01.c
	@./out

02:
	@$(CC) $(GCC_FLAGS) -o $(OUT) 02.c
	@./out

03:
	@$(CC) $(GCC_FLAGS) -o $(OUT) 03.c
	@./out

04:
	@$(CC) $(GCC_FLAGS) -o $(OUT) 04.c
	@./out

exec:
	@./out

