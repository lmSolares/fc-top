CC      = gcc
CFLAGS  = -g -I./include/
LIBS    = -lncurses
TARGET  = build/fc-top
SRCS    = src/main.c src/core/memory.c src/ui/ui_ncurses.c

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LIBS)

clean:
	rm -f build/fc-top

run:
	chmod +r build/fc-top && ./build/fc-top

.PHONY: clean
