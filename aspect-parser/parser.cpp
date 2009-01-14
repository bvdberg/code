#include <stdio.h>
#include "AspectParser.h"

using namespace std;

int main(int argc, char* argv[]) {
	if (argc < 2) { printf("Usage: parser [infile] <aspect1> <aspect2> ...\n"); return -1; }

	try {
        Aspects aspects;
        for (int i=2; i<argc; i++) aspects.push_back(string(argv[i]));
        AspectParser parser(argv[1], aspects);
	} catch (std::exception& e) {
		fprintf(stderr, "ERROR: %s\n", e.what());
	}

    return 0;
}

