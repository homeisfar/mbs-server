# Server makefile
FILES = server.c \
				util.c

CC = gcc -Wall
DD = gcc -Wall -g -D DEBUG
TARGET = mbs-server

all:
	$(CC) -o $(TARGET) $(FILES)

clean:
	rm *.o $(TARGET)

debug:
	$(DD) -o $(TARGET) $(FILES)

# Missing:
# LIBS = -lssl. Worry about this when I reach SSL
