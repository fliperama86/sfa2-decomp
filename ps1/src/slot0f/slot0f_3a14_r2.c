/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudState *data_8018f5a0;
extern u8 data_800f024c_slot0f;
extern u8 data_800f0250_slot0f;
extern u8 data_800f0254_slot0f;
extern u8 data_800f0258_slot0f;
extern u8 data_800f025c_slot0f;
extern u8 data_800f0260_slot0f;
extern u8 data_800e98e3_slot0f[];
extern u8 data_800e990f_slot0f[];
extern u8 data_800e9783_slot0f[];
extern u8 data_800e981b_slot0f[];

void func_800e3b60_slot0f(void) {
    u8 i;

    ((HudBig *)data_8018f5a0)->field_52++;
    data_800f024c_slot0f = 0;
    data_800f0250_slot0f = 0;
    data_800f0254_slot0f = 0;
    data_800f0258_slot0f = 0;
    data_800f025c_slot0f = 0;
    data_800f0260_slot0f = 0;
    for (i = 0; i < 8; i++) {
        data_800e98e3_slot0f[i * 5] = data_8016e674[i];
        data_800e990f_slot0f[i * 5] = data_8016e67c[i];
    }
    for (i = 0; i < 8; i++) {
        u8 v = data_800e98e3_slot0f[i * 5];

        if (v == 0x94 || v == 0x95 || v == 0x96 || v == 0x97) {
            data_800e98e3_slot0f[i * 5 - 1] = 0x1a;
        } else {
            data_800e98e3_slot0f[i * 5 - 1] = 0x1d;
        }
        v = data_800e990f_slot0f[i * 5];
        if (v == 0x94 || v == 0x95 || v == 0x96 || v == 0x97) {
            data_800e990f_slot0f[i * 5 - 1] = 0x1a;
        } else {
            data_800e990f_slot0f[i * 5 - 1] = 0x1d;
        }
    }
    data_800e9783_slot0f[data_800f024c_slot0f * 14] = 0x10;
    data_800e981b_slot0f[data_800f0250_slot0f * 14] = 0x10;
    for (i = 1; i < 8; i++) {
        data_800e9783_slot0f[i * 14] = 0x1b;
        data_800e981b_slot0f[i * 14] = 0x1b;
    }
}
