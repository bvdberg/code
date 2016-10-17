#include <unistd.h>
#include <stdlib.h>
#include "ctest.h"
#include "utils.h"

CTEST_DATA(utils) {
    char* argv[30];
};
// basic test without setup/teardown
CTEST2(utils, noargs) {
    ASSERT_EQUAL(1, parseArgs("cmd", "", data->argv, 30));
}

CTEST2(utils, one_arg) {
    ASSERT_EQUAL(2, parseArgs("cmd", "one", data->argv, 30));
    ASSERT_STR("cmd", data->argv[0]);
    ASSERT_STR("one", data->argv[1]);
    ASSERT_NULL(data->argv[2]);
}

CTEST2(utils, two_args) {
    ASSERT_EQUAL(3, parseArgs("cmd", "one two", data->argv, 30));
    ASSERT_STR("cmd", data->argv[0]);
    ASSERT_STR("one", data->argv[1]);
    ASSERT_STR("two", data->argv[2]);
}

CTEST2(utils, quoted_arg) {
    ASSERT_EQUAL(2, parseArgs("cmd", "'one two'", data->argv, 30));
    ASSERT_STR("cmd", data->argv[0]);
    ASSERT_STR("one two", data->argv[1]);
}

CTEST2(utils, partial_quoted_arg) {
    ASSERT_EQUAL(2, parseArgs("cmd", "--header='one two'", data->argv, 30));
    ASSERT_STR("cmd", data->argv[0]);
    ASSERT_STR("--header=one two", data->argv[1]);
}

CTEST2(utils, combi) {
    ASSERT_EQUAL(4, parseArgs("cmd", "one --header='one two' three", data->argv, 30));
    ASSERT_STR("cmd", data->argv[0]);
    ASSERT_STR("one", data->argv[1]);
    ASSERT_STR("--header=one two", data->argv[2]);
    ASSERT_STR("three", data->argv[3]);
}

CTEST2(utils, multi_spaces) {
    ASSERT_EQUAL(3, parseArgs("cmd", " one  two ", data->argv, 30));
    ASSERT_STR("cmd", data->argv[0]);
    ASSERT_STR("one", data->argv[1]);
    ASSERT_STR("two", data->argv[2]);
    ASSERT_NULL(data->argv[3]);
}

CTEST2(utils, url) {
    const char* args = "--no-check-certificate --header='Content-Type: application/json' --header='x-abbevci-version: 2016-05-02' --post-file=/tmp/update_request.json -O /tmp/update_response.json https://cluster.westeurope.cloudapp.azure.com:8443/api/deviceregistration/updater";
    ASSERT_EQUAL(8, parseArgs("/usr/bin/wget", args, data->argv, 30));
    ASSERT_STR("/usr/bin/wget", data->argv[0]);
    ASSERT_STR("--no-check-certificate", data->argv[1]);
    ASSERT_STR("--header=Content-Type: application/json", data->argv[2]);
    ASSERT_STR("--header=x-abbevci-version: 2016-05-02", data->argv[3]);
    ASSERT_STR("--post-file=/tmp/update_request.json", data->argv[4]);
    ASSERT_STR("-O", data->argv[5]);
    ASSERT_STR("/tmp/update_response.json", data->argv[6]);
    ASSERT_STR("https://cluster.westeurope.cloudapp.azure.com:8443/api/deviceregistration/updater", data->argv[7]);
}

