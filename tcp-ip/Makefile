all:
	gcc -o send tcp_send.c
	gcc -o recv tcp_recv.c
	gcc -o usend udp_send.c
	gcc -o urecv udp_recv.c
t: tcp_send.c tcp_recv.c
	gcc -o send tcp_send.c
	gcc -o recv tcp_recv.c
send: tcp_send.c
	gcc -o send tcp_send.c
	gcc -o usend udp_send.c
recv: tcp_recv.c
	gcc -o recv tcp_recv.c
	gcc -o urecv udp_recv.c
u: udp_send.c udp_recv.c
	gcc -o usend udp_send.c
	gcc -o urecv udp_recv.c
clean:
	rm send
	rm recv
	rm usend
	rm urecv
