all:
	gcc -o xsetwall xsetwall.c -lX11 -lImlib2

install:
	mv xsetwall /usr/local/bin
