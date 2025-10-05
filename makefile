all: 02

FLAGS := -DDEBUG -Wall -Wunsupported -Wimplicit-function-declaration -Wdiscarded-qualifiers -Wwrite-strings

build:
	@gcc -std=c11 -g3 -O0 -fno-inline -fno-omit-frame-pointer -Wall -Wextra -DDEBUG -o out 02.c
00:
	@tcc $(FLAGS) -std=c11 -b -bt -run 00.c 
01:
	@tcc $(FLAGS) -std=c11 -b -bt -run 01.c 
02:
	@tcc $(FLAGS) -std=c11 -b -bt -run 02.c 
