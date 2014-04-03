#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x;
    int y;
} Point;

static inline void point_cleanup(Point **p) {
    printf("%s() p=%p\n", __func__, *p);
    if (*p) free(*p);
}

#define _cleanup_(x) __attribute__((cleanup(x)))
#define _cleanup_point_free_ _cleanup_(point_cleanup)

int main(int argc, const char *argv[])
{
    printf("start\n");
    {
        _cleanup_point_free_ Point* p = NULL;
        p = (Point*)malloc(sizeof(Point));
    }
    printf("after\n");
    return 0;
}

