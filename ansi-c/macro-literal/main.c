#include <stdio.h>

typedef enum {
    WMI_SET_BT_STATUS_CMDID = 0x1001,
    WMI_SET_BT_PARAMS_CMDID,
    WMI_SET_KEEPALIVE_CMDID = 0x2001,
    WMI_GET_KEEPALIVE_CMDID,
    WMI_SET_APPIE_CMDID = 0x3001, 
    WMI_GET_APPIE_CMDID,
    WMI_SET_WSC_STATUS_CMDID
} ID;

#define CASE(a) case a: return #a;

const char* id2str(ID id) {
    static char buffer[24];
    switch(id) {
    CASE(WMI_SET_BT_STATUS_CMDID)
    CASE(WMI_SET_BT_PARAMS_CMDID)
    CASE(WMI_SET_KEEPALIVE_CMDID)
    CASE(WMI_GET_KEEPALIVE_CMDID)
    CASE(WMI_SET_APPIE_CMDID)
    CASE(WMI_GET_APPIE_CMDID)
    CASE(WMI_SET_WSC_STATUS_CMDID)
    default:
        snprintf(buffer, sizeof(buffer), "Unknown EventID(0x%04x)", id);
        return buffer;
    }
}

void test(ID id) { printf("0x%04x -> '%s'\n", id, id2str(id)); }

int main() {
    test(0x1001);
    test(0x1002);
    test(0x2001);
    test(0x2002);
    test(0x3001);
    test(0x3002);
    test(0x0000);
    return 0;
}

