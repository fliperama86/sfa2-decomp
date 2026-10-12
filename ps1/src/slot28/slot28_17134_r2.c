/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_80050ca0_slot28[];
void set_random_sequence_from_table_slot28(Object *obj);
void set_random_x_top_position_slot28(Object *obj);
void init_random_vertical_motion_and_facing_slot28(Object *obj);

void init_random_motion_position_and_sequence_slot28(Object *obj) {
    obj->field_0f = 1;
    obj->state_0 = obj->state_0 + 1;
    init_random_vertical_motion_and_facing_slot28(obj);
    set_random_x_top_position_slot28(obj);
    set_random_sequence_from_table_slot28(obj);
}

void set_random_sequence_from_table_slot28(Object *obj) {
    set_sequence_from_table_5(obj, data_80050ca0_slot28[get_random_byte() & 3], (SequenceStep **)obj->box_tables);
}

void update_falling_object_and_respawn_slot28(Object *obj) {
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->pos_y >= 0xb0) {
        init_random_vertical_motion_and_facing_slot28(obj);
        set_random_x_top_position_slot28(obj);
        set_random_sequence_from_table_slot28(obj);
    }
    advance_sequence_checked(obj);
    obj->field_01 = 1;
}
