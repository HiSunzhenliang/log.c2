# GNU Makefile

CC     		:= gcc
CFLAGS 		:= -g -Wall
DEFINES		:= -DLOG_USE_COLOR
LDFLAGS 	:=
INCLUDE 	:= -I..
SRC			:= $(wildcard *.c) log.c
OBJS  		:= $(SRC:%.c=%.o)

TARGET		:= demo

.PHONY: all clean

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(DEFINES) $(LDFLAGS) $(INCLUDE) -o $@ $^

log.o: ../log.c ../log.h
	$(CC) $(CFLAGS) $(DEFINES) $(LDFLAGS) $(INCLUDE) -c $<

%.o: %.c
	$(CC) $(CFLAGS) $(DEFINES) $(LDFLAGS) $(INCLUDE) -c $<

clean:
	rm -f $(TARGET) $(OBJS)
