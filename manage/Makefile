manage: manage.o socket_tcp.o wrap.o
	gcc manage.o socket_tcp.o wrap.o -o manage

manage.o: manage.c manage.h
	gcc -c manage.c

socket_tcp.o: socket_tcp.c socket_tcp.h
	gcc -c socket_tcp.c

wrap.o: wrap.c wrap.h
	gcc -c wrap.c
