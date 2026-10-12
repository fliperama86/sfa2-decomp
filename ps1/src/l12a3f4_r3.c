/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void update_airborne_object(Object *object) {
    if ((u8)func_80130184(object)) {
        if (object->mode_flag_2 != 0) {
            func_8012ab5c(object);
        } else if (func_80130470(object)) {
            func_80130304(object);
        } else if ((u8)func_8012f2b8(object)) {
            func_8012f384(object);
        } else if (func_8012f970(object)) {
            func_8012f3e0(object);
        } else {
            update_sequence(object);
        }
    } else {
        object->pos_y = object->ground_y;
        object->active_state = 0;
        object->active_flag_3 = 0;
        *(u32 *)&object->pos_y_frac &= 0xffff0000;
        func_801209c4(object);
        if (object->mode_flag_2 == 0) {
            if (object->kind == 0xe && (u8)func_801320e8(object)) {
                func_80132140(object);
            } else if (func_80130470(object)) {
                func_8013047c(object);
            } else if (func_8012f970(object)) {
                func_8012fe60(object);
            } else if ((u8)func_8012f898(object)) {
                func_8012f8c4(object);
            } else {
                object->state_3 = object->state_3 + 1;
                set_sequence_by_side(object, 0x11);
            }
        } else {
            object->state_3 = object->state_3 + 1;
            set_sequence_by_side(object, 0x11);
        }
    }
}
