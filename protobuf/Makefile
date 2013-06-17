all: test_add test_list

LIBS = `pkg-config --cflags --libs protobuf`
CPPFLAGS = -Wall -Wextra -O2 -lrt

clean:
	@rm -f test_add test_list addressbook.pb.cpp addressbook.pb.h 

addressbook.pb.cpp: addressbook.proto
	@protoc --cpp_out=. addressbook.proto
	@mv addressbook.pb.cc addressbook.pb.cpp

test_add: add_person.cpp addressbook.pb.cpp
	c++ add_person.cpp addressbook.pb.cpp -o test_add $(CPPFLAGS) $(LIBS)

test_list: list_people.cpp addressbook.pb.cpp
	c++ list_people.cpp addressbook.pb.cpp -o test_list $(CPPFLAGS) $(LIBS)

