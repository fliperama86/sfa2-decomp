/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c77d8_slot04_02[];
extern ObjectFn data_801c77ec_slot04_02[];
void set_object_states_0_to_3_3_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);
void set_sequence_a_by_sound_side_slot04_02(Object *obj, u8 a);

void toggle_image_row_call_handler_by_state_1_slot04_02(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    if (o->table_index >= 6) {
        o->countdown_2 ^= 1;
        o->image_row = o->countdown_2 + obj->field_b0;
    }
    data_801c77d8_slot04_02[o->state_1](o);
}

void call_handler_by_table_index_and_advance_slot04_02(Object *o) {
    if ((game_state.field_65 | game_state.field_a8) == 0) {
        data_801c77ec_slot04_02[o->table_index >> 1](o);
        advance_sequence_checked(o);
        set_state_0_if_x_outside_margin_narrow(o);
    }
    func_8011ffdc(o);
}

void apply_velocity_with_inverted_y_slot04_02(Object *o) {
    *(s32 *)&o->pos_x_frac += o->vel_x;
    *(s32 *)&o->pos_y_frac -= o->vel_y;
}

void move_until_near_ground_then_land_slot04_02(Object *obj) {
    if (obj->pos_y >= (s16)(obj->ground_y - 0x10)) {
        set_object_states_0_to_3_3_slot04_02(obj, 2, 0, 0, 0);
        obj->pos_y = obj->ground_y;
        set_sequence_a_by_sound_side_slot04_02(obj, 0x10);
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    }
}
