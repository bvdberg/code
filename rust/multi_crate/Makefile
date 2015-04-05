CFLAGS=-Wall -Wextra -Wno-unused -Wno-unused-parameter -std=c99

all: executable library.rlib

library.rlib: rary.rs
	rustc --crate-type=lib rary.rs

executable: library.rlib executable.rs
	rustc -L . executable.rs

clean:
	rm -f executable library.rlib


