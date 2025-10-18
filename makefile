TCC_FLAGS := -DDEBUG -Wall -std=std11 $(INCS)
GCC_WARNS := -Wall -Wextra -Wno-override-init -Wno-unused-local-typedefs
GCC_SAN   := -fsanitize=address,undefined,leak -fno-omit-frame-pointer
GCC_FLAGS := -std=gnu11 -g3 -O0  -DDEBUG $(GCC_WARNS) $(GCC_SAN)

all: 02

build:
	@gcc $(GCC_FLAGS) -o out 02.c
00:
	@tcc $(TCC_FLAGS) -b -bt -run 00.c
01:
	@tcc $(TCC_FLAGS) -b -bt -run 01.c
02:
	@tcc $(TCC_FLAGS) -b -bt -run 02.c
03:
	@tcc $(TCC_FLAGS) -b -bt -run 03.c


