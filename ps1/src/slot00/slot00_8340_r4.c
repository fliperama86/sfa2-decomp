/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_8007a110_slot00[];

void update_child_image_row_then_call_state_2_slot00(Object *obj) {
    Object *p = obj->parent;
    obj->field_a4 = obj->field_a4 - 1;
    if (obj->field_a4 & 0x80) {
        obj->field_a4 = 2;
        obj->field_0c = p->field_0c;
        obj->image_row = p->image_row;
        obj->field_a5 = obj->field_a5 ^ 1;
        if (obj->field_a5 != 0) {
            obj->field_0c = 0xff;
            obj->image_row = obj->image_row + 1;
        }
    }
    data_8007a110_slot00[obj->state_2](obj);
}

void set_countdown_ten_and_advance_state_2_slot00(Object *obj) {
    obj->countdown = 0xa;
    obj->state_2 = obj->state_2 + 1;
    advance_sequence_checked(obj);
}

void func_800787c0_slot00(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown < 0) {
        obj->field_60 = 0x13;
        obj->hit_type = 0xf;
        obj->active_state = 0x1d;
        obj->state_2 = obj->state_2 + 1;
    }
    advance_sequence_checked(obj);
}
