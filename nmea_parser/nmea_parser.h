#ifndef NMEA_PARSER_H
#define NMEA_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct nmea_parser_ nmea_parser;

nmea_parser* nmea_parser_create();

void nmea_parser_destroy(nmea_parser* parser);

void nmea_parser_add(nmea_parser* parser, char* line);

#ifdef __cplusplus
}
#endif

#endif

