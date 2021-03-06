#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "ctest.h"
#include "json_parser.h"

#define PARSE_OK() ASSERT_TRUE(json_Parser_parse(&data->parser, text));
#define PARSE_FAIL() ASSERT_FALSE(json_Parser_parse(&data->parser, text));

#define READ_FILE(x) const char* text = read_file(x);

static const char* read_file(const char* filename) {
    static char buffer[4096];

    int fd = open(filename, O_RDONLY);
    ASSERT_NOT_EQUAL(fd, -1);

    int numread = read(fd, buffer, sizeof(buffer)-1);
    ASSERT_TRUE(numread > 0);
    buffer[numread] = 0;

    close(fd);

    return buffer;
}

CTEST_DATA(json_test) {
    json_Parser parser;
};

CTEST_SETUP(json_test) {
    json_Parser_init(&data->parser);
}

CTEST_TEARDOWN(json_test) {
    json_Parser_destroy(&data->parser);
}

CTEST2(json_test, empty_array) {
    const char text[] = "[ ]";
    PARSE_OK();
}

CTEST2(json_test, empty_obj) {
    const char text[] = "{ }";
    PARSE_OK();
}

CTEST2(json_test, obj) {
    const char text[] = "{ \"key1\" : \"value1\", \"key2\" : \"value2\" }";
    PARSE_OK();
}

CTEST2(json_test, obj_comma_at_end) {
    const char text[] = "{ \"key1\" : \"value1\", \"key2\" : \"value2\" , }";
    PARSE_FAIL();
    ASSERT_STR("superfluous comma at line 1:40", json_Parser_getDiag(&data->parser));
}

CTEST2(json_test, array_comma_at_end) {
    const char text[] = "[ 1, 2, 3, ]";
    PARSE_FAIL();
    ASSERT_STR("superfluous comma at line 1:10", json_Parser_getDiag(&data->parser));
}

CTEST2(json_test, numbers) {
    const char text[] = "[ 2, -4, 6, -7, 3.14, -9.87 ]";
    PARSE_OK();
    json_Iter iter = json_Parser_getArrayIter(&data->parser, NULL);
    ASSERT_TRUE(json_Iter_check_schema(&iter, "ssssss"));
    ASSERT_STR("2", json_Iter_getValue(&iter));
    json_Iter_next(&iter);
    ASSERT_STR("-4", json_Iter_getValue(&iter));
    json_Iter_next(&iter);
    ASSERT_STR("6", json_Iter_getValue(&iter));
    json_Iter_next(&iter);
    ASSERT_STR("-7", json_Iter_getValue(&iter));
    json_Iter_next(&iter);
    ASSERT_STR("3.14", json_Iter_getValue(&iter));
    json_Iter_next(&iter);
    ASSERT_STR("-9.87", json_Iter_getValue(&iter));
    json_Iter_next(&iter);
    ASSERT_TRUE(json_Iter_done(&iter));
}

CTEST2(json_test, keywords) {
    const char text[] = "[ true, false, null ]";
    PARSE_OK();
    json_Iter iter = json_Parser_getArrayIter(&data->parser, NULL);
    ASSERT_TRUE(json_Iter_check_schema(&iter, "sss"));
}

CTEST2(json_test, array_mixed) {
    READ_FILE("files/test4.json");
    PARSE_OK();
    json_Iter iter = json_Parser_getArrayIter(&data->parser, NULL);
    ASSERT_TRUE(json_Iter_check_schema(&iter, "ssso(ss)"));
}

