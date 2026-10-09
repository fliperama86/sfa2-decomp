/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801192bc(int a) {
    ((SndCtl *)data_8018f5a0)->field_02 = a;
    ((SndCtl *)data_8018f5a0)->field_00 = 1;
    func_801577ec(0xff000000);
}

void func_801192f0(void) {
    ((SndCtl *)data_8018f5a0)->field_00 = 0;
    func_801575dc();
    func_801577bc(((SndCtl *)data_8018f5a0)->field_04);
    func_8015786c();
    func_801577ec(0xff000000);
}

void func_80119340(int a) {
    u16 *p = (u16 *)(0x801fc200 + a * 0x80);
    if (*p != 0) {
        *p = 0;
        func_801575dc();
        func_801577bc(*(u32 *)(0x801fc204 + a * 0x80));
        func_8015786c();
    }
}

void func_801193a4(u32 a) {
    ((SndCtl *)data_8018f5a0)->field_00 = 3;
    ((SndCtl *)data_8018f5a0)->field_0c = a;
    func_801575dc();
    func_801577bc(((SndCtl *)data_8018f5a0)->field_04);
    func_8015786c();
    func_801577ec(0xff000000);
}
