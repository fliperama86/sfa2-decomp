/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_801c5758_slot04_06;

void follow_parent_position_and_sequence_slot04_06(Object *obj);
void set_variant_sequence_by_sound_side_slot04_06(Object *obj, int a);
void increment_state_0_slot04_06(Object *obj);

void init_object_from_global_object_slot04_06(Object *obj) {
    Object *p;

    obj->state_0++;
    p = data_801c5758_slot04_06;
    obj->field_1c = p->field_1c;
    obj->cycle_value = p->cycle_value;
    obj->table_index = p->kind;
    obj->field_0c = data_801c5758_slot04_06->field_0c;
    obj->image_row = data_801c5758_slot04_06->image_row;
    obj->field_0e = data_801c5758_slot04_06->field_0e;
    obj->variant_index = 0;
    follow_parent_position_and_sequence_slot04_06(obj);
}

void follow_parent_position_and_sequence_slot04_06(Object *obj) {
    Object *p;
    u8 a;

    obj->field_01 = 0;
    p = data_801c5758_slot04_06;
    if (obj->table_index == p->kind) {
        a = p->frame->frame_param;
        if (a == 0) {
            obj->variant_index = 0;
        } else {
            obj->pos_x = p->pos_x;
            obj->pos_y = p->pos_y;
            obj->facing_side = p->facing_side;
            obj->field_01 = 1;
            if (obj->variant_index != a) {
                set_variant_sequence_by_sound_side_slot04_06(obj, a);
            } else {
                advance_sequence_checked(obj);
            }
        }
    } else {
        increment_state_0_slot04_06(obj);
    }
}
