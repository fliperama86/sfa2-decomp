/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

u8 func_801b5ef0_slot04_02(Object *obj);
void func_801b5bf4_slot04_02(Object *obj);
u8 func_801b5eb8_slot04_02(Object *obj);

void func_801b5ce8_slot04_02(Object *obj) {
    Object *o = obj->other;
    int t;
    obj->field_07 = obj->field_07 + 1;
    func_80141f28(obj, 3);
    func_80120554(o, o->side, 0x31a);
    obj->field_0b = 0;
    obj->field_4c = 0x48000;
    if (obj->field_cd != 0) {
        t = func_801b5ef0_slot04_02(obj);
    } else {
        t = obj->field_c2 & 0x8000;
    }
    if (t) {
        obj->field_4c = -0x48000;
        obj->field_0b = obj->field_0b + 1;
    }
    obj->field_50 = 0x20000;
    obj->field_58 = -0x2000;
    func_801307e0(obj, 0x45);
}

void func_801b5da8_slot04_02(Object *o) {
    if (*(u8 *)&o->field_3a == 0) {
        o->field_45 = 1;
        o->field_07++;
    }
    func_80130efc(o);
}

void func_801b5de8_slot04_02(Object *obj) {
    if (func_801b5eb8_slot04_02(obj)) {
        if (obj->field_70 <= obj->pos_y) {
            obj->pos_y = obj->field_70;
            game_state.field_63 = 0x18;
        }
    }
    func_801b5bf4_slot04_02(obj);
}
