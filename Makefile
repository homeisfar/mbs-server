# Server makefile
FILES = server.c \
				util.c

CC = gcc -Wall -g
TARGET = mbs-server

all:
	$(CC) -o $(TARGET) $(FILES)

clean:
	rm *.o $(TARGET)

# Missing:
# LIBS = -lssl. Worry about this when I reach SSL
