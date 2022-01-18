# GNU Makefile

CC     		:= gcc
CFLAGS 		:= -g -Wall
LDFLAGS 	:=
INCLUDE 	:= -I..
SRC			:= demo.c demo_stdout.c log.c
OBJS  		:= $(SRC:%.c=%.o)

TARGET		:= demo

.PHONY: all clean

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE) -o $@ $^

demo.o: demo.c demo.h ../log.h

demo_stdout.o: demo_stdout.c demo.h ../log.h

log.o: ../log.c ../log.h
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE) -c $<

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) $(INCLUDE) -c $<

clean:
	rm -f $(TARGET) $(OBJS)
