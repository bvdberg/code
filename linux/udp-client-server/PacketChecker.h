#ifndef PACKETCHECKER_H
#define PACKETCHECKER_H

#include "Packet.h"

#include <stdio.h>

class PacketChecker {
public:
    PacketChecker()
        : seenPacket(false)
        , lastID(0)
        , missingPackets(0)
        , malformedPackets(0)
    {
    }
    void addPacket(char* buffer, ssize_t size) {
        if (size_t(size) < sizeof(Packet)) {
            ++malformedPackets;
            return;
        }
        Packet* header = (Packet*)buffer;
        if (!seenPacket) {
            seenPacket = true;
        } else {
            int dif = header->id - lastID;
            if (dif != 1) {
                missingPackets += dif;
            }
        }
        lastID = header->id;
    }
    void getStatus() {
        printf("lastID=%u, missing %u, errors %u\n", lastID, missingPackets, malformedPackets);
    }
private:
    bool seenPacket;
    unsigned int lastID;
    
    unsigned int missingPackets;
    unsigned int malformedPackets;

};

#endif

