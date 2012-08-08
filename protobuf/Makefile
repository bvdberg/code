.PHONY: all cpp clean

all: cpp

cpp:    add_person_cpp    list_people_cpp

clean:
	rm -f add_person_cpp list_people_cpp 
	rm -f addressbook.pb.cpp addressbook.pb.h 

addressbook.pb.cpp: addressbook.proto
	protoc --cpp_out=. addressbook.proto
	mv addressbook.pb.cc addressbook.pb.cpp

add_person_cpp: add_person.cpp addressbook.pb.cpp
	pkg-config --cflags protobuf
	c++ add_person.cpp addressbook.pb.cpp -o add_person_cpp `pkg-config --cflags --libs protobuf`

list_people_cpp: list_people.cpp addressbook.pb.cpp
	pkg-config --cflags protobuf 
	c++ list_people.cpp addressbook.pb.cpp -o list_people_cpp `pkg-config --cflags --libs protobuf`
