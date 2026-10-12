/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern int data_8002d57c_slot12;
void func_8001129c_slot12(void);
int func_80012d90_slot12(u8 a);

void func_80010550_slot12(void) {
    while (data_8018db10) {
        func_80138164();
        func_8011abe4();
        func_8001129c_slot12();
        func_801192bc(1);
    }
    func_801373e8();
    data_8018f5a0->substep_index++;
}

void func_800105cc_slot12(void) {
    data_8002d57c_slot12 = 1;
    if (func_80012d90_slot12(3)) {
        int n = 4;
        int t;
        do {
            func_80138164();
            func_8011abe4();
            func_8001129c_slot12();
            func_801192bc(1);
            t = n;
            n = n - 1;
        } while (t != 0);
        data_8018f5a0->substep_index++;
        game_state.busy_flag = 0xff;
    }
}
