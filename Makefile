
CC	:= clang
CFLAGS	:= -O0 -ggdb
LDFLAGS	:= -lX11 -lXi

OUTPUT	:= ver list event

all: $(OUTPUT)

.PHONY: clean
clean:
	rm -f $(OUTPUT)
