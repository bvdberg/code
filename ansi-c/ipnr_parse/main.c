#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

bool read_number(const char** line, uint32_t* num) {
    // read  max 10 chars
    uint32_t result = 0;
    const char* cp = *line;
    uint32_t count = 0;
    while (count < 10) {
        if (*cp < '0' || *cp > '9') break;
        result *= 10;
        result += *cp - '0';
        cp++;
        count++;
    }
    *line = cp;
    *num = result;
    return (count != 0);
}


bool ipaddrconv(const char *str, uint8_t* ipaddr)
{
    for (unsigned i = 0; i < 4; ++i) {
        uint32_t num = 0;
        if (!read_number(&str, &num)) return false;
        if (num > 255) return false;
        ipaddr[i] = (uint8_t)num;
        if (i < 3 && str[0] != '.') return false;
        str++;
    }
    return true;
}

static void test(const char* str) {
    uint8_t ipnr[4] = { 0 };
    bool ok = ipaddrconv(str, ipnr);
    printf("[%16s]  -> %s   %u.%u.%u.%u\n", str, ok ? "ok " : "err", ipnr[0], ipnr[1], ipnr[2], ipnr[3]);
}

int main(int argc, const char *argv[])
{
    test("");
    test("0.0.0.");
    test("0.0.0.256");
    test("foo");
    test("0.0.0.0");
    test("1.2.3.4");
    test("1.22.33.123");
    test("777.888.999.000");
    test("1.2.3.4FOO");
    test("255.255.255.255");
    test("255.255.255.255Z");

    return 0;
}

