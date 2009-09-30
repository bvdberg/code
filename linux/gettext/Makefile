all:
	gcc main.c -o test
	xgettext -a main.c -o messages.pot
	msginit
	msginit -l nl_NL

clean:
	@rm -rf test
