/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_801c7844_slot04_11[];
extern ObjectFn data_801c7854_slot04_11[];

void select_box_tables(Object *object);
void build_metrics(Object *object);
void reset_object_to_state_1_0_0_2_sequence_1(Object *object);
void land_reset_fields_and_choose_next_state(Object *object);
void set_sequence_by_side(Object *object, int arg);
unsigned short get_pressed_low_byte(Object *object);

void call_reset_object_to_state_1_0_sequence_0_slot04_11(Object *obj);
Object *get_table_object_by_field_slot04_11(Object *obj);
int has_buttons_94_pressed_or_released_slot04_11(Object *obj);
int has_buttons_68_pressed_or_released_slot04_11(Object *obj);
void copy_kind_table_to_scratch_by_side_slot04_11(Object *obj);

int check_buttons_by_kind_and_flag_slot04_11(Object *obj) {
    int r;

    if (obj->kind == 0x11) {
        if (obj->flags_28b & 1) {
            goto y;
        }
x:
        r = has_buttons_68_pressed_or_released_slot04_11(obj);
        goto done;
    } else if (obj->flags_28b & 1) {
        goto x;
    }
y:
    r = has_buttons_94_pressed_or_released_slot04_11(obj);
done:
    return r;
}

void call_reset_object_to_state_1_0_sequence_0_slot04_11(Object *obj) {
    reset_object_to_state_1_0_sequence_0(obj);
}

void call_reset_object_to_state_1_0_0_2_sequence_1_slot04_11(Object *obj) {
    reset_object_to_state_1_0_0_2_sequence_1(obj);
}

void call_state_3_handler_wrapper_slot04_11(Object *obj) {
    land_reset_fields_and_choose_next_state(obj);
}

int has_buttons_94_pressed_or_released_slot04_11(Object *obj) {
    return ((obj->pressed_bits | obj->released_bits) & 0x94) == 0x94;
}

int has_buttons_68_pressed_or_released_slot04_11(Object *obj) {
    return ((obj->pressed_bits | obj->released_bits) & 0x68) == 0x68;
}

Object *get_table_object_by_field_slot04_11(Object *obj) {
    return data_801c7844_slot04_11[obj->field_0e];
}

void call_handler_by_state_3_for_kinds_slot04_11(Object *obj) {
    if (obj->kind == 0x11 || obj->kind == 0x13) {
        data_801c7854_slot04_11[obj->state_3](obj);
    }
}

void toggle_kind_bit_rebuild_tables_set_sequence_slot04_11(Object *obj) {
    obj->state_3++;
    obj->flags_28b = 0;
    obj->kind ^= 2;
    copy_kind_table_to_scratch_by_side_slot04_11(obj);
    select_box_tables(obj);
    build_metrics(obj);
    set_sequence_by_side(obj, 0x30);
}

void update_sequence_or_switch_on_input_slot04_11(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        reset_object_to_state_1_0_sequence_0(obj);
        return;
    }
    if (obj->mode_flag_2 == 0) {
        if (get_pressed_low_byte(obj) & 0xffff) {
            set_state_1_0_5_and_read_input_direction(obj);
            return;
        }
        if (find_nearby_enemy_and_get_flag(obj)) {
            enter_state_2_6_by_button_input(obj);
            return;
        }
    }
    if ((u8)is_facing_differs_from_saved(obj)) {
        set_win_state_or_state_4_sequence(obj);
    } else {
        update_sequence(obj);
    }
}
