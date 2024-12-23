CC=x86_64-w64-mingw32-gcc
CCRES=x86_64-w64-mingw32-windres
CFLAGS=-Wall -Wextra -std=c11 -pedantic -ggdb
LIBS=-lurlmon

minefree.exe: main.obj res.obj
	$(CC) $(CFLAGS) -o minefree.exe main.obj res.obj $(LIBS)

main.obj: main.c
	$(CC) $(CFLAGS) -c main.c -o main.obj $(LIBS)

res.obj: minefree.rc
	$(CCRES) minefree.rc res.obj

minefree.rc: minefree.exe.manifest
