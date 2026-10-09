/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3e00_slot04_0a(Object *obj);

extern s32 data_801c0608_slot04_0a[];

void func_801b1750_slot04_0a(Object *obj) {
    s32 v;
    s32 a;

    obj->field_07++;
    func_80141f28(obj, 5);
    func_80138ae8(&game_state, obj);
    obj->field_45 = 1;
    obj->field_17b = 1;
    v = data_801c0608_slot04_0a[obj->field_12a >> 1];
    a = -0x2000;
    if (obj->field_0b == 0) {
        v = -v;
        a = 0x2000;
    }
    obj->field_4c = v;
    obj->field_50 = 0x20000;
    obj->field_54 = a;
    obj->field_58 = -0x3000;
    if (obj->field_49 == 0) {
        func_801307e0(obj, 0x1f);
    } else {
        func_801307e0(obj, 0x50);
    }
}

void func_801b1810_slot04_0a(Object *obj) {
    if (((Slot04aObj *)obj)->field_3a == 0) {
        func_801b3e00_slot04_0a(obj);
        if (obj->pos_y > obj->field_70) {
            obj->field_14 = 0;
            obj->field_45 = 0;
            obj->field_17b = 0;
            obj->field_07++;
            obj->pos_y = obj->field_70;
        }
    }
    func_80130efc(obj);
}
