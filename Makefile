CC      = gcc
CFLAGS  = -g -I./include/
LIBS    = -lncursesw
TARGET  = build/fc-top
SRCS    = src/main.c src/system/status.o src/ui/ui_ncurses.c

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET) $(LIBS)

clean:
	rm -f build/fc-top

run:
	chmod +r build/fc-top && ./build/fc-top

.PHONY: clean
