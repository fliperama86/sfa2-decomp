/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_choice_from_pressed_buttons_slot27(void);
void add_y_offset_to_clamped_health_slot27(Object *obj);
void sync_vec_table_index_from_field_slot27(Object *obj);
void check_opposite_player_same_selection_slot27(void);
void advance_palette_animation_and_upload_slot27(Object *obj);

void update_two_choice_selector_state_slot27(Object *obj) {
    u8 v;
    update_choice_from_pressed_buttons_slot27();
    if (ref_other.p->field_d8 != data_8019045c[0]) {
        call_func_with_side_x_origin(ref_other.p->side, 0);
        data_80190464[0] = 2;
        ref_other.p->field_d8 = v = data_8019045c[0];
        if (v == 0) {
            data_80190464[0] = -data_80190464[0];
        }
        obj->y_offset = data_80190464[0];
    }
    add_y_offset_to_clamped_health_slot27(obj);
    if (game_state.busy_flag == 0) {
        int m;
        int t;
        s16 n = obj->countdown;
        n -= 1;
        obj->countdown = n;
        if (n != 0) {
            m = ~ref_other.p->prev_input_bits;
            data_8019045c[0] = m;
            t = ref_other.p->input_bits & m;
            data_8019045c[0] = t & 0xf0;
            if (data_8019045c[0] == 0) {
                goto call;
            }
        }
        obj->state_2 = obj->state_2 + 1;
        call_func_with_side_x_origin(ref_other.p->side, 1);
        data_8019045c[0] = ref_other.p->field_d8;
        if (ref_other.p->field_d8 != 0) {
            ref_other.p->field_d4 = ref_other.p->field_d4 & 1;
            ref_other.p->field_d4 = ref_other.p->field_d4 + 4;
            check_opposite_player_same_selection_slot27();
            if (data_8019045c[0] != 0) {
                ref_other.p->field_d4 = ref_other.p->field_d4 ^ data_8019045c[0];
            }
        }
        data_8019045c[0] = ref_other.p->field_d8 << 4;
        obj->health = data_8019045c[0];
        sync_vec_table_index_from_field_slot27(obj);
        return;
    }
call:
    advance_palette_animation_and_upload_slot27(obj);
}
