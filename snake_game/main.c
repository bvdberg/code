#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#define ANSI_BLACK    "\033[0;30m"
#define ANSI_RED      "\033[0;31m"
#define ANSI_GREEN    "\033[0;32m"
#define ANSI_YELLOW   "\033[0;33m"
#define ANSI_BLUE     "\033[0;34m"
#define ANSI_MAGENTA  "\033[0;35m"
#define ANSI_CYAN     "\033[0;36m"
#define ANSI_GREY     "\033[0;37m"
#define ANSI_DARKGREY "\033[01;30m"
#define ANSI_BRED     "\033[01;31m"
#define ANSI_BGREEN   "\033[01;32m"
#define ANSI_BYELLOW  "\033[01;33m"
#define ANSI_BBLUE    "\033[01;34m"
#define ANSI_BMAGENTA "\033[01;35m"
#define ANSI_BCYAN    "\033[01;36m"
#define ANSI_WHITE    "\033[01;37m"
#define ANSI_NORMAL   "\033[0m"

static u_int64_t getCurrentTime() {
    struct timeval now;
    gettimeofday(&now, 0);
    u_int64_t now64 = now.tv_sec * 1000000 + now.tv_usec;
    return now64;
}

typedef struct {
    char x;
    char y;
    unsigned char flags;
} Coor;

typedef struct {
    Coor side1[4];
    Coor side2[4];
} Piece;

typedef enum {
    FIELD_FREE = 0,
    FIELD_BLOCKED,
    FIELD_TAKEN,    // + index of piece
} FieldStatus;

typedef struct {
    FieldStatus status : 2;
    unsigned piece_id : 4;
    unsigned flags : 3;
    unsigned expected : 3;  // set by other pieces
} Field;

typedef struct {
    Field fields[5*5];
    int haveHead;
    int haveTail;
} __attribute__((packed)) Board;
Board board;

typedef enum {
    NORTH = 1,
    EAST  = 2,
    SOUTH = 3,
    WEST  = 4,
    HEAD  = 5,
    TAIL  = 6,
} Flags;

static Piece pieces[] = {
    { // Hollow piece
        .side1 = { { 0, 0, 0 } },
        .side2 = { { 0, 0, 0 } },
    },
    { // S-shaped
        .side1 = { { 0, 0, 0 }, { 0, 1, NORTH }, { 1, 1, SOUTH }, { 1, 2, 0 } },
        .side2 = { { 0, 0, 0 }, { 1, 0, 0 }, { 1, 1, WEST }, { 2, 1, SOUTH } },
    },
    { // L-shaped
        .side1 = { { 0, 0, 0 }, { 0, 1 , 0 }, { 0, 2, NORTH }, { 1, 0, EAST } },
        .side2 = { { 0, 0, 0 }, { 1, 0 , 0 }, { 1, 1, 0 }, { 1, 2, 0 } }
    },
    { // L-shaped, head
        .side1 = { { 0, 0, 0 }, { 0, 1, 0 }, { 0, 2, 0 }, { 1, 0, 0 } },
        .side2 = { { 0, 0, NORTH }, { 1, 0 , 0 }, { 1, 1, 0 }, { 1, 2, HEAD } }
    },
    { // 3 1-side empty
        .side1 = { { 0, 0, 0 }, { 0, 1, 0 }, { 1, 0, 0 } },
        .side2 = { { 0, 0, 0 }, { 0, 1, EAST }, { 1, 0, EAST } },
    },
    { // 3 2-tails
        .side1 = { { 0, 0, WEST }, { 0, 1, TAIL }, { 1, 0, 0 } },
        .side2 = { { 0, 0, SOUTH }, { 0, 1, 0 }, { 1, 0, TAIL } },
    },
    { // I-shaped, 3-straight
        .side1 = { { 0, 0, SOUTH }, { 0, 1, 0 }, { 0, 2, HEAD } },
        .side2 = { { 0, 0, SOUTH }, { 0, 1, 0 }, { 0, 2, WEST } },
    },
    { // I-shaped, 2 long
        .side1 = { { 0, 0, WEST }, { 0, 1, NORTH } },
        .side2 = { { 0, 0, 0 }, { 0, 1, 0 } },
    },
};

#define num_pieces (sizeof(pieces) / sizeof(pieces[0]))
#define num_options 8

typedef struct {
    Coor parts[4];
} Option;

static Option options[num_options][num_pieces];

typedef enum {
    PLACE_FREE = 0,  // not placed yet
    PLACE_SETUP,     // placed by initial setup
    PLACE_PLACED,    // placed during search
} __attribute__((packed)) PositionType;

typedef struct {
    PositionType type;
    char option;
    char x;
    char y;
} Position;
static Position setup[num_pieces] = {
    [0] = { PLACE_SETUP, 0, 1, 4, },
};

