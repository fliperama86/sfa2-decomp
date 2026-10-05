/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern unsigned data_801831e8;

void func_8016a730(unsigned char a, unsigned char b, unsigned char c) {
    PadReq req;
    if (a == 0) {
        if (b == 0) {
            req.field_00 = 0x200;
            req.field_18 = c;
        }
        if (b == 1) {
            req.field_00 = 0x100;
            req.field_14 = c;
        }
    }
    if (a == 1) {
        if (b == 0) {
            req.field_00 = 0x2000;
            req.field_24 = c;
        }
        if (b == a) {
            req.field_00 = 0x1000;
            req.field_20 = c;
        }
    }
    func_8016b364(&req);
}

void func_8016a7c4(void) {
    func_8016cd84(1);
}

int func_8016a7e4(unsigned a) {
    int i = -1;
    int j;
    unsigned short c;
    for (j = 0; j < 24; j++) {
        if (a & (1 << j)) {
            i = j;
            break;
        }
    }
    if (i == -1) {
        return -1;
    }
    c = data_80183138[i].field_0c;
    if (data_801831e8 & (1 << i)) {
        if (c != 0) {
            return 1;
        }
        return 3;
    }
    return (c != 0) << 1;
}
