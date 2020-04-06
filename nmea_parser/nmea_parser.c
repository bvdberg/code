#include "nmea_parser.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE 128

#define CR 0x0D
#define LF 0x0A

typedef enum {
    INIT = 0,       // just search for CR
    INIT_CR,        // got CR, expect LF
    IN_LINE,        // during line, until CR
    LINE_END,       // got CR, expect LF
} nmea_state;

struct nmea_parser_ {
    nmea_state state;
    char* line_end;
    char* cur;          // used for parsing inside line
    char line_buf[MAX_LINE];
};

nmea_parser* nmea_parser_create() {
    nmea_parser* parser = calloc(1, sizeof(nmea_parser));
    parser->line_end = parser->line_buf;
    return parser;
}

void nmea_parser_destroy(nmea_parser* parser) {
    free(parser);
}

static uint8_t nmea_parser_checksum(const char *sentence)
{
    // The optional checksum is an XOR of all bytes between "$" and "*".
    uint8_t checksum = 0x00;

    while (*sentence && *sentence != '*') checksum ^= *sentence++;

    return checksum;
}

static uint8_t hex2val(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0;
}

static uint8_t hex2num(const char* str) {
    uint8_t hi = hex2val(str[0]);
    uint8_t lo = hex2val(str[1]);
    return hi << 4 | lo;
}

static char* consume_field(nmea_parser* parser) {
    char* start = parser->cur;
    char* cp = start;
    while (*cp) {
        if (*cp == ',') {
            *cp = 0;
            cp++;
            break;
        }
        cp++;
    }
    parser->cur = cp;
    return start;
}

// example: $GPRMC,102157.000,A,5134.7186,N,00518.7930,E,0.28,107.49,060420,,,A*65
static void nmea_parser_handle_rmc(nmea_parser* parser) {
    //printf("RMC: %s\n", parser->cur);
    printf("RMC sentence:\n");
    char* utc = consume_field(parser);        // UTC
    char* status = consume_field(parser);     // status
    char* latitude = consume_field(parser);   // latitude
    char* north = consume_field(parser);      // N/S
    char* longitude = consume_field(parser);  // longitude
    char* east = consume_field(parser);       // E/W
    consume_field(parser);                    // Speed over ground in knots
    consume_field(parser);                    // course over ground in degrees
    char* date = consume_field(parser);       // date
    consume_field(parser);                    // magnetic variation
    consume_field(parser);                    // magnetic variation E/W indicator
    consume_field(parser);                    // mode
    printf("  UTC %s (hhmmss.ss)\n", utc);
    printf("  status %s\n", status);
    printf("  position: %s %s, %s %s\n", latitude, north, longitude, east);
    printf("  date %s (ddmmyy)\n", date);
}

// TODO use longjmp
static void nmea_parser_handle_line(nmea_parser* parser, char* line, unsigned len) {
    if (len < 10) return;    // just need at least a sentence identifier and checksum.
    uint8_t sum1 = nmea_parser_checksum(line + 1); // skip $
    uint8_t sum2 = hex2num(&line[len-2]);
    if (sum1 != sum2) {
        printf("invalid checksum (expected 0x%02X, got 0x%02X)\n", sum1, sum2);
        return;
    }
    line[len-3] = 0;    // cut off checksum
    //printf("%s\n", line);
    // only care for GPRMC
    parser->cur = line;
    char* cmd = consume_field(parser);
    if (strcmp(cmd, "$GPRMC") != 0) return;
    nmea_parser_handle_rmc(parser);
}

void nmea_parser_add(nmea_parser* parser, char* data) {
    char* cp = data;
    while (*cp) {
        switch (parser->state) {
        case INIT:
            if (*cp == CR) {
                parser->state = INIT_CR;
            }
            cp++;
            break;
        case INIT_CR:
            if (*cp == LF) {
                parser->state = IN_LINE;
                parser->line_end = parser->line_buf;
                cp++;
            } else {
                parser->state = INIT;
                cp++;
            }
            break;
        case IN_LINE:
            if (*cp == CR) {
                *parser->line_end = 0;
                parser->state = LINE_END;
                cp++;
            } else {
                if (*cp != LF) {
                    // TODO check for line overflow
                    *parser->line_end = *cp;
                    parser->line_end++;
                }
                cp++;
            }
            break;
        case LINE_END:
            if (*cp == LF) {
                *cp = 0;
                nmea_parser_handle_line(parser, parser->line_buf, parser->line_end - parser->line_buf);
                parser->state = IN_LINE;
                parser->line_end = parser->line_buf;
                cp++;
            } else {
                parser->state = INIT;
                parser->line_end = parser->line_buf;
                cp++;
            }
            break;
        }
    }
}

