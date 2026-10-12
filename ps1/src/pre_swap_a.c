/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void func_801312b8(Object *object) {
    func_80131f7c(object);
    set_sequence_by_side(object, 0);
    object->state_0 = 1;
    object->state_1 = 0;
    object->state_2 = 0;
    object->state_3 = 0;
    object->field_157 = 0;
    func_8013151c(object);
    func_80155f30(object);
    object->field_170 = 0;
    object->round_count = 0;
    if (game_state.versus_flag != 0 && game_state.mode != object->side + 1) {
        object->field_1a1 = object->field_1a1 - 1;
        if (object->field_1a1 & 0x80) {
            object->field_19f = 0;
        }
    }
    object->field_69 = 0;
}

void func_8013136c(Object *object) {
    if (object->mode_flag_2 == 0 && object->kind == 10) {
        func_801313bc(object, 0x30);
    } else {
        func_801312b8(object);
    }
}

void func_801313bc(Object *object, unsigned short arg) {
    set_sequence_by_side(object, arg);
    object->state_0 = 1;
    object->state_1 = 0;
    object->state_2 = 0;
    object->state_3 = 0;
    object->field_157 = 0;
    func_8013151c(object);
    func_80155f30(object);
    object->field_170 = 0;
    object->round_count = 0;
    if (game_state.versus_flag != 0 && game_state.mode != object->side + 1) {
        object->field_1a1 = object->field_1a1 - 1;
        if (object->field_1a1 & 0x80) {
            object->field_19f = 0;
        }
    }
    object->field_69 = 0;
}

void func_80131468(Object *object) {
    func_80131f7c(object);
    object->state_0 = 1;
    object->state_1 = 0;
    object->state_2 = 0;
    object->state_3 = 2;
    object->field_157 = 1;
    func_8013151c(object);
    func_80155f30(object);
    object->field_170 = 0;
    object->round_count = 0;
    if (game_state.versus_flag != 0 && game_state.mode != object->side + 1) {
        object->field_1a1 = object->field_1a1 - 1;
        if (object->field_1a1 & 0x80) {
            object->field_19f = 0;
        }
    }
    object->field_69 = 0;
    set_sequence_by_side(object, 1);
}

void func_8013151c(Object *object) {
    object->active_state = 0;
    object->field_15b = 0;
    object->field_15c = 0;
    object->field_165 = 0;
    object->flag_166_set = 0;
    object->field_241 = 0;
    object->step_applied_flag = 0;
    object->kind_table_value = 0;
    object->field_24e = 0;
    object->field_251 = 0;
    object->input_matched_flag = 0;
    object->field_256 = 0;
    object->once_flag_2 = 0;
    object->done_flag = 0;
    object->flag_25a_set = 0;
    object->state_change_flag = 0;
    object->field_252 = 0;
    object->field_25f = 0;
    object->hit_flag = 0;
    object->active_flag_4 = 0;
    object->field_16a = 0;
    object->hit_count_a = 0;
    object->hit_count_b = 0;
    object->dir_second = 0xff;
    object->dir_first = 0xff;
    object->field_17e = 0xff;
    object->field_28a = 0;
    object->flags_28b = 0;
    object->cooldown_timer = 0;
    object->pos_y = object->ground_y;
    *(unsigned int *)&object->pos_y_frac &= 0xffff0000;
    object->field_295 = 0;
    object->facing_side = object->saved_facing;
    object->field_49 = 0;
    object->field_4b = 0;
    object->field_299 = 0;
    object->field_29c = 0;
    object->hit_skip_flag = 0;
    object->field_278 = 0;
    object->dir_match_flag = 0;
    object->saved_mode_bits = 0;
    object->hit_flag_a = 0;
    object->attack_param_d = 0;
    object->field_221 = 0;
    object->once_flag = 0;
    object->sequence_hold_flag = 0;
    object->field_2a3 = 0;
    if (object->field_7e == 0) {
        object->active_flag_3 = 0;
    }
    game_state.other_object = object->other;
    if (game_state.other_object->field_7e == 0) {
        object->field_225 = 0;
        object->field_216 = 0;
    }
}

void func_80131638(Object *object) {
    func_80131f7c(object);
    object->state_0 = 1;
    object->state_2 = 3;
    object->state_1 = 0;
    object->state_3 = 2;
    object->pos_y = object->ground_y;
    func_8013151c(object);
    if (object->mode_flag_2 == 0) {
        if (func_80130470(object)) {
            func_8013047c(object);
        } else if (func_8012f970(object)) {
            func_8012fe60(object);
        } else if ((u8)func_8012f898(object)) {
            func_8012f8c4(object);
        } else {
            set_sequence_by_side(object, 0x11);
        }
    } else {
        if ((u8)func_8012f898(object)) {
            func_8012f8c4(object);
        } else {
            set_sequence_by_side(object, 0x11);
        }
    }
}
