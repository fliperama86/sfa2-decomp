/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_80048ccc_slot28[][2];
extern s32 data_80048cf4_slot28[][2];
void func_80022f04_slot28(Object *obj, int idx);

void func_80022e4c_slot28(Object *obj) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    u8 unused[16];
    int k = 0;
    obj->field_0c = 0;
    obj->field_0e = 0;
    obj->field_0b = 0;
    obj->field_48 = 0;
    obj->field_04++;
    obj->pos_x = data_80048ccc_slot28[obj->field_03][0];
    obj->pos_y = data_80048ccc_slot28[obj->field_03][1];
    if (obj->field_03 != 0) {
        int j = obj->field_03 - 1;
        obj->field_4c = data_80048cf4_slot28[j][0];
        obj->field_50 = data_80048cf4_slot28[j][1];
        k = 3;
    }
    func_80022f04_slot28(obj, k);
}

void func_80022f04_slot28(Object *obj, int idx) {
    func_80130768(obj, idx, (SequenceStep **)obj->box_tables);
}
