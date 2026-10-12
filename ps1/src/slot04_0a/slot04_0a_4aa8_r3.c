/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4ff0_slot04_0a(Object *obj);
void func_801b4ff8_slot04_0a(Object *obj);

void func_801b4d2c_slot04_0a(Object *obj) {
    obj->ot_depth = 4;
    obj->spawn_mode = 5;
    obj->field_0c = 0xff;
    obj->state_0++;
    obj->field_1c = obj->parent->field_1c;
}

void func_801b4d60_slot04_0a(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown < 0) {
        obj->countdown = 0;
        if (obj->vel_y != ((obj->parent->state_0 << 24) | (obj->parent->state_1 << 16) | (obj->parent->state_2 << 8))) {
            obj->state_0 = 2;
        } else {
            if (obj->step_flags & 0x100) {
                func_801b4ff0_slot04_0a(obj);
            } else if (obj->step_flags & 0x200) {
                func_801b4ff8_slot04_0a(obj);
            }
            func_80120028(obj);
        }
    }
}
