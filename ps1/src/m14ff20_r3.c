/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8015cec4(u32 a, u8 *p);

void func_80150b80(Menu *menu, int a) {
    int lo = a & 0xff;
    int hi = (a & 0xff00) >> 8;
    menu->field_ec = 1;
    menu->field_18 = a;
    menu->field_ed = lo;
    func_80150c6c(menu, data_8017eb34[hi]);
    menu->field_28 = menu->field_2c = lo + menu->field_28;
    menu->field_2c = (data_8017eb44[hi * 8 + lo] << 3) + menu->field_2c;
    func_8015cec4(menu->field_28, &menu->field_f0);
    func_8014f604(0);
    func_8016a730(0, 0, 1);
    func_8016904c(0, 0x55, 0x55);
    menu->field_00 = 2;
    menu->field_02 = 7;
}
