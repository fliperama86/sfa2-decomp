/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s8 data_800f855d_slot0f;
extern u16 data_800e9074_slot0f[];
extern u8 data_800e8808_slot0f[];
extern u8 *data_800e9014_slot0f;
extern u8 *data_800e9024_slot0f;
extern u8 data_800e9008_slot0f[];
extern u8 data_800e9044_slot0f[];

void func_800e2fcc_slot0f(void) {
    u8 *p;
    s8 *q;
    s8 sel;
    u16 pad = data_801a696a | data_801a6976;

    if (pad & 0x2000) {
        q = &data_800f855d_slot0f;
        *q = (*q + 1 < 0x3b) ? *q + 1 : 0;
        q = &data_800f855d_slot0f;
        if (*q == 0x33) {
            *q = 0x34;
        }
    } else if (pad & 0x8000) {
        q = &data_800f855d_slot0f;
        *q = (*q - 1 >= 0) ? *q - 1 : 0x3a;
        q = &data_800f855d_slot0f;
        if (*q == 0x33) {
            *q = 0x32;
        }
    } else if (pad & 1) {
        q = &data_800f855d_slot0f;
        *q = (*q + 10 < 0x3b) ? *q + 10 : 0;
        q = &data_800f855d_slot0f;
        if (*q == 0x33) {
            *q = 0x34;
        }
    } else if (pad & 2) {
        q = &data_800f855d_slot0f;
        *q = (*q - 10 >= 0) ? *q - 10 : 0x3a;
        q = &data_800f855d_slot0f;
        if (*q == 0x33) {
            *q = 0x32;
        }
    } else if (pad & 0x20) {
        func_8014f4d4(1, data_800e9074_slot0f[data_800f855d_slot0f]);
    } else if (pad & 0x40) {
        func_8014f4d4(6, 2);
    } else {
        goto skip;
    }
    sel = data_800f855d_slot0f;
    data_800e9014_slot0f = data_800e8808_slot0f + sel * 0x20;
    if (sel == 0x32) {
        data_800e9024_slot0f = data_800e8808_slot0f + 0x660;
    }
skip:
    p = data_800e9008_slot0f;
    func_801519b4(p);
    if (data_800f855d_slot0f == 0x32) {
        func_801519b4(p + 0x10);
    }
    p = data_800e9044_slot0f;
    func_801519b4(p);
    func_801519b4(p + 0x10);
    func_801519b4(p + 0x20);
}
