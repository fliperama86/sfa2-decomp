/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_800f7d34_slot0f;
extern u8 data_800f7d38_slot0f[];
extern u16 data_800f7d3c_slot0f;
extern u16 data_800f7d3e_slot0f;
extern u8 data_800f7d40_slot0f;
extern u8 data_800f7d41_slot0f;
extern u8 data_800f7d42_slot0f;
extern u8 data_800f7d43_slot0f;
extern u32 data_800f7d44_slot0f;
extern u32 data_800f01c4_slot0f[];

void func_800e79a0_slot0f(Object *obj) {
    u32 v = (u32)data_800f7d38_slot0f;
    if (game_state.field_2bd == 0) {
        *(u32 *)v = 0;
        v = 0x10;
        data_800f7d40_slot0f = v;
        data_800f7d41_slot0f = v;
        data_800f7d43_slot0f = v;
        v = data_800f01c4_slot0f[0];
    } else {
        *(u32 *)v = 0;
        v = 0x10;
        data_800f7d40_slot0f = v;
        data_800f7d41_slot0f = v;
        data_800f7d43_slot0f = v;
        v = data_800f01c4_slot0f[1];
    }
    data_800f7d42_slot0f = 2;
    data_800f7d34_slot0f = 1;
    data_800f7d3c_slot0f = 0x40;
    data_800f7d3e_slot0f = 0x10;
    data_800f7d44_slot0f = v;
}
