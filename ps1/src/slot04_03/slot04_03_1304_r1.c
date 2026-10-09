/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s32 data_801c05e0_slot04_03[];
void func_801b1304_slot04_03(Object *obj);

void func_801b1304_slot04_03(Object *obj) {
    int a;
    int v;

    obj->field_07++;
    obj->field_17b = 1;
    obj->field_45 = 1;
    func_80141f28(obj, 9);
    func_80138ae8(&game_state, obj);
    func_801204f4(obj, obj->side, 5);
    func_801204f4(obj, obj->side, 0xd);
    v = obj->field_0b;
    if (v != 0) {
        v = 0x10000;
    } else {
        v = 0xfffe0000;
    }
    obj->field_4c = v | 0x8000;
    obj->field_58 = 0x7800;
    obj->field_50 = data_801c05e0_slot04_03[obj->field_12a >> 1];
    a = 0x25;
    if (obj->field_49 != 0) {
        a = 0x46;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + a);
}
