CC      = gcc
CFLAGS  = -std=c11 -O2 -Wall -Wextra -Wpedantic -D__EXTENSIONS__
LDFLAGS =

EXE     = mywho
HEADERS = mywho.h strings.h
SRCS    = mywho.c buildUtmpInfoTable.c displayUtmpInfo.c pa4time.c calcIdleTime.c extraFunctions.c
OBJS    = $(SRCS:.c=.o)

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXE)
