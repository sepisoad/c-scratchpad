FLAGS := -DDEBUG -Wall -std=c11

all: 02

build:
	@gcc -std=c11 -g3 -O0 -Wall -Wextra -DDEBUG -o out 02.c
00:
	@tcc $(FLAGS) -b -bt -run 00.c
01:
	@tcc $(FLAGS) -b -bt -run 01.c
02:
	@tcc $(FLAGS) -b -bt -run 02.c