static Coor coor_rotate(Coor input, int rotation) {
    int x = input.x;
    int y = input.y;
    int t;
    switch (rotation) {
    case 0: // nothing to do
        break;
    case 1: // 90 degrees right
        t = x;
        x = y;
        y = -t;
        break;
    case 2: // 180 degrees right
        x = -x;
        y = -y;
        break;
    case 3: // 270 degrees right
        t = x;
        x = -y;
        y = t;
        break;
    }
    int flags = input.flags;  // keep HEAD/TAIL, rotate NESW
    if (rotation > 0 && flags >= NORTH && flags <= WEST) {
        flags += rotation;
        if (flags > 4) flags -= 4;
    }

    Coor result;
    result.x = x;
    result.y = y;
    result.flags = flags;
    return result;
}

// calculate all rotations + flipped rotations
static void calculate_rotations() {
    memset(options, 0, sizeof(options));
    for (unsigned p=0; p<num_pieces; p++) {
        const Piece* P = &pieces[p];
        for (unsigned o=0; o<4; o++) {  // 4 rotations
            Option* O1 = &options[o][p];
            for (unsigned i=0; i<4; i++) {  // side1
                Coor c = P->side1[i];
                if (i != 0 && c.x == 0 && c.y == 0) break;
                O1->parts[i] = coor_rotate(c, o);
            }
            Option* O2 = &options[o+4][p];
            for (unsigned i=0; i<4; i++) {  // side2
                Coor c = P->side2[i];
                if (i != 0 && c.x == 0 && c.y == 0) break;
                O2->parts[i] = coor_rotate(c, o);
            }
        }
    }
}

static void board_set(int x, int y, Field field) {
    board.fields[x + y * 5] = field;
}

static Field board_get(int x, int y) {
    return board.fields[x + y * 5];
}

static void board_clearflags(int x, int y) {
    board.fields[x + y * 5].expected = 0;
}

static void board_setExpected(int x, int y, int side) {
    board.fields[x + y * 5].expected = side;
}

static void board_init() {
    Field f;
    f.status = FIELD_FREE;
    f.piece_id = 0;
    f.flags = 0;
    f.expected = 0;
    for (int y=0; y<5; y++) {
        for (int x=0; x<5; x++) {
            board_set(x, y, f);
        }
    }
    f.status = FIELD_BLOCKED;
    board_set(0, 4, f);
    board.haveHead = 0;
    board.haveTail = 0;
}

static char flags2char(int flags) {
    char f = ' ';
    switch (flags) {
    case NORTH:
        f = 'N';
        break;
    case EAST:
        f = 'E';
        break;
    case SOUTH:
        f = 'S';
        break;
    case WEST:
        f = 'W';
        break;
    case HEAD:
        f = 'H';
        break;
    case TAIL:
        f = 'T';
        break;
    }
    return f;
}

static void board_print() {
    char buffer[1024];
    memset(buffer, 0, sizeof(buffer));
    char* cp = buffer;
    cp += sprintf(cp, "Board: Head=%d  Tail=%d\n", board.haveHead, board.haveTail);
    for (int y=4; y>=0; y--) {
        cp += sprintf(cp, "---------------------\n");
        for (int x=0; x<5; x++) {
            *cp++ = '|';
            Field field = board.fields[x + y * 5];
            switch (field.status) {
            case FIELD_FREE:
                if (field.expected) {
                    cp += sprintf(cp, ANSI_RED"%c"ANSI_NORMAL, flags2char(field.expected));
                } else {
                    *cp++ = ' ';
                }
                *cp++ = ' ';
                *cp++ = ' ';
                break;
            case FIELD_BLOCKED:
                cp += sprintf(cp, ANSI_DARKGREY"XXX"ANSI_NORMAL);
                break;
            default:
            {
                int flags = field.flags;
                if (field.expected) {
                    cp += sprintf(cp, ANSI_RED"%c"ANSI_NORMAL, flags2char(field.expected));
                } else {
                    *cp++ = ' ';
                }
                cp += sprintf(cp, ANSI_GREEN"%c", '0' + field.piece_id);
                *cp++ = flags2char(flags);
                cp += sprintf(cp, ANSI_NORMAL);
                break;
            }
            }
        }
        *cp++ = '|';
        *cp++ = '\n';
    }
    cp += sprintf(cp, "---------------------\n");
    *cp = 0;
    puts(buffer);
}

static int board_check_fault(int xx, int yy, int expect) {
    Field F = board_get(xx, yy);
    switch (F.status) {
    case FIELD_FREE:
        if (F.expected && F.expected != expect) return 1;
        break;
    case FIELD_BLOCKED:
        return 1;
    case FIELD_TAKEN:
        if (F.flags != expect) return 1;
        break;
    }
    return 0;
}

