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

TEST(AspectParserTest, testOneLineWithoutNewLine) {
    const char* input = "abcd";
    const char* expected = "abcd";
    TEST_INPUT();
}

TEST(AspectParserTest, testSimpleLines) {
    const char* input = "abcd\nefgh\n";
    const char* expected = "abcd\nefgh\n";
    TEST_INPUT();
}

TEST(AspectParserTest, testOnlyNewlines) {
    const char* input = "\n\n";
    const char* expected = "\n\n";
    TEST_INPUT();
}

TEST(AspectParserTest, testOnlyAspects) {
    const char* input =
        "%%begin A\n"
        "%%end A\n";
    const char* expected = "";
    TEST_INPUT();
}

TEST(AspectParserTest, testBeginAspectWithoutName) {
    const char* input = "%%begin \n";
    const char* expected = "";
    try {
        TEST_INPUT();
        ASSERT_FAIL();
    } catch (AspectParserException& e) {
        ASSERT_STR_EQUAL("missing aspect name (line: 1)", e.what());
    }
}

TEST(AspectParserTest, testEndAspectWithoutName) {
    const char* input =
        "%%begin A\n"
        "%%end \n";
    const char* expected = "";
    try {
        TEST_INPUT();
        ASSERT_FAIL();
    } catch (AspectParserException& e) {
        ASSERT_STR_EQUAL("missing aspect name (line: 2)", e.what());
    }
}


