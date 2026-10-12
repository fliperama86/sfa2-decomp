/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012a1e4(Object *object) {
    u16 saved;
    if (game_state.field_4d != 0) {
        func_8012ff80(object);
    } else if (func_8012f56c(object)) {
        func_8012f59c(object);
    } else if (object->mode_flag_2 != 0) {
        if ((u8)func_8012f898(object)) {
            func_8012f8c4(object);
        } else if ((s16)object->step_flags < 0) {
            func_80131468(object);
        } else {
            update_sequence(object);
        }
    } else if (func_80130470(object)) {
        func_8013047c(object);
    } else if ((u8)func_80130258(object)) {
        func_80130280(object);
    } else if (func_8012f970(object)) {
        func_8012fe60(object);
    } else if ((u8)func_8012f618(object)) {
        if ((u8)func_8012f898(object)) {
            func_8012f8c4(object);
        } else if ((s16)object->step_flags < 0) {
            func_80131468(object);
        } else {
            update_sequence(object);
        }
    } else if ((u8)func_8012f7c0(object)) {
        func_8012f838(object);
    } else {
        saved = object->step_timer;
        object->state_3++;
        set_sequence_by_side(object, (u8)object->step_flags + 4);
        object->step_timer = saved;
    }
}
