/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

extern ObjectRef data_80190468;
extern ObjectFn data_801eefc4_slot06_0f[];
extern ObjectFn data_801eefd4_slot06_0f[];
extern ObjectFn data_801eefe4_slot06_0f[];
extern ObjectFn data_801eefec_slot06_0f[];
extern ObjectFn data_801eeff4_slot06_0f[];

Block172 *pop_block_from_pool(void);
void update_child_object_by_state_1_handler_slot06_0f(Object *obj);
void set_states_to_1_0_0_0_random_countdown_slot06_0f(Object *obj);
void update_variant_by_relative_x_slot06_0f(Object *obj);
int func_801ea77c_slot06_0f(Object *obj);
void set_states_one_one_and_random_countdown_slot06_0f(Object *obj);
int is_other_object_state_attacking_with_hit_slot06_0f(Object *obj);
void set_states_one_two_and_random_countdown_slot06_0f(Object *obj);
int is_both_state_checks_true_slot06_0f(Object *obj);
void set_states_one_one_and_random_countdown_2_slot06_0f(Object *obj);
int is_config_enabled_flag_set_slot06_0f(Object *obj);
void set_states_1_3_0_0_slot06_0f(Object *obj);
void set_sequence_from_other_side_table_slot06_0f(Object *obj, int idx);

void spawn_object_for_kind_0f_player_slot06_0f(void) {
    Object *s;
    Object *n;
    Object *right;
    int k;
    data_80190468.p = (Object *)&game_state;
    if (((GameState *)data_80190468.p)->kind_index == 0xf && ((GameState *)data_80190468.p)->field_80 == 0) {
        s = &player_left;
        right = s + 1;
        k = player_left.kind == 0xf;
        if (player_right.kind == ((GameState *)data_80190468.p)->kind_index) {
            k |= 2;
        }
        if (k != 1) {
            if (k == 0) {
                return;
            }
            if (k == 2 || (k == 3 && player_right.win_streak != 0)) {
                s = right;
            }
        }
        n = (Object *)pop_block_from_pool();
        if (n != 0) {
            n->active_count = 1;
            n->field_02 = 0x46;
            n->parent = s;
            n->sound_side = s->side;
            n->ot_depth = 0xe;
            n->field_1c = s->field_1c;
            n->spawn_x = 0x60;
            n->spawn_y = 0x1e0;
            n->image_row = s->image_row;
            n->field_90 = s->field_90;
            n->data_ptr_a = s->data_ptr_a;
            n->data_ptr_b = s->data_ptr_b;
        }
    }
}

void call_handler_by_state_0_3_slot06_0f(Object *obj) {
    data_80190468.p = (Object *)&game_state;
    data_801eefc4_slot06_0f[obj->state_0](obj);
}

void init_object_from_parent_position_slot06_0f(Object *obj) {
    s16 t;
    game_state.other_object = obj->parent;
    if (game_state.other_object->field_01 != 0) {
        obj->state_0 = obj->state_0 + 1;
        obj->field_0c = game_state.other_object->field_0c;
        obj->field_0e = game_state.other_object->field_0e;
        obj->ot_depth = 0xe;
        obj->field_1c = game_state.other_object->field_1c;
        obj->pos_x = game_state.other_object->pos_x;
        obj->pos_y = game_state.other_object->pos_y - 6;
        t = 0x30;
        if (game_state.other_object->facing_side == 0) {
            t = -0x30;
        }
        obj->pos_x += t;
        obj->table_index = game_state.other_object->kind;
        obj->field_0f = 1;
        obj->countdown = 1;
        obj->facing_side = 0;
        obj->variant_index = 0;
        set_sequence_from_other_side_table_slot06_0f(obj, 0);
        update_child_object_by_state_1_handler_slot06_0f(obj);
    }
}

void update_child_object_by_state_1_handler_slot06_0f(Object *obj) {
    GameState *g = (GameState *)data_80190468.p;
    if (g->gate_flag == 0) {
        game_state.other_object = obj->parent;
        if (obj->table_index == game_state.other_object->kind) {
            if (g->field_65 == 0) {
                data_801eefd4_slot06_0f[obj->state_1](obj);
            }
            if (game_state.field_226 == 0) {
                clear_or_set_flag_by_pos_x_limit(obj);
            } else {
                obj->field_01 = 0;
            }
            return;
        }
    }
    obj->state_0 = 2;
}

void call_handler_by_state_2_slot06_0f(Object *obj) {
    data_801eefe4_slot06_0f[obj->state_2](obj);
}

void increment_state_2_and_update_variant_by_x_slot06_0f(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    update_variant_by_relative_x_slot06_0f(obj);
}

void select_state_from_other_object_state_checks_slot06_0f(Object *obj) {
    if (is_config_enabled_flag_set_slot06_0f(obj)) {
        set_states_1_3_0_0_slot06_0f(obj);
    } else if (is_both_state_checks_true_slot06_0f(obj)) {
        set_states_one_one_and_random_countdown_2_slot06_0f(obj);
    } else if (func_801ea77c_slot06_0f(obj)) {
        set_states_one_one_and_random_countdown_slot06_0f(obj);
    } else if (is_other_object_state_attacking_with_hit_slot06_0f(obj)) {
        set_states_one_two_and_random_countdown_slot06_0f(obj);
    } else {
        update_variant_by_relative_x_slot06_0f(obj);
        advance_sequence_checked(obj);
    }
}

void call_handler_by_state_2_2_slot06_0f(Object *obj) {
    data_801eefec_slot06_0f[obj->state_2](obj);
}

void count_down_then_set_sequence_2_slot06_0f(Object *obj) {
    int v = obj->countdown - 1;
    obj->countdown = v;
    if ((s16)v < 0) {
        obj->facing_side = 0;
        obj->state_2++;
        set_sequence_from_other_side_table_slot06_0f(obj, 2);
    } else {
        advance_sequence_checked(obj);
    }
}

void update_state_by_other_object_state_a_slot06_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        set_states_to_1_0_0_0_random_countdown_slot06_0f(obj);
    } else {
        if (is_config_enabled_flag_set_slot06_0f(obj)) {
            set_states_1_3_0_0_slot06_0f(obj);
        }
        if (func_801ea77c_slot06_0f(obj) == 0 && is_both_state_checks_true_slot06_0f(obj) == 0 && is_other_object_state_attacking_with_hit_slot06_0f(obj) != 0) {
            set_states_one_two_and_random_countdown_slot06_0f(obj);
        } else {
            advance_sequence_checked(obj);
        }
    }
}

void call_handler_by_state_2_3_slot06_0f(Object *obj) {
    data_801eeff4_slot06_0f[obj->state_2](obj);
}

void count_down_then_set_sequence_3_slot06_0f(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown < 0) {
        obj->facing_side = 0;
        obj->state_2++;
        set_sequence_from_other_side_table_slot06_0f(obj, 3);
    }
    advance_sequence_checked(obj);
}

void update_state_by_other_object_state_b_slot06_0f(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        set_states_to_1_0_0_0_random_countdown_slot06_0f(obj);
    } else if (is_config_enabled_flag_set_slot06_0f(obj)) {
        set_states_1_3_0_0_slot06_0f(obj);
    } else if (is_other_object_state_attacking_with_hit_slot06_0f(obj) == 0 && is_both_state_checks_true_slot06_0f(obj) == 0 && func_801ea77c_slot06_0f(obj) != 0) {
        set_states_one_one_and_random_countdown_slot06_0f(obj);
    } else {
        advance_sequence_checked(obj);
    }
}
