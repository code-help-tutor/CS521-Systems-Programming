# Set the following to '0' to disable log messages:
LOGGER ?= 1

# Compiler/linker flags
CFLAGS += -g -Wall -fPIC -DLOGGER=$(LOGGER)
LDLIBS += -lm
LDFLAGS +=

all: libclist.so linked_list

libclist.so: clist.o
	$(CC) $(CFLAGS) $(LDLIBS) $(LDFLAGS) $^ -shared -o $@

clist.o: clist.c clist.h logger.h

linked_list: linked_list.c
	gcc -g -Wall linked_list.c -o linked_list

clean:
	rm -f *.o
	rm -f linked_list
	rm -f libclist.so
