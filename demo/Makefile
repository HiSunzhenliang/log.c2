# GNU Makefile

CC     		:= gcc
CFLAGS 		:= -g -Wall
LDFLAGS 	:=
INCLUDE 	:= -I..
SRC			:= $(wildcard *.c) log.c
OBJS  		:= $(SRC:%.c=%.o)

TARGET		:= demo

.PHONY: all clean

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE) -o $@ $^

log.o: ../log.c ../log.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE) -c $<

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE) -c $<

clean:
	rm -f $(TARGET) $(OBJS)
