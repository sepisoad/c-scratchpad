# LD_PRELOAD=/opt/gcc/15.2.0/lib64/libasan.so ./out


SEPI_LIBS := -I. -I/home/sepi/Projects/sepi/sqv/src/deps
CC := gcc
GCC_WARNS := -Wall -Wextra -Wno-override-init -Wno-unused-local-typedefs
GCC_SAN   := -fsanitize=address,undefined,leak -fno-omit-frame-pointer -static-libasan
GCC_FLAGS := -std=gnu11 -g3 -O0  -DDEBUG $(GCC_WARNS) $(GCC_SAN) $(SEPI_LIBS)
FILC_FLAGS := -std=gnu11 -g3 -O0  -DDEBUG $(GCC_WARNS) $(SEPI_LIBS)
OUT := out

all: 08

san:
	@$(CC) $(GCC_FLAGS) -o $(OUT) 02.c

filc:
	@/opt/filc/build/bin/filcc $(FILC_FLAGS) -o $(OUT) 02.c

01:
	echo "nothing"

02:
	echo "nothing"

03:
	lua 03.lua

04:
	@$(CC) $(GCC_FLAGS) -o $(OUT) 04.c
	./out

05:
	@$(CC) $(GCC_FLAGS) -o $(OUT) 05.c
	./out

06:
	@$(CC) $(GCC_FLAGS) -o $(OUT) 06.c
	./out

07:
	@$(CC) $(GCC_FLAGS) -o $(OUT) 07.c
	./out

08:
	@$(CC) $(GCC_FLAGS) -o $(OUT) 08.c
	./out

exec:
	@./out

