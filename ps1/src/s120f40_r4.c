/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_80121a64(Object *object) {
    if ((u8)func_80125268() != 0) {
        data_8018f5a0->step_index_2 += 2;
        object->phase_state = 1;
    } else {
        int n = data_8018f5a0->countdown_timer - 1;
        data_8018f5a0->countdown_timer = n;
        if ((s16)n < 0) {
            data_8018f5a0->countdown_timer = 0x3f;
            data_8018f5a0->step_index_2++;
            func_80121b00(object);
        }
    }
}

void func_80121b00(Object *object) {
    func_80125dc0(0, 4, 0, 0x15);
    func_80125dc0(0, 4, 0, 0x15);
    func_80125dc0(0, 7, 0x10, 0);
    func_80125dc0(1, 0x20, 0x10, 0);
    func_80125dc0(2, 0x20, 0x10, 0);
    func_80125dc0(3, 0x20, 0x10, 0);
    func_80137220(0, 6);
    func_80137220(1, 0);
    func_80137220(2, 1);
    func_80137220(3, 2);
}

void func_80121bc0(Object *object) {
    int n;
    if ((u8)func_80125268() != 0 || (n = data_8018f5a0->countdown_timer - 1, data_8018f5a0->countdown_timer = n, (s16)n < 0)) {
        data_8018f5a0->step_index_2++;
        data_80190a40 = 1;
        func_801373e8();
        object->phase_state = 1;
    }
}

void func_80121c4c(Object *unused) {
    data_8018f5a0->step_index_2++;
    func_801374c0();
    func_80137564();
}

void func_80121c88(Object *object) {
    data_8018f5a0->step_index_2++;
    data_80190a40 = 0;
    func_801374c0();
    data_8018f5a0->countdown_timer = 0x78;
    object->team_id = 0;
}

void func_80121ce0(Object *object) {
    int n;
    if ((u8)func_80125268() != 0) {
        func_8014f4d4(6, 3);
    }
    if ((u8)func_80125268() != 0 || (n = data_8018f5a0->countdown_timer - 1, data_8018f5a0->countdown_timer = n, (s16)n < 0)) {
        data_8018f5a0->step_index_2++;
        data_8018f5a0->countdown_timer = 0x1c;
        object->phase_state = 2;
    }
}
