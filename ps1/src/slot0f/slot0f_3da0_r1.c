/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern TextBuf data_800e9750_slot0f;
extern TextBuf data_800e9938_slot0f;
extern TextBuf data_800e9948_slot0f;
extern TextBuf data_800e9958_slot0f;
extern TextBuf data_800e9968_slot0f;
extern TextBuf data_800e9978_slot0f;
extern TextBuf data_800e9988_slot0f;
extern u8 data_800e9783_slot0f[];
extern u8 data_800e981b_slot0f[];
extern u8 data_800e98e2_slot0f[];
extern u8 data_800e98e3_slot0f[];
extern u8 data_800e990e_slot0f[];
extern u8 data_800e990f_slot0f[];
extern u8 data_800e9998_slot0f[];
extern u8 data_800f8564_slot0f[2][8];
extern u8 data_800f856c_slot0f[];
extern u8 data_800f024c_slot0f;
extern u8 data_800f0250_slot0f;
extern u8 data_800f0254_slot0f;
extern u8 data_800f0258_slot0f;
extern u8 data_8016e674[];
extern u8 data_8016e67c[];

void func_800e3da0_slot0f(void) {
    u16 *pad;
    u8 key;
    u8 idx;
    u8 i;
    u8 j;

    func_801519b4(&data_800e9750_slot0f);
    func_801519b4(&data_800e9938_slot0f);
    func_801519b4(&data_800e9948_slot0f);
    func_801519b4(&data_800e9958_slot0f);
    func_801519b4(&data_800e9968_slot0f);
    func_801519b4(&data_800e9978_slot0f);
    func_801519b4(&data_800e9988_slot0f);
    pad = &data_801a696a;
    if (*pad & 0x800) {
        data_8018f5a0->field_52 = 3;
    }
    if (data_801a6976 & 0x800) {
        data_8018f5a0->field_52 = 3;
    }
    if (*pad & 0x4000) {
        data_800e98e3_slot0f[data_800f024c_slot0f * 5] = data_8016e674[data_800f024c_slot0f];
        data_800e9783_slot0f[data_800f024c_slot0f * 14] = 0x1b;
        data_800f024c_slot0f = (data_800f024c_slot0f + 1) & 7;
        data_800e9783_slot0f[data_800f024c_slot0f * 14] = 0x10;
        func_80120554(0, 0, 0x34d);
    }
    if (*pad & 0x1000) {
        data_800e98e3_slot0f[data_800f024c_slot0f * 5] = data_8016e674[data_800f024c_slot0f];
        data_800e9783_slot0f[data_800f024c_slot0f * 14] = 0x1b;
        data_800f024c_slot0f = (data_800f024c_slot0f + 255) & 7;
        data_800e9783_slot0f[data_800f024c_slot0f * 14] = 0x10;
        func_80120554(0, 0, 0x34d);
    }
    if ((*pad & 0xff) != 0) {
        if (*pad & 0x80) { key = 0x90; idx = 0; }
        if (*pad & 0x10) { key = 0x92; idx = 1; }
        if (*pad & 0x4) { key = 0x95; idx = 2; }
        if (*pad & 0x40) { key = 0x93; idx = 3; }
        if (*pad & 0x20) { key = 0x91; idx = 4; }
        if (*pad & 0x8) { key = 0x94; idx = 5; }
        if (*pad & 0x1) { key = 0x97; idx = 6; }
        if (*pad & 0x2) { key = 0x96; idx = 7; }
        for (i = 0; i < 8; i++) {
            if (key == data_8016e674[i]) {
                data_800f0254_slot0f = i;
            }
            if (data_800e9998_slot0f[data_800f024c_slot0f] == ((u8 *)data_800f8564_slot0f)[i]) {
                j = i;
            }
        }
        if (key == 0x94 || key == 0x95 || key == 0x96 || key == 0x97) {
            data_800e98e2_slot0f[data_800f024c_slot0f * 5] = 0x1a;
        } else {
            data_800e98e2_slot0f[data_800f024c_slot0f * 5] = 0x1d;
        }
        if (data_8016e674[data_800f024c_slot0f] == 0x94 || data_8016e674[data_800f024c_slot0f] == 0x95 || data_8016e674[data_800f024c_slot0f] == 0x96 || data_8016e674[data_800f024c_slot0f] == 0x97) {
            data_800e98e2_slot0f[data_800f0254_slot0f * 5] = 0x1a;
        } else {
            data_800e98e2_slot0f[data_800f0254_slot0f * 5] = 0x1d;
        }
        data_8016e674[data_800f0254_slot0f] = data_8016e674[data_800f024c_slot0f];
        data_8016e674[data_800f024c_slot0f] = key;
        key = ((u8 *)data_800f8564_slot0f)[idx];
        ((u8 *)data_800f8564_slot0f)[idx] = data_800e9998_slot0f[data_800f024c_slot0f];
        ((u8 *)data_800f8564_slot0f)[j] = key;
        data_800e98e3_slot0f[data_800f0254_slot0f * 5] = data_8016e674[data_800f0254_slot0f];
        data_800e98e3_slot0f[data_800f024c_slot0f * 5] = data_8016e674[data_800f024c_slot0f];
        data_800f0254_slot0f = data_800f024c_slot0f;
        func_80120554(0, 0, 0x34c);
    }
    pad = &data_801a6976;
    if (*pad & 0x4000) {
        data_800e990f_slot0f[data_800f0250_slot0f * 5] = data_8016e67c[data_800f0250_slot0f];
        data_800e981b_slot0f[data_800f0250_slot0f * 14] = 0x1b;
        data_800f0250_slot0f = (data_800f0250_slot0f + 1) & 7;
        data_800e981b_slot0f[data_800f0250_slot0f * 14] = 0x10;
        func_80120554(0, 0, 0x34d);
    }
    if (*pad & 0x1000) {
        data_800e990f_slot0f[data_800f0250_slot0f * 5] = data_8016e67c[data_800f0250_slot0f];
        data_800e981b_slot0f[data_800f0250_slot0f * 14] = 0x1b;
        data_800f0250_slot0f = (data_800f0250_slot0f + 255) & 7;
        data_800e981b_slot0f[data_800f0250_slot0f * 14] = 0x10;
        func_80120554(0, 0, 0x34d);
    }
    if ((*pad & 0xff) != 0) {
        if (*pad & 0x80) { key = 0x90; idx = 0; }
        if (*pad & 0x10) { key = 0x92; idx = 1; }
        if (*pad & 0x4) { key = 0x95; idx = 2; }
        if (*pad & 0x40) { key = 0x93; idx = 3; }
        if (*pad & 0x20) { key = 0x91; idx = 4; }
        if (*pad & 0x8) { key = 0x94; idx = 5; }
        if (*pad & 0x1) { key = 0x97; idx = 6; }
        if (*pad & 0x2) { key = 0x96; idx = 7; }
        for (i = 0; i < 8; i++) {
            if (key == data_8016e67c[i]) {
                data_800f0258_slot0f = i;
            }
            if (data_800e9998_slot0f[data_800f0250_slot0f] == data_800f856c_slot0f[i]) {
                j = i;
            }
        }
        if (key == 0x94 || key == 0x95 || key == 0x96 || key == 0x97) {
            data_800e990e_slot0f[data_800f0250_slot0f * 5] = 0x1a;
        } else {
            data_800e990e_slot0f[data_800f0250_slot0f * 5] = 0x1d;
        }
        if (data_8016e67c[data_800f0250_slot0f] == 0x94 || data_8016e67c[data_800f0250_slot0f] == 0x95 || data_8016e67c[data_800f0250_slot0f] == 0x96 || data_8016e67c[data_800f0250_slot0f] == 0x97) {
            data_800e990e_slot0f[data_800f0258_slot0f * 5] = 0x1a;
        } else {
            data_800e990e_slot0f[data_800f0258_slot0f * 5] = 0x1d;
        }
        data_8016e67c[data_800f0258_slot0f] = data_8016e67c[data_800f0250_slot0f];
        data_8016e67c[data_800f0250_slot0f] = key;
        key = data_800f856c_slot0f[idx];
        data_800f856c_slot0f[idx] = data_800e9998_slot0f[data_800f0250_slot0f];
        data_800f856c_slot0f[j] = key;
        data_800e990f_slot0f[data_800f0258_slot0f * 5] = data_8016e67c[data_800f0258_slot0f];
        data_800e990f_slot0f[data_800f0250_slot0f * 5] = data_8016e67c[data_800f0250_slot0f];
        data_800f0258_slot0f = data_800f0250_slot0f;
        func_80120554(0, 0, 0x34c);
    }
}
