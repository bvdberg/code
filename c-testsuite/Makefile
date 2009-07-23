CC             = gcc
CFLAGS         = -c -Wall -Wextra -I .
LDFLAGS        = 
BIN_DIR        = .


SOURCES=testsuite.c mysuite1.c mysuite2.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=testrunner

all: $(SOURCES) $(EXECUTABLE)
  
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS)  -o $(BIN_DIR)/$@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $(BIN_DIR)/$@

clean:
	rm -f $(OBJECTS) 
	rm -f $(BIN_DIR)/$(EXECUTABLE) 

