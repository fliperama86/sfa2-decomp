/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_8007a0d8_slot00[];
extern ObjectFn data_8007a104_slot00[];

void func_800784a4_slot00(Object *obj) {
    if (game_state.field_6a != 0) {
        obj->field_04 = 2;
        obj->field_01 = 0;
    } else {
        if (game_state.field_65 == 0) {
            data_8007a0d8_slot00[obj->field_03 >> 1](obj);
        }
        if (obj->field_67 == 0) {
            func_80120028(obj);
        }
    }
}

void func_8007853c_slot00(Object *obj) {
    Object *p = obj->field_3c;
    if (p->field_01 != 0) {
        obj->field_a4 = obj->field_a4 - 1;
        if (obj->field_a4 & 0x80) {
            obj->field_a4 = 2;
            obj->field_0c = p->field_0c;
            obj->field_0d = p->field_0d;
            obj->field_a5 = obj->field_a5 ^ 1;
            if (obj->field_a5 != 0) {
                obj->field_0c = 0xff;
                obj->field_0d = obj->field_0d + 1;
            }
        }
        data_8007a104_slot00[obj->field_06](obj);
    }
}
