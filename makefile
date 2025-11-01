# LD_PRELOAD=/opt/gcc/15.2.0/lib64/libasan.so ./out

CC := gcc
GCC_WARNS := -Wall -Wextra -Wno-override-init -Wno-unused-local-typedefs
GCC_SAN   := -fsanitize=address,undefined,leak -fno-omit-frame-pointer -static-libasan
GCC_FLAGS := -std=gnu11 -g3 -O0  -DDEBUG $(GCC_WARNS) $(GCC_SAN)
FILC_FLAGS := -std=gnu11 -g3 -O0  -DDEBUG $(GCC_WARNS)

all: filc exec

san:
	@$(CC) $(GCC_FLAGS) -o out 04.c

filc:
	@/opt/filc/build/bin/filcc $(FILC_FLAGS) -o out 04.c


exec:
	./out

