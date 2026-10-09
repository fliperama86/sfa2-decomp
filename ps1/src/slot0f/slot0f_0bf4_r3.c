/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

long _card_info(long chan);
long _card_clear(long chan);
long _card_load(long chan);
int func_800e1074_slot0f(int *p);
int func_800e10e8_slot0f(int *p);
void func_800e1148_slot0f(int *p);
void func_800e119c_slot0f(int *p);

int func_800e0da8_slot0f(int chan) {
    int *p = data_8018fef8;
    int tries = 0x14;
    int r;
    int i;

    for (;;) {
        if (_card_info(chan) != 1) {
            continue;
        }
        r = func_800e1074_slot0f(p);
        func_800e1148_slot0f(p);
        tries--;
        if (r != 1 && r != 2) {
            tries = 0;
        }
        if (tries == 0) {
            break;
        }
    }
    if (r == 1) {
        return 3;
    }
    if (r == 2) {
        return 1;
    }
    if (r == 3) {
        tries = 0x14;
        for (;;) {
            func_800e119c_slot0f(p);
poll:
            if (_card_clear(chan) != 1) {
                goto poll;
            }
            r = func_800e10e8_slot0f(p);
            tries--;
            if (r != 1 && r != 2) {
                tries = 0;
            }
            if (tries == 0) {
                break;
            }
        }
        if (r == 1) {
            return 3;
        }
        if (r == 2) {
            return 1;
        }
    }
    tries = 0x14;
    for (;;) {
        if (_card_load(chan) != 1) {
            continue;
        }
        for (i = 0x48; i != 0; i--) {
            func_8015fb30(0);
        }
        r = func_800e1074_slot0f(p);
        func_800e1148_slot0f(p);
        tries--;
        if (r != 1 && r != 2) {
            tries = 0;
        }
        if (tries == 0) {
            break;
        }
    }
    if (r == 3) {
        return 4;
    }
    if (r != 1) {
        return r == 2;
    }
    return 3;
}
