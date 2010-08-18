all: test

git_version.h:
	@echo 'const char* git_version="'`git log --pretty=oneline --no-color -n 1`'";' > git_version.h

test: main.c git_version.h
	@gcc main.c -o test
