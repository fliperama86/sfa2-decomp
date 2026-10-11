/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4a60_slot04_07(Object *obj) {
    obj->field_159 = 1;
    obj->state_3 = obj->state_3 + 1;
    obj->field_0b = obj->field_158;
    if (obj->field_12a != 4) {
        func_80130dc0(obj);
    } else if (obj->field_219 == 0) {
        obj->state_3 = 2;
        play_sound_at(obj, obj->side, 0x324);
        obj->vel_x = 0x90000;
        obj->accel_x = -0x8000;
        func_80130dc0(obj);
    } else {
        obj->state_3 = 3;
        obj->field_157 = 0;
        func_80141f28(obj, 2);
        func_801307e0(obj, 0x2d);
    }
}
