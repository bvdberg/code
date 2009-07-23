all:
	@g++ main.cpp -pg -o example1 -O2 -lc -g

run:
	@./example 1000

analyse:
	@gprof example1 gmon.out -p
	@echo or compile with -g and use gprof -A to get annotated source

clean:
	@rm -rf example1 gmon.out
