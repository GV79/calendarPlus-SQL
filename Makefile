all: list parser

list: src/LinkedListAPI.c
	gcc src/LinkedListAPI.c -Iinclude -o bin/LinkedListAPI.o -c
	ar cr bin/libllist.a bin/LinkedListAPI.o
parser: src/CalendarParser.c
	gcc src/CalendarParser.c -Iinclude -o bin/CalendarParser.o -c
	ar cr bin/libcparse.a bin/CalendarParser.o
clean:
	rm bin/LinkedListAPI.o bin/libllist.a bin/CalendarParser.o bin/libcparse.a
