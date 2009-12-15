#include <stdio.h>
#include <string.h>

// find the relative path difference, eg
//      target = /media/disk/foo/bar/doc.pdf
//      src =    /media/disk/faa/zoo/doc.pdf.shortcut
//      -> ../../foo/bar/doc.pdf
const char* find_rel_path(const char* target, const char* src) {
    static char result[512];
    result[0] = 0;

    // skip common part
    char* rp = result;
    const char* tp = target;
    const char* sp = src;
    const char* common = target;
    while ( *tp != 0 && *tp == *sp) {
        if (*tp == '/') common = tp;
        tp++;
        sp++;
    }
    common++;
    tp = common;

    // find out how many dirs to go up from src
    while (1) {
        sp = strchr(sp, '/');
        if (sp == NULL) break;
        sp++;
        sprintf(rp, "../");
        rp += 3;
    }
    strcpy(rp, tp); // add tail part
    return result;
}

void test(const char* target, const char* src) {
    printf("---------------------------\n");
    const char* result = find_rel_path(target, src);

    printf("target = %s\n   src = %s\n      -> %s\n", target, src, result);
}

int main(int argc, const char *argv[])
{
    test("/media/disk/document/doc1.pdf", "/media/disk/shortcut/doc1.shortcut");
    test("/media/disk/document/doc1.pdf", "/media/disk/a/b/c/doc1.shortcut");
    test("/media/disk/doc1.pdf", "/media/disk/doc1.shortcut");
    test("/media/disk/doc1.pdf", "/media/disk/a/b/doc1.shortcut");
    test("/media/disk/a/b/c/doc1.pdf", "/media/disk/doc1.shortcut");

    return 0;
}

