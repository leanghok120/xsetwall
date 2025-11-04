CC = gcc
LIBS = -lX11 -lImlib2
TARGET = xsetwall
SRC = xsetwall.c

all:
	$(CC) -o $(TARGET) $(SRC) $(LIBS)

install:
	mv $(TARGET) /usr/local/bin
