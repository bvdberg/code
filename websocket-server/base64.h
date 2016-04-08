#ifndef BASE64_H
#define BASE64_H

#include <string>

std::string base64_encode(unsigned char const* in, unsigned int in_len);

int base64_decode(const char *in, char *out, int out_size);

#endif