// return 0=OK, 1=Not OK
static int board_try_option(const Option* P, int x, int y) {
    for (unsigned i=0; i<4; i++) {
        Coor c = P->parts[i];
        if (i != 0 && c.x == 0 && c.y == 0) return 0;    // ignore unused parts
        int xx = x + c.x;
        if (xx < 0 || xx > 4) return 1;
        int yy = y + c.y;
        if (yy < 0 || yy > 4) return 1;
        Field F = board_get(xx, yy);
        if (F.status != FIELD_FREE) return 1;
        if (F.expected != 0 && F.expected != c.flags) return 1;
        switch (c.flags) {
        case NORTH:
            if (yy == 4) return 1;
            if (board_check_fault(xx, yy+1, SOUTH)) return 1;
            break;
        case EAST:
            if (xx == 4) return 1;
            if (board_check_fault(xx+1, yy, WEST)) return 1;
            break;
        case SOUTH:
            if (yy == 0) return 1;
            if (board_check_fault(xx, yy-1, NORTH)) return 1;
            break;
        case WEST:
            if (xx == 0) return 1;
            if (board_check_fault(xx-1, yy, EAST)) return 1;
            break;
        case HEAD:
            if (board.haveHead) return 1;
            break;
        case TAIL:
            if (board.haveTail) return 1;
            break;
        }
    }
    return 0;
}

static void board_add(const Option* P, int x, int y, unsigned id) {
    for (unsigned i=0; i<4; i++) {
        Coor c = P->parts[i];
        if (i != 0 && c.x == 0 && c.y == 0) break;
        int xx = x + c.x;
        int yy = y + c.y;
        Field field = board_get(xx, yy);;
        assert(field.status == FIELD_FREE);
        field.status = FIELD_TAKEN;
        field.piece_id = id;
        field.flags = c.flags;
        board_set(xx, yy, field);
        switch (c.flags) {
        case NORTH:
            board_setExpected(xx, yy+1, SOUTH);
            break;
        case EAST:
            board_setExpected(xx+1, yy, WEST);
            break;
        case SOUTH:
            field.flags = NORTH;
            board_setExpected(xx, yy-1, NORTH);
            break;
        case WEST:
            board_setExpected(xx-1, yy, EAST);
            break;
        case HEAD:
            board.haveHead = 1;
            break;
        case TAIL:
            board.haveTail = 1;
            break;
        }
    }
}

static void board_rem(const Option* P, int x, int y) {
    for (unsigned i=0; i<4; i++) {
        Coor c = P->parts[i];
        if (i != 0 && c.x == 0 && c.y == 0) break;
        int xx = x + c.x;
        int yy = y + c.y;
        Field field = board_get(xx, yy);
        field.status = FIELD_FREE;
        field.piece_id = 0;
        field.flags = 0;
        board_set(xx, yy, field);
        switch (c.flags) {
        case NORTH:
            board_clearflags(xx, yy+1);
            break;
        case EAST:
            board_clearflags(xx+1, yy);
            break;
        case SOUTH:
            board_clearflags(xx, yy-1);
            break;
        case WEST:
            board_clearflags(xx-1, yy);
            break;
        case HEAD:
            board.haveHead = 0;
            break;
        case TAIL:
            board.haveTail = 0;
            break;
        }
    }
}

static unsigned found;
static unsigned long long tried;
static void board_try_piece(unsigned p) {
    if (p == num_pieces) {
        found++;
        printf("Found solution %u\n", found);
        board_print();
        return;
    }
    if (setup[p].type == PLACE_SETUP) {
        int o = setup[p].option;
        int x = setup[p].x;
        int y = setup[p].y;
        if (board_try_option(&options[o][p], x, y)) {
            fprintf(stderr, "Invalid setup: piece %d, option %d at %d,%d\n", p, o, x, y);
            exit(-1);
        }
        board_add(&options[o][p], x, y, p);
        board_try_piece(p+1);
        return;
    }
    for (int y=4; y>=0; y--) {
        for (int x=0; x<5; x++) {
            Field F = board_get(x, y);
            if (F.status != FIELD_FREE) continue;;
            for (unsigned o=0; o<num_options; o++) {
                if (!board_try_option(&options[o][p], x, y)) {
                    board_add(&options[o][p], x, y, p);
                    board_try_piece(p+1);
                    board_rem(&options[o][p], x, y);
                }
                tried++;
                if (p==0) break;
            }
        }
    }
}


int main() {
    u_int64_t t1 = getCurrentTime();
    calculate_rotations();
    board_init();
    // TODO give starting situation
    board_try_piece(0);
    u_int64_t t2 = getCurrentTime();
    printf("%lluM tries, found %d solutions in %llu ms\n", tried/1000000, found, (t2-t1)/1000);
    return 0;
}

