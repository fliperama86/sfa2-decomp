/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Pair table_80028148_slot12[];
void func_80014a38_slot12(Object *obj);
void func_80014dd0_slot12(Object *obj, int a);

void func_800148f4_slot12(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[8];
    FrameRecord *rec;
    int k;
    obj->field_09 = 3;
    obj->field_01 = 1;
    obj->field_50 = 0x70000;
    obj->field_54 = 0x500;
    obj->field_58 = -0x5000;
    obj->field_4c = 0xfffa7000;
    obj->field_0b = 0;
    obj->field_04 = obj->field_04 + 1;
    if (obj->field_03 == 0) {
        k = game_state.field_2b8;
        rec = table_8016e5c4 + k;
    } else {
        k = game_state.field_2ba;
        rec = table_8016e614 + k;
    }
    obj->field_7a = 0x60;
    obj->field_7c = 0x1f0;
    obj->field_0d = 0;
    obj->field_60 = 0;
    obj->field_60 = rec->field_08;
    obj->field_61 = rec->field_09;
    obj->field_0c = 0xff;
    obj->field_98 = table_80028148_slot12[obj->field_60].first;
    obj->field_9c = table_80028148_slot12[obj->field_60].second;
    func_80014a38_slot12(obj);
    func_80014dd0_slot12(obj, 0);
    obj->field_01 = 0;
    obj->pos_x += 0x60;
    obj->pos_y -= 0x48;
}
