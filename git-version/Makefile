all: test

clean:
	@rm -f test git_version.h

git_version.h:
	@./gen_version.sh

test: main.c git_version.h
	@gcc main.c -o test
