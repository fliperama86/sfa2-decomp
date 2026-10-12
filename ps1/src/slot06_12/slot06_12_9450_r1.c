/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801eeec0_slot06_12[];
extern ObjectFn data_801eeed0_slot06_12[];
extern ObjectFn data_801eeed8_slot06_12[];
extern ObjectFn data_801eeee0_slot06_12[];
extern ObjectFn data_801eeef0_slot06_12[];
extern ObjectFn data_801ef300_slot06_12[];
extern SequenceStep *data_801eee74_slot06_12[];

Block172 *pop_block_from_pool(void);
void spawn_on_active_state_change_slot06_12(Object *a, u8 *b);
void spawn_object_35_at_ground_slot06_12(Object *obj);
void offset_pos_x_by_layer_scroll_start_sequence_slot06_12(Object *obj);
void advance_accel_y_index_and_load_record_slot06_12(Object *obj);
void set_table_index_from_accel_x_high_bits_slot06_12(Object *obj);
void set_object_spawn_position_and_sequence_slot06_12(Object *obj);

void call_handler_by_state_0_slot06_12(Object *obj) {
    data_801eeec0_slot06_12[obj->state_0](obj);
}

void init_spawn_and_call_handler_by_table_index_b_slot06_12(Object *obj) {
    obj->spawn_mode = 4;
    obj->field_0c = 0;
    obj->state_0++;
    data_801eeed0_slot06_12[obj->table_index](obj);
}

void init_object_store_player_positions_slot06_12(Object *obj) {
    Object *p;

    obj->field_0f = 1;
    game_state.pair_ptr = (Pair *)&player_left;
    obj->hit_type = 0;
    obj->field_62 = 0;
    *(u32 *)&obj->health = *(u32 *)&((Object *)game_state.pair_ptr)->pos_x_frac;
    obj->field_60 = ((Object *)game_state.pair_ptr)->active_state;
    p = &player_left;
    p++;
    game_state.pair_ptr = (Pair *)p;
    obj->team_id = 0;
    obj->sound_side = 0;
    *(u32 *)&obj->field_60 = *(u32 *)&((Object *)game_state.pair_ptr)->pos_x_frac;
    obj->damage = ((Object *)game_state.pair_ptr)->active_state;
}

void init_object_fields_and_set_sequence_zero_slot06_12(Object *obj) {
    obj->facing_side = 0;
    obj->field_0a = 1;
    obj->field_0e = 8;
    obj->ot_depth = 0x1a;
    obj->field_0f = 1;
    set_sequence_from_table_5(obj, 0, data_801eee74_slot06_12);
}

void call_handler_by_table_index_a_slot06_12(Object *obj) {
    data_801eeed8_slot06_12[obj->table_index](obj);
}

void check_both_players_active_state_change_slot06_12(Object *obj) {
    Object *pair = &player_left;

    spawn_on_active_state_change_slot06_12(pair, (u8 *)obj + 0x5c);
    spawn_on_active_state_change_slot06_12(pair + 1, (u8 *)obj + 0x60);
}

void spawn_on_active_state_change_slot06_12(Object *a, u8 *b) {
    u8 x = a->active_state;
    u8 y = b[4];

    if ((x & y) == 0 && (x | y) != 0) {
        b[5] = 0;
        b[6] = 0;
        spawn_object_35_at_ground_slot06_12(a);
    }
    b[4] = a->active_state;
}

void spawn_object_35_at_ground_slot06_12(Object *obj) {
    game_state.other_object = (Object *)pop_block_from_pool();
    if (game_state.other_object != 0) {
        game_state.other_object->active_count = 1;
        game_state.other_object->field_02 = 0x35;
        game_state.other_object->table_index = 1;
        game_state.other_object->pos_x = obj->pos_x;
        game_state.other_object->pos_y = obj->ground_y;
        game_state.other_object->parent = obj;
        game_state.other_object->spawn_mode = 4;
    }
}

void advance_sequence_next_state_on_negative_flags_slot06_12(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        obj->state_0++;
    }
    advance_sequence_checked(obj);
    clear_or_set_flag_by_pos_x_limit(obj);
}

void release_object_record_slot06_12(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_state_0_2_slot06_12(Object *obj) {
    data_801eeee0_slot06_12[obj->state_0](obj);
}

void advance_state_save_x_start_sequence_1_slot06_12(Object *obj) {
    obj->field_0f = 1;
    obj->field_0a = 1;
    obj->spawn_mode = 4;
    obj->state_0++;
    obj->vel_x = obj->pos_x;
    set_sequence_from_table_5(obj, 1, data_801eee74_slot06_12);
}

void update_object_x_from_layer_scroll_slot06_12(Object *obj) {
    Slot06Layer *l2;
    s16 d;
    int x;

    if (game_state.field_65 == 0) {
        l2 = (Slot06Layer *)data_801aa5d4;
        x = l2->scroll_x;
        x -= l2->ref_x;
        x &= -0x80;
        d = obj->vel_x + x;
        obj->pos_x = d;
        d = *(u16 *)&l2->vel_x;
        d += 1;
        set_sequence_from_table_5(obj, d, data_801eee74_slot06_12);
    }
    set_flag_and_push_if_mode_8(obj);
}

void release_object_record_2_slot06_12(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_state_0_3_slot06_12(Object *obj) {
    data_801eeef0_slot06_12[obj->state_0](obj);
}

void init_object_saving_position_in_velocity_slot06_12(Object *obj) {
    obj->field_0f = 1;
    obj->field_0a = 1;
    obj->spawn_mode = 4;
    obj->state_0++;
    obj->vel_x = obj->pos_x;
    obj->vel_y = obj->pos_y;
    offset_pos_x_by_layer_scroll_start_sequence_slot06_12(obj);
}

void offset_pos_x_by_layer_scroll_start_sequence_slot06_12(Object *obj) {
    Slot06Layer *l2 = (Slot06Layer *)data_801aa5d4;

    obj->pos_x = l2->ref_x - l2->scroll_x + (u16)obj->pos_x;
    set_sequence_from_table_5(obj, 1, data_801eee74_slot06_12);
}

void update_object_position_from_layer_scroll_slot06_12(Object *obj) {
    Slot06Layer *l2;
    s16 d;

    if (game_state.field_65 == 0) {
        l2 = (Slot06Layer *)data_801aa5d4;
        d = l2->field_0e;
        d -= l2->scroll_y;
        d += d >> 2;
        obj->pos_y = (u16)obj->vel_y - d;
        obj->pos_x = l2->ref_x - l2->scroll_x + (u16)obj->vel_x;
        advance_sequence_checked(obj);
    }
    set_flag_and_push_if_mode_8(obj);
}

void release_object_record_3_slot06_12(Object *obj) {
    reset_and_push_object_to_pool_table((Slab172 *)obj);
}

void call_handler_by_state_0_4_slot06_12(Object *obj) {
    data_801ef300_slot06_12[obj->state_0](obj);
}

void init_object_and_advance_state_0_slot06_12(Object *obj) {
    obj->field_01 = 0;
    obj->field_0c = 0;
    obj->accel_y = 0;
    obj->spawn_mode = 4;
    obj->state_0++;
    advance_accel_y_index_and_load_record_slot06_12(obj);
    set_table_index_from_accel_x_high_bits_slot06_12(obj);
    set_object_spawn_position_and_sequence_slot06_12(obj);
}
