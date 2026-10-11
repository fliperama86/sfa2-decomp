/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8007632c_slot00(Object *obj) {
    obj->state_3++;
    obj->field_159 = 1;
    obj->field_0b = obj->field_158;
    if (obj->field_219 != 0) {
        obj->state_3 = 2;
        obj->field_225 = 1;
        if (obj->field_246 == 0) {
            obj->field_225 = 0;
            func_80141f28(obj, 1);
        }
        play_side_sound_at(obj, obj->side, 9);
        func_801307e0(obj, 0x2b);
    } else {
        func_80130dc0(obj);
    }
}

void func_800763c4_slot00(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        func_80131468(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}
