all: test_add test_list

CPPFLAGS = `pkg-config --cflags --libs protobuf`

clean:
	@rm -f add_person_cpp list_people_cpp 
	@rm -f addressbook.pb.cpp addressbook.pb.h 

addressbook.pb.cpp: addressbook.proto
	@protoc --cpp_out=. addressbook.proto
	@mv addressbook.pb.cc addressbook.pb.cpp

test_add: add_person.cpp addressbook.pb.cpp
	c++ add_person.cpp addressbook.pb.cpp -o test_add $(CPPFLAGS)

test_list: list_people.cpp addressbook.pb.cpp
	c++ list_people.cpp addressbook.pb.cpp -o test_list $(CPPFLAGS)

