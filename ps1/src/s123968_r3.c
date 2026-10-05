/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


extern HudBig *data_8018f5a0;

void func_8012411c(void) {
    func_80125dc0(0, 0x20, 8, 0);
    func_80125dc0(1, 0x20, 8, 0);
    func_80125dc0(2, 0x20, 8, 0);
    func_80125dc0(3, 0x20, 8, 0);
    func_80125dc0(4, 0x11, 8, 0);
    func_801260ac(4, 0xf, 0x11);
    func_80125dc0(4, 1, 8, 0x18);
    func_80125dc0(4, 1, 8, 0x1c);
    func_80137b10();
}

void func_801241d8(void) {
    table_8016e804[data_8018f5a0->field_52]();
}
