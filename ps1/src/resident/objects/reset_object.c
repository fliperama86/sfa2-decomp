/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../externs.h"
#include "../../protos.h"


/* Used when both players hold one of three character pairings. */
/* Second name for shared_handler_17. The original loads that address in two
   separate blocks; with a single name this compiler merges them. */

void reset_object(Object *object) {
    ObjectFn handler;
    int i;
    /* The original stores this zeroed local, not a constant, then reuses it. */
    int value = 0;

    object->active_state = 0;
    object->status_flag = 0;
    object->field_157 = 0;
    object->accum_count = 0;
    object->countdown_timer_2 = 0;
    object->field_163 = 0;
    object->active_flag_3 = 0;
    object->field_15b = 0;
    object->field_15c = 0;
    object->field_167 = 0;
    object->round_count = 0;
    if (game_state.versus_flag != 0 && game_state.mode != object->side + 1) {
        player_right.mode_flag_2 = 0;
        object->field_19f = 0;
        object->field_1a2 = 0;
    }
    i = 0;
    object->field_0e = 2;
    object->field_69 = value;
    object->active_flag = value;
    object->mode_flag = value;
    object->effect_code_bc = value;
    object->field_262 = value;
    object->field_49 = value;
    object->sequence_changed_flag = value;
    game_state.cursor = (unsigned int *)&object->hit_count_2;
    do {
        i++;
        *game_state.cursor++ = 0;
    } while (i < 0x4d);
    if (object->mode_flag_2) {
        object->field_d8 = 0;
        set_table_flag_from_column_table(object);
    }
    object->field_01 = 1;
    object->health = 0x90;
    object->y_offset = 0x90;
    if (game_state.config->field_a9 != 0 && object->field_f1 != 0) {
        object->health = object->saved_health;
        object->y_offset = object->saved_health;
        object->meter = object->field_fe;
    }
    init_table_fields(object);
    set_start_position(object);
    value = -0x60;
    if (object->side != 0) value = 0x60;
    object->pos_x = object->pos_x + value;
    set_object_spawn_by_side(object);
    if (player_left.kind == 0x0d && player_right.kind == 0x0d) {
        handler = shared_handler_15;
    } else if (player_left.kind == 0x10 && player_right.kind == 0x10) {
        handler = shared_handler_16;
    } else if (player_left.kind == 0x11 &&
               (player_right.kind == 0x11 || player_right.kind == 0x13)) {
        handler = shared_handler_17;
    } else if (player_left.kind == 0x13 &&
               (player_right.kind == 0x13 || player_right.kind == 0x11)) {
        handler = shared_handler_17_second;
    } else if (object->side == 0) {
        handler = handlers_left[object->kind];
    } else {
        handler = handlers_right[object->kind];
    }
    handler(object);
    player_left.field_02 = 0;
    player_right.field_02 = 1;
    select_box_tables(object);
    set_player_list_offset_from_random_pattern(&game_state);
    set_side_field_0d(object);
    set_object_field_from_kind(object);
    call_copy_image_rows_and_load_image(object);
    object->facing_side = 1;
    object->saved_facing = 1;
    object->field_0c = 0xff;
    if (object->side != 0) {
        object->facing_side = 0;
        object->saved_facing = 0;
        object->field_0c = 0xff;
    }
    build_metrics(object);
    if (game_state.config->field_42 != 0) {
        reset_object_to_state_1_0_sequence_0(object);
    } else {
        object->state_1 = 7;
        object->state_0 = 1;
        if (object->kind != 0x0b) {
            object->state_2 = 0;
            object->state_3 = 0;
        }
        if (object->kind != 0x17) set_sequence_by_side(object, 0x22);
    }
    object->remaining_count = 1;
    set_side_ring_flag(object);
}

void set_object_spawn_by_side(Object *object) {
    object->base_x = 0x180;
    object->spawn_x = 0x60;
    object->base_y = 0;
    object->spawn_y = 0x1e0;
    object->field_90 = data_801a27d8;
    if (object->side != 0) {
        object->base_x = 0x1c0;
        object->base_y = 0;
        object->field_90 = data_801a27dc;
    }
}

void call_copy_image_rows_and_load_image(Object *object) {
    copy_image_rows_and_load_image(object);
}

void set_object_field_from_kind(Object *object) {
    object->kind_table_f3_value = kind_table_f3[object->kind];
}
