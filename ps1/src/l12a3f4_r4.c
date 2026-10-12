/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012b184(Object *object) {
    u16 saved;
    if (object->mode_flag_2 != 0) {
        func_8012b2c4(object);
    } else if (func_80130470(object)) {
        func_8013047c(object);
    } else if ((u8)func_80130258(object)) {
        func_80130280(object);
    } else if ((u8)func_8012f7c0(object)) {
        func_8012f838(object);
    } else if ((u8)func_8012f618(object)) {
        object->state_3 = object->state_3 + 1;
        saved = object->step_timer;
        set_sequence_by_side(object, (u8)object->step_flags + 0xd);
        object->step_timer = saved;
    } else if (func_8012f970(object)) {
        func_8012fe60(object);
    } else if ((s16)object->step_flags < 0) {
        func_801312b8(object);
    } else {
        object->facing_side = object->saved_facing;
        update_sequence(object);
    }
}
