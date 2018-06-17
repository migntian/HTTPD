.PHONY:clean
httpd:httpd.c
	gcc -o $@ $^ -lpthread
clean:
	rm httpd
