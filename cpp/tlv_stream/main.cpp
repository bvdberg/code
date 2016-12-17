#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <arpa/inet.h>

/*
    Design:
    - Every Object has an ID and a label
    - goal of TLV structure is to be able to extend Objects, create new
        types without changing other clients.
        * add new Properties (just add TLV Type) -> unknown properties are ignored
        * Q: new Objects types? (needed?, consequences?
*/

/*
    Alignment: EVERY TLV is aligned on 4-byte boundaries. If size is smaller, zero padding
    is in between.
*/
enum Type {
    TYPE_ID = 0,
    TYPE_STRING = 1,
    TYPE_NODE = 2,
    TYPE_CHILDREN = 3,
};
#define TYPE_LAST 3

#define CASE2STR(x) case x: return #x

static const char* Str(Type t) {
    switch (t) {
    CASE2STR(TYPE_ID);
    CASE2STR(TYPE_STRING);
    CASE2STR(TYPE_NODE);
    CASE2STR(TYPE_CHILDREN);
    }
    return "?";
}

typedef struct __attribute__((__packed__)) {
    uint16_t size;
    uint8_t type;
    uint8_t pad;
} TlvHeader;

typedef struct __attribute__((__packed__)) {
    TlvHeader hdr;
    uint32_t children[0];
} ChildrenTlv;

class Parser {
public:
    static void parse(const uint8_t* data, unsigned len) {
        // TODO data must be 4 byte aligned
        const uint8_t* end = data + len;
        const uint8_t* cp = data;
        assert(data & 0x3 == 0);
        while (true) {
            if ((unsigned)(end - cp) < sizeof(TlvHeader)) break;
            const TlvHeader* tlv = (TlvHeader*)cp;
            // TODO check type
            unsigned size = ntohs(tlv->size);
            printf("TLV %14s  %3d\n", Str((Type)tlv->type), size);

            switch ((Type)tlv->type) {
            case TYPE_ID:
            case TYPE_STRING:
            case TYPE_NODE:
                break;
            case TYPE_CHILDREN:
                {
                    const ChildrenTlv* children = (ChildrenTlv*)cp;
                    unsigned numChildren = size/sizeof(uint32_t);
                    for (unsigned i=0; i<numChildren; i++) {
                        uint32_t child = ntohl(children->children[i]);
                        printf("  0x%08x\n", child);
                    }
                    break;
                }
            }
            // round up to 4 byte multiple
            if (size & 0x3) size = (size & 0x3) + 1;
            cp += sizeof(TlvHeader) + size;
            break;
        }
    }
};

int main(int argc, const char *argv[])
{
    uint8_t data[] = {  0x00, 0x02, 0x02, 0x00, 0xAA, 0xBB };
    Parser::parse(data, sizeof(data));

    uint8_t data2[] = { 0x00, 0x0c, 0x03, 0x00, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, };
    Parser::parse(data2, sizeof(data2));

    return 0;
}

