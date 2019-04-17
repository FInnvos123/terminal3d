CC = clang
CFLAGS = -g
LDFLAGS = -lncursesw -lm

src = $(wildcard *.c)
obj = $(src:.c=.o)

terminal3d: $(obj)
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(obj) terminal3d
