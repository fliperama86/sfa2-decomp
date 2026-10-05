/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

int func_8015c938(int a, u8 *p);

void func_801507e8(Menu *menu) {
    int v = func_8015c938(1, &menu->field_0c);
    if (v != 0) {
        if (v == 2) {
            if (!(menu->field_0c & 0x40) && func_8015c9c0(0x1b, &menu->field_f0, 0)) {
                menu->field_02 = 2;
                if (menu->field_03 == 0) {
                    func_8014f604(0x55);
                }
            }
        } else {
            menu->field_02 = 7;
        }
    }
}

void func_80150878(Menu *menu) {
    menu->field_01 = 1;
    if (menu->field_06 & menu->field_07) {
        if (func_8015c938(1, 0)) {
            if (func_8015c9c0(9, 0, &menu->field_0c)) {
                menu->field_02 = 3;
                if (menu->field_0c & 0x10) {
                    menu->field_08 = 1;
                }
            }
        }
    }
}

void func_801508fc(Menu *menu) {
    if (func_8015c938(1, 0) && menu->field_01 != 0 && menu->field_06 == 0) {
        if (func_8015c9c0(0x1b, 0, &menu->field_0c)) {
            menu->field_07 = 0;
            menu->field_02 = 2;
            if (menu->field_0c & 0x10) {
                menu->field_08 = 1;
            }
        }
    }
}

void func_80150984(Menu *menu) {
    if (func_8015c938(1, 0)) {
        if (func_8015c9c0(9, 0, 0)) {
            func_8016a730(0, 0, 0);
            func_8014f604(0);
            menu->field_01 = 0;
            menu->field_02 = 0;
        }
    }
}

void func_801509ec(Menu *menu) {
    if (func_8015c938(1, 0)) {
        if (func_8015c9c0(0x15, &menu->field_f0, 0)) {
            menu->field_02 = 1;
        }
    }
}

void func_80150a3c(Menu *menu) {
    if (func_8015c938(1, 0)) {
        if (func_8015c9c0(0xd, &menu->field_ec, 0)) {
            menu->field_02 = 5;
        }
    }
}

void func_80150a8c(Menu *menu) {
    if (func_8015c938(1, 0)) {
        u8 buf = 0xc8;
        if (func_8015c9c0(0xe, &buf, &menu->field_0c)) {
            menu->field_02 = 6;
            if (menu->field_0c & 0x10) {
                menu->field_08 = 1;
            }
        }
    }
}

void func_80150afc(Menu *menu) {
    u8 vol = menu->field_05;
    u8 step = menu->field_1d;
    if (menu->field_03 == 1) {
        vol = vol + step;
        if (vol > 0x55) {
            vol = 0x55;
            menu->field_03 = 0;
        }
    } else {
        vol = vol - step;
        if ((s8)vol < 4) {
            vol = 0;
            menu->field_03 = 0;
            menu->field_02 = 4;
        }
    }
    func_8014f604(vol);
}
