/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_8012d4e4(Object *object) {
    if ((u8)func_80130184(object)) {
        func_801376b8(object);
        if (object->hit_type == 0x13) {
            ref_other.p = object->other;
            if (ref_other.p->field_7e == 0) {
                update_sequence(object);
            } else if (object->vel_y >= 0) {
                update_sequence(object);
            }
        } else if (object->hit_type == 0x11) {
            update_sequence(object);
        } else if (object->vel_y < 0) {
            ref_other.p = object->other;
            if (ref_other.p->field_7e == 0) {
                if ((s16)object->health < 0 || object->field_15b != 0) {
                    update_sequence(object);
                } else {
                    object->state_3 = 6;
                    if (object->field_2a3) {
                        object->field_2a3 = 0;
                    }
                    func_801308c4(object, 0x18);
                }
            }
        }
    } else {
        object->state_3 = object->state_3 + 1;
        if ((s16)object->health < 0) {
            func_80120a30(object, 0, 1);
        } else {
            func_80120a30(object, 0, 0);
        }
        object->active_state = 0;
        object->countdown = 4;
        object->pos_y = (u16)object->ground_y;
        func_80131df8(object);
        set_side_flag(object);
        object->once_flag_2 = 0;
        object->done_flag = 0;
        object->flag_25a_set = 0;
        func_8012d7e4(object);
    }
}

void func_8012d680(Object *object) {
    int v = object->countdown - 1;
    object->countdown = v;
    if ((s16)v != 0) {
        if (object->hit_type == 0x13) {
            ref_other.p = object->other;
            if (ref_other.p->field_7e == 0) {
                update_sequence(object);
            } else if (object->vel_y >= 0) {
                update_sequence(object);
            }
        } else if (object->hit_type == 0x11) {
            update_sequence(object);
        } else if (object->vel_y < 0) {
            ref_other.p = object->other;
            if (ref_other.p->field_7e == 0) {
                if ((s16)object->health < 0 || object->field_15b != 0) {
                    update_sequence(object);
                } else {
                    object->state_3 = 6;
                    if (object->field_2a3) {
                        object->field_2a3 = 0;
                    }
                    func_801308c4(object, 0x18);
                }
            }
        }
    } else {
        object->active_state = 0xff;
        object->state_3 = object->state_3 + 1;
        func_8012ef24(object);
        func_801308c4(object, 0x15);
    }
}
