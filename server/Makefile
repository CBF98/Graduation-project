server: server.o wrap.o
	gcc server.o wrap.o -o server

server.o: server.c
	gcc -c server.c

wrap.o: wrap.c wrap.h
	gcc -c wrap.c
