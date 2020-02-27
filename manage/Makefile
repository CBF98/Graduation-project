manage: manage.o socket_tcp.o wrap.o socket_udp.o
	gcc manage.o socket_tcp.o wrap.o socket_udp.o -o manage -lpthread

manage.o: manage.c manage.h
	gcc -c manage.c

socket_tcp.o: socket_tcp.c socket_tcp.h
	gcc -c socket_tcp.c

wrap.o: wrap.c wrap.h
	gcc -c wrap.c

socket_udp.o: socket_udp.c socket_udp.h
	gcc -c socket_udp.c
