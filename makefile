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

all: 11

san:
	@$(CC) $(GCC_FLAGS) -o $(OUT) 02.c

filc:
	@/opt/filc/build/bin/filcc $(FILC_FLAGS) -o $(OUT) 02.c

01:
	@echo "nothing"

02:
	@echo "nothing"

03:
	@lua 03.lua

04:
	@$(CC) $(GCC_FLAGS) -o $(OUT) 04.c
	@./out

05:
	@$(CC) $(GCC_FLAGS) -o $(OUT) 05.c
	@./out

06:
	@$(CC) $(GCC_FLAGS) -o $(OUT) 06.c
	@./out

07:
	@$(CC) $(GCC_FLAGS) -o $(OUT) 07.c
	@./out

08:
	@$(CC) $(GCC_FLAGS) -o $(OUT) 08.c
	@./out

09:
	@$(CC) $(GCC_FLAGS) -o $(OUT) 09.c
	@./out

10:
	@$(CC) $(GCC_FLAGS) -o $(OUT) 10.c
	@./out

11:
	@$(CC) $(GCC_FLAGS) -o $(OUT) 11.c
	@./out

12:
	@$(CC) $(GCC_FLAGS) -o $(OUT) 12.c
	@./out

exec:
	@./out

