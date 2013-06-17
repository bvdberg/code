// See README.txt for information and build instructions.

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include "addressbook.pb.h"
using namespace std;

u_int64_t getCurrentTime()
{
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    u_int64_t now64 = now.tv_sec;
    now64 *= 1000000;
    now64 += (now.tv_nsec/1000);
    return now64;
}

void ListPeople2(const tutorial::AddressBook& address_book) {
  for (int i = 0; i < address_book.person_size(); i++) {
    const tutorial::Person& person = address_book.person(i);

    for (int j = 0; j < person.phone_size(); j++) {
      const tutorial::Person::PhoneNumber& phone_number = person.phone(j);

      switch (phone_number.type()) {
        case tutorial::Person::MOBILE:
          break;
        case tutorial::Person::HOME:
          break;
        case tutorial::Person::WORK:
          break;
      }
    }
  }
}
// Main function:  Reads the entire address book from a file and prints all
//   the information inside.
int main(int argc, char* argv[]) {
  // Verify that the version of the library that we linked against is
  // compatible with the version of the headers we compiled against.
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  if (argc != 2) {
    cerr << "Usage:  " << argv[0] << " ADDRESS_BOOK_FILE" << endl;
    return -1;
  }

  u_int64_t t1 = getCurrentTime();
  tutorial::AddressBook address_book;

  {
    // Read the existing address book.
    fstream input(argv[1], ios::in | ios::binary);
    if (!address_book.ParseFromIstream(&input)) {
      cerr << "Failed to parse address book." << endl;
      return -1;
    }
  }

  ListPeople2(address_book);
  u_int64_t t2 = getCurrentTime();
  printf("listing %d entries took %lld usec\n", address_book.person_size(), t2 - t1);
  // Optional:  Delete all global objects allocated by libprotobuf.
  google::protobuf::ShutdownProtobufLibrary();

  return 0;
}
