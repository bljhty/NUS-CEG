.SILENT: test tidy
.PHONY: test tidy
SHELL := /bin/bash
FILES=collatz rectangle prime pattern
CC=clang
LDLIBS=-lm -lcs1010
CFLAGS=@compile_flags.txt 

all: tidy
test: $(FILES)
	for question in $(FILES); do ./test.sh $$question; done

tidy: test
	clang-tidy -quiet *.c 2> /dev/null
clean:
	rm $(FILES)
# vim:noexpandtab
