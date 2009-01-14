#include <stdio.h>
#include "AspectParser.h"

using namespace std;

int main(int argc, char* argv[]) {
	if (argc < 2) { printf("Usage: parser [infile] <aspect1> <aspect2> ...\n"); return -1; }

	try {
        AspectParser parser;
        for (int i=2; i<argc; i++) parser.addAspect(string(argv[i]));
        parser.parse(argv[1]);
	} catch (std::exception& e) {
		fprintf(stderr, "ERROR: %s\n", e.what());
	}

    return 0;
}

