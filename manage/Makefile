manage: manage.o socket_tcp.o wrap.o socket_udp.o data_processing.o pcap_send.o
	gcc manage.o socket_tcp.o wrap.o socket_udp.o data_processing.o pcap_send.o -o manage -lpthread -lpcap

manage.o: manage.c manage.h data_processing.h socket_tcp.h wrap.h socket_udp.h pcap_send.h
	gcc -c manage.c

socket_tcp.o: socket_tcp.c socket_tcp.h
	gcc -c socket_tcp.c

wrap.o: wrap.c wrap.h
	gcc -c wrap.c

socket_udp.o: socket_udp.c socket_udp.h
	gcc -c socket_udp.c

data_processing.o: data_processing.c data_processing.h manage.h
	gcc -c data_processing.c

pcap_send.o: pcap_send.c pcap_send.h
	gcc -c pcap_send.c -lpcap
