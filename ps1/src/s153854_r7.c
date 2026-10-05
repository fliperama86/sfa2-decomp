/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_801542e4(Object *o) {
}

void func_801542ec(void) {
    s32 *flag = (s32 *)0x1f800000;
    u8 *d = data_8018fef8;
    if (*flag < 0) {
        func_801544a8(d);
    }
    func_80154364(d);
    if (*flag == 0) {
        func_8016d8e0(1);
        func_8016d8f0();
        func_8016d900();
    }
}
