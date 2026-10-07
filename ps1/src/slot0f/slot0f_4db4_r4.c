/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_800ebc08_slot0f[];
extern u16 data_800eb970_slot0f[];
extern u16 data_800f726c_slot0f[16];
extern s16 data_800f728c_slot0f[16];
extern u16 data_800f72ac_slot0f[20][16];
extern u16 data_800f752c_slot0f[20][16];
extern u16 data_800f77ac_slot0f;
extern u16 data_800f77b0_slot0f;
void func_800e52e0_slot0f(void);

void func_800e51c8_slot0f(int a) {
    int i;
    s16 c;
    for (i = 0; i < 16; i++) {
        data_800f726c_slot0f[i] = data_800ebc08_slot0f[a * 16 + i];
    }
    c = 0x8421;
    for (i = 15; i >= 0; i--) {
        data_800f728c_slot0f[i] = c;
    }
    data_800f77ac_slot0f = 0;
    data_800f77b0_slot0f = 0x1f;
    func_800e52e0_slot0f();
}

void func_800e5258_slot0f(void) {
    int j;
    int i;
    u16 *src;
    for (j = 0; j < 20; j++) {
        for (i = 15; i >= 0; i--) {
            data_800f752c_slot0f[j][i] = 0x421;
        }
    }
    src = data_800eb970_slot0f;
    for (j = 0; j < 20; j++) {
        for (i = 0; i < 16; i++) {
            data_800f72ac_slot0f[j][i] = *src++;
        }
    }
}
