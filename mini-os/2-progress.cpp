#include <stdio.h>
/*
    Changes from first example:
    - always answer commands directly.
    - progress command
    NOTE: SPI slave cannot initiate communication

    PROBLEM: how to do timing in task, when commands can interrupt?

    Commands: Start, Cancel, Progress
*/


enum CMD {START, CANCEL, PROGRESS};
enum ACTION { NONE=0, DO_START, DO_PROGRESS};

static ACTION action = NONE;
static int cancelled = 0;


int task_do_work() {
    while (!cancelled) {
        // do work
    }
    // cleanup
    if (cancelled) {
        cancelled = 0;
        return 1;   // cancelled
    }
    return 0;   // normal finish
}

void sendResult(int result) {
    // send result over SPI bus
}

void interrupt(CMD cmd) {
    switch (cmd) {
        case START:
            if (action == NONE) {
                action = DO_START;
            } // else ignore
            break;
        case CANCEL:
            cancelled = 1;
            break;
        case PROGRESS:
            // TODO
            break;
    }
}

void clearAction() {
    // int_off();
    action = NONE;
    // int_on();
}

int main() {
    while (1) {
        switch (action) {
            case NONE:
                // sleep a bit
                break;
            case DO_START:
                clearAction();
                int result = task_do_work();
                sendResult(result);
                break;
            case DO_PROGRESS:
                // TODO
                break;
        }
    }
}

