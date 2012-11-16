// usage: $ ./regex_sample extended "[a-z]+[0-9]{2,3}" abc123
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>


void showargs(int argc, const char** argv) {
  printf("Program argc=%d\n", argc);
  for (int i = 0; i < argc; ++i)
    printf("[%d]: %s\n", i, argv[i]);
}


void report_regex_error(int errcode, const regex_t* preg) {
  const size_t BUFSIZE = 512;
  char buf[BUFSIZE];

  size_t errlen = regerror(errcode, preg, buf, BUFSIZE);
  assert(errlen > 0 && errlen < BUFSIZE);
  printf("regerror: %s\n", buf);
}


int main(int argc, const char* argv[])
{
  // Used here to make sure shell quotiong doesn't screw up with our
  // regexes.
  showargs(argc, argv);
  printf("--------\n");

  if (argc < 4) {
    printf("Usage: %s <regex_type> <regex> <string>\n", argv[0]);
    exit(1);
  }

  const char* arg_type = argv[1];
  const char* arg_regex = argv[2];
  const char* arg_string = argv[3];

  int extended = 0;
  if (!strcmp(arg_type, "extended"))
    extended = 1;
  else if (!strcmp(arg_type, "basic"))
    extended = 0;
  else {
    printf("Expected regex_type to be \"extended\" or \"basic\"\n");
    exit(1);
  }

  regex_t compiled_regex;
  const size_t max_groups = 9;
  size_t errcode;
  int regflags = 0;

  // Set up flags for regcomp
  if (extended)
    regflags |= REG_EXTENDED;

  // Compile the regex. Return code != 0 means an error.
  if ((errcode = regcomp(&compiled_regex, arg_regex, regflags))) {
    report_regex_error(errcode, &compiled_regex);
    exit(1);
  }

  regmatch_t match_groups[max_groups];
  if (regexec(&compiled_regex, arg_string,
              max_groups, match_groups, 0) == 0) {
    // Go over all matches. A match with rm_so = -1 signals the end
    for (size_t i = 0; i < max_groups; ++i) {
      if (match_groups[i].rm_so == -1)
        break;
      printf("Match group %zu: ", i);
      for (regoff_t p = match_groups[i].rm_so;
           p < match_groups[i].rm_eo; ++p) {
        putchar(arg_string[p]);
      }
      putchar('\n');
    }
  } else {
    printf("No match\n");
  }

  return 0;
}
