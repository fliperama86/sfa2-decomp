/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c7820_slot04_02[];
void set_sequence_a_by_sound_side_slot04_02(Object *obj, u8 a);
extern s32 data_801c7830_slot04_02[];
void set_object_states_0_to_3_3_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

void advance_state_2_flip_facing_table_image_row_slot04_02(Object *obj) {
    u8 a;

    obj->countdown = 2;
    obj->state_2++;
    obj->facing_side ^= 1;
    call_kind_9_handler_for_player(obj);
    a = data_801c7820_slot04_02[obj->vel_table_index];
    if (a == 0x1d || a == 0x1e || a == 0x1f) {
        obj->image_row++;
    }
    if (a != 0) {
        set_sequence_a_by_sound_side_slot04_02(obj, a);
    }
}

void count_down_then_reverse_vel_x_set_vel_y_slot04_02(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown < 0) {
        obj->active_count = 1;
        set_object_states_0_to_3_3_slot04_02(obj, 1, 0, 0, 0);
        obj->vel_x = -obj->vel_x;
        obj->vel_y = data_801c7830_slot04_02[obj->vel_table_index];
    }
    advance_sequence_checked(obj);
}
