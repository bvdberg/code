#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define json_MAX_DIAG 256

typedef struct json_Data_ json_Data;

typedef struct {
  json_Data* data;
  char message[json_MAX_DIAG];
} json_Parser;

typedef struct {
  const json_Data* data;
  uint32_t cur_idx;
} json_Iter;

typedef struct {
  char* buffer;
  uint32_t size;
  uint32_t capacity;
  char hasEntry[6];
  char inArray[6];
  uint32_t indent;
} json_Builder;

// parser
void json_Parser_init(json_Parser* p);
void json_Parser_destroy(json_Parser* p);
int json_Parser_parse(json_Parser* p, const char* text);
void json_Parser_report(const json_Parser* p);
void json_Parser_dump(const json_Parser* p, char verbose);
const char* json_Parser_getDiag(json_Parser* p);
const char* json_Parser_find_text(const json_Parser* p, const char* key);

// iter
json_Iter json_Parser_getArrayIter(const json_Parser* p, const char* key);
json_Iter json_Parser_getObjectIter(const json_Parser* p, const char* key);
char json_Iter_done(const json_Iter* i);
void json_Iter_next(json_Iter* i);
const char* json_Iter_find_text(const json_Iter* i, const char* key);
const char* json_Iter_getKey(const json_Iter* i);
const char* json_Iter_getValue(const json_Iter* i);
int json_Iter_isValue(const json_Iter* i);
int json_Iter_isObject(const json_Iter* i);
json_Iter json_Iter_getObjectIter(const json_Iter* i);
char json_Iter_check_schema(const json_Iter* i, const char* schema);

// builder
void json_Builder_init(json_Builder* b, uint32_t capacity);
void json_Builder_free(json_Builder* b);
void json_Builder_finalize(json_Builder* b);
const char* json_Builder_getData(const json_Builder* b);
uint32_t json_Builder_getSize(const json_Builder* b);
void json_Builder_addNumber(json_Builder* b, const char* key, int64_t value);
void json_Builder_addValue(json_Builder* b, const char* key, const char* value);
void json_Builder_addObject(json_Builder* b, const char* key);
void json_Builder_closeObject(json_Builder* b);
void json_Builder_addArray(json_Builder* b, const char* key);
void json_Builder_closeArray(json_Builder* b);
void json_Builder_reset(json_Builder* b);

#ifdef __cplusplus
}
#endif

#endif
