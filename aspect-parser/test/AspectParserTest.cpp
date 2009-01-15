#include <string>
#include <string.h>

#include "yaffut.h"
#include "myassert.h"

#define private public
#include "AspectParser.h"
#undef private

using namespace std;

struct AspectParserTest {
    AspectParser parser;

    void test(const char* input, const char* expected, const char* caller, int line) {
        static char buffer[4096];
        strncpy(buffer, input, 4095);
        stringstream output;
        parser.parse(buffer, strlen(buffer), output);
        Assert::assert_str_equal(expected, output.str(), caller, line);
    }
};
#define TEST_INPUT() test(input, expected, __FILE__, __LINE__)


TEST(AspectParserTest, testEmptyInput) {
    const char* input = "";
    const char* expected = "";
    TEST_INPUT();
}


TEST(AspectParserTest, testOneLine) {
    const char* input = "abcd";
    const char* expected = "abcd";
    TEST_INPUT();
}


