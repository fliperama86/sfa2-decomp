/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_8002bc78_slot12;
extern u8 data_8002bc7c_slot12[];
extern u16 data_8002bc80_slot12;
extern u16 data_8002bc82_slot12;
extern u8 data_8002bc84_slot12;
extern u8 data_8002bc85_slot12;
extern u8 data_8002bc86_slot12;
extern u8 data_8002bc87_slot12;
extern u32 data_8002bc88_slot12;
extern u32 data_800284e4_slot12[];

void func_80015910_slot12(Object *obj) {
    u32 v = (u32)data_8002bc7c_slot12;
    if (game_state.field_2bd == 0) {
        *(u32 *)v = 0;
        v = 0x10;
        data_8002bc84_slot12 = v;
        data_8002bc85_slot12 = v;
        data_8002bc87_slot12 = v;
        v = data_800284e4_slot12[0];
    } else {
        *(u32 *)v = 0;
        v = 0x10;
        data_8002bc84_slot12 = v;
        data_8002bc85_slot12 = v;
        data_8002bc87_slot12 = v;
        v = data_800284e4_slot12[1];
    }
    data_8002bc86_slot12 = 2;
    data_8002bc78_slot12 = 1;
    data_8002bc80_slot12 = 0x40;
    data_8002bc82_slot12 = 0x10;
    data_8002bc88_slot12 = v;
}
