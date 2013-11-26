all:
	gcc -o send.exe tcp_send.c
	gcc -o recv.exe tcp_recv.c
	gcc -o usend.exe udp_send.c
	gcc -o urecv.exe udp_recv.c
t: tcp_send.c tcp_recv.c
	gcc -o send.exe tcp_send.c
	gcc -o recv.exe tcp_recv.c
send: tcp_send.c
	gcc -o send.exe tcp_send.c
	gcc -o usend.exe udp_send.c
recv: tcp_recv.c
	gcc -o recv.exe tcp_recv.c
	gcc -o urecv.exe udp_recv.c
u: udp_send.c udp_recv.c
	gcc -o usend.exe udp_send.c
	gcc -o urecv.exe udp_recv.c
clean:
	rm send.exe
	rm recv.exe
	rm usend.exe
	rm urecv.exe
