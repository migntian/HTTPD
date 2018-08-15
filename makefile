CC=gcc 
.PHONY:clean

all:client server

client:client.c
	$(CC) -o $@ $^

server:echo_www.o get_line.o httpd.o listensock.o
	$(CC) -o $@ $^ -lpthread

ehco_www.o:echo_www.c echo_www.h
	$(CC) -c echo_www.c
get_line.o:get_line.c get_line.h
	$(CC) -c get_line.c
httpd.o:httpd.c http.h
	$(CC) -c httpd.c
listensock.o:listensock.c listensock.h
	$(CC) -c listensock.c

clean:
	rm -f *.o client server
