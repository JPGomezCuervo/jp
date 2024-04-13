CFLAGS= -Wall -Wextra -pedantic -ggdb -std=c11 -O0
jp: main.c
	$(CC) $(CFLAGS) -o jp main.c
