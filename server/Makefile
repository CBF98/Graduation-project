server: server.o wrap.o data_processing.o
	gcc server.o wrap.o data_processing.o -o server

server.o: server.c server.h
	gcc -c server.c

wrap.o: wrap.c wrap.h
	gcc -c wrap.c

data_processing.o: data_processing.c data_processing.h
	gcc -c data_processing.c
