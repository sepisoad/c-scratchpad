UNAME_S := $(shell uname -s)

CC := clang
GCC_WARNS := -Wall -Wextra -Wno-override-init -Wno-unused-local-typedefs
GCC_FLAGS := -std=c11 -g -O0  -DDEBUG $(GCC_WARNS) $(GCC_SAN) -pthread
OUT := out

ifeq ($(UNAME_S), Darwin)
    GCC_SAN :=
    SEPI_LIBS := -I/Users/sepi/Projects/sepi/sqv/src/deps
else
    GCC_SAN := -fsanitize=address,undefined,leak -fno-omit-frame-pointer
		SEPI_LIBS := -I/home/sepi/Projects/sepi/sqv/src/deps
endif


all: 18

01:
	@$(CC) $(GCC_FLAGS) $(SEPI_LIBS) -o $(OUT) 01.c
	@./out

02:
	@$(CC) $(GCC_FLAGS) $(SEPI_LIBS) -o $(OUT) 02.c
	@./out

03:
	@$(CC) $(GCC_FLAGS) $(SEPI_LIBS) -o $(OUT) 03.c
	@./out

04:
	@$(CC) $(GCC_FLAGS) $(SEPI_LIBS) -o $(OUT) 04.c
	@./out

05:
	@$(CC) $(GCC_FLAGS) $(SEPI_LIBS) -o $(OUT) 05.c
	@./out

06:
	@$(CC) $(GCC_FLAGS) $(SEPI_LIBS) -o $(OUT) 06.c
	@./out

07:
	@$(CC) $(GCC_FLAGS) $(SEPI_LIBS) -o $(OUT) 07.c
	@./out

08:
	@$(CC) $(GCC_FLAGS) $(SEPI_LIBS) -o $(OUT) 08.c
	@./out

09:
	@$(CC) $(GCC_FLAGS) $(SEPI_LIBS) -o $(OUT) 09.c
	@./out

10:
	@$(CC) $(GCC_FLAGS) $(SEPI_LIBS) -o $(OUT) 10.c
	@./out

11:
	@$(CC) $(GCC_FLAGS) $(SEPI_LIBS) -o $(OUT) 11.c
	@./out

12:
	@$(CC) $(GCC_FLAGS) $(SEPI_LIBS) -o $(OUT) 12.c
	@./out

13:
	@$(CC) $(GCC_FLAGS) $(SEPI_LIBS) -o $(OUT) 13.c
	@./out

14:
	@$(CC) $(GCC_FLAGS) $(SEPI_LIBS) -o $(OUT) 14.c
	@./out

15:
	@$(CC) $(GCC_FLAGS) $(SEPI_LIBS) -o $(OUT) 15.c
	@./out

16:
	@$(CC) $(GCC_FLAGS) $(SEPI_LIBS) -o $(OUT) 16.c
	@./out

17:
	@$(CC) $(GCC_FLAGS) $(SEPI_LIBS) -o $(OUT) 17.c
	@./out

18:
	@$(CC) $(GCC_FLAGS) $(SEPI_LIBS) -o $(OUT) 18.c
	@./out
