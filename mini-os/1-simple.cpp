#include <stdio.h>

// Only ONE task running at same time. Multiple different operations are supported
// Only Start, Cancel
// -> work can only be cancelled (no progress)
// -> No command Queuing

// -> Interrupt sets actions
// -> Main clears actions

// -> Handling of commands/work happens outside interrupts.
// ISSUE: can interrupt handle SPI commands?

/*
    Commands: Start, Cancel
    Action: do_start, do_cancel
    State: Idle, Working

    Start:      Idle -> do_start
                Working -> Error?
    Cancel:     Idle -> Error?
                Working -> do_cancel
    Interrupt handler should set Action + State
    if main handler changes Action/State, turn off interrupts

*/


enum CMD {START, CANCEL};
enum ACTION { NONE=0, DO_START};

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
        }
    }
}

