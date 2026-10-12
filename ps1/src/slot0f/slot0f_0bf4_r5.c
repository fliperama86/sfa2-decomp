/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int wait_for_event_0_to_3_slot0f(int *p) {
    while (1) {
        if (func_8015785c(p[0]) != 0) {
            return 0;
        }
        if (func_8015785c(p[1]) != 0) {
            return 1;
        }
        if (func_8015785c(p[2]) != 0) {
            return 2;
        }
        if (func_8015785c(p[3]) != 0) {
            return 3;
        }
    }
}

int wait_for_event_4_to_6_slot0f(int *p) {
    while (1) {
        if (func_8015785c(p[4]) != 0) {
            return 0;
        }
        if (func_8015785c(p[5]) != 0) {
            return 1;
        }
        if (func_8015785c(p[6]) != 0) {
            return 2;
        }
    }
}

void test_events_first_four_of_array_slot0f(int *p) {
    func_8015785c(p[0]);
    func_8015785c(p[1]);
    func_8015785c(p[2]);
    func_8015785c(p[3]);
}
