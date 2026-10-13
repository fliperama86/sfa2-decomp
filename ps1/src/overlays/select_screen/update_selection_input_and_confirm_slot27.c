/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern Slot27Rec9338 data_80029338_slot27[];
void select_kind_from_pressed_direction_slot27(void);
void update_input_sequence_match_record_slot27(void);
void update_hold_counters_for_kind_2_and_4_slot27(void);
void set_sequence_and_position_by_other_kind_slot27(Object *obj);
void check_opposite_player_same_selection_slot27(void);
void load_palette_anim_from_table_index_slot27(Object *obj);
void advance_palette_animation_and_upload_slot27(Object *obj);

void update_selection_input_and_confirm_slot27(Object *obj) {
    u8 *e;
    Object *s0;
    int k;

    select_kind_from_pressed_direction_slot27();
    update_input_sequence_match_record_slot27();
    (*(s32 *)&game_state.field_354) = ref_other.p->kind;
    if ((*(s32 *)&game_state.field_354) != obj->table_index) {
        call_func_with_side_x_origin(ref_other.p->side, 0);
        obj->facing_flag = 0;
        if ((*(u8 *)&data_801901ba) != 0 && (*(u8 *)&game_state.pending_flag_2) != 0) {
            (*(u8 *)&data_801901ba) = 0;
        }
        obj->table_index = *(u8 *)scratch_word_a;
        set_sequence_and_position_by_other_kind_slot27(obj);
        goto done;
    }
    s0 = ref_other.p;
    e = (u8 *)(data_80029338_slot27 + ref_other.p->side);
    update_hold_counters_for_kind_2_and_4_slot27();
    if (e[5] == 0xb4 && s0->kind == 4) {
        (*(s32 *)&game_state.field_354) = ~ref_other.p->prev_input_bits;
        (*(s32 *)scratch_word_b) = ref_other.p->input_bits;
        (*(s32 *)&game_state.field_354) = (*(s32 *)scratch_word_b) & (*(s32 *)&game_state.field_354) & 0xf0;
        if ((*(s32 *)&game_state.field_354) != 0) {
            s0->kind = 0x12;
        }
    }
    if (data_8016e68e != 0 && e[6] == 0xb4) {
        if (s0->kind != 2) goto l280;
        (*(s32 *)&game_state.field_354) = ~ref_other.p->prev_input_bits;
        (*(s32 *)scratch_word_b) = ref_other.p->input_bits;
        (*(s32 *)&game_state.field_354) = (*(s32 *)scratch_word_b) & (*(s32 *)&game_state.field_354) & 0xf0;
        if ((*(s32 *)&game_state.field_354) != 0) {
            s0->kind = 0x14;
        }
    }
    if (s0->kind == 2 && e[4] != 0) {
        (*(s32 *)&game_state.field_354) = ~ref_other.p->prev_input_bits;
        (*(s32 *)scratch_word_b) = ref_other.p->input_bits;
        (*(s32 *)&game_state.field_354) = (*(s32 *)scratch_word_b) & (*(s32 *)&game_state.field_354) & 0xf0;
        if ((*(s32 *)&game_state.field_354) != 0) {
            s0->kind = 0x14;
            data_8016e68e = -1;
        }
    }
l280:
    if (game_state.busy_flag != 0) goto done;
    if (ref_other.p->input_bits & 0x100) {
        if (game_state.selection_value & 0x80) {
            obj->facing_flag = obj->facing_flag + 1;
            if (obj->facing_flag >= 0x3c) {
                (*(s32 *)scratch_word_c) = ref_other.p->kind;
                if ((*(s32 *)scratch_word_c) == 0x12) (*(s32 *)scratch_word_c) = 4;
                if ((*(s32 *)scratch_word_c) == 0x14) (*(s32 *)scratch_word_c) = 2;
                game_state.selection_value = (*(s32 *)scratch_word_c);
                if ((*(s32 *)scratch_word_c) == 10) goto setb;
                if ((*(s32 *)scratch_word_c) == 11) {
setb:
                    (*(u8 *)&data_801901ba) = (*(s32 *)scratch_word_c);
                    (*(u8 *)&game_state.pending_flag_2) = 0;
                }
            }
        }
    } else {
        if ((*(u8 *)&data_801901ba) != 0) {
            (*(u8 *)&game_state.pending_flag_2) = 1;
        }
        obj->facing_flag = 0;
        obj->ground_y = 0;
    }
    (*(s32 *)&game_state.field_354) = 0x80;
    (*(s32 *)scratch_word_b) = 0;
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown != 0) {
        (*(s32 *)&game_state.field_354) = ~ref_other.p->prev_input_bits;
        (*(s32 *)scratch_word_b) = ref_other.p->input_bits;
        (*(s32 *)&game_state.field_354) = (*(s32 *)scratch_word_b) & (*(s32 *)&game_state.field_354) & 0xf0;
        if ((*(s32 *)&game_state.field_354) == 0) goto done;
    }
    obj->state_2 = obj->state_2 + 1;
    call_func_with_side_x_origin(ref_other.p->side, 1);
    k = (*(s32 *)&game_state.field_354);
    (*(s32 *)scratch_word_c) = 0;
    (*(s32 *)scratch_word_b) = k;
    (*(s32 *)&game_state.field_354) = ((k & 0x80) >> 6) | ((k & 0x10) >> 4);
    if ((*(s32 *)&game_state.field_354) == 0) {
        (*(s32 *)scratch_word_c) = 1;
        (*(s32 *)scratch_word_b) = ((k & 0x40) >> 5) | ((k & 0x20) >> 5);
        (*(s32 *)&game_state.field_354) = (*(s32 *)scratch_word_b);
    }
    if ((*(s32 *)&game_state.field_354) >= 3) {
        (*(s32 *)scratch_word_c) = (*(s32 *)scratch_word_c) + 2;
    }
    ref_other.p->field_d4 = (*(s32 *)scratch_word_c);
    (*(s32 *)&game_state.field_354) = ref_other.p->kind;
    (*(s32 *)&game_state.field_354) = ref_other.p->kind;
    (*(s32 *)scratch_word_b) = ref_other.p->side;
    (*(s32 *)&game_state.field_354) = ref_other.p->side;
    game_state.mode |= 1 << (*(s32 *)&game_state.field_354);
    game_state.side = (*(s32 *)&game_state.field_354);
    check_opposite_player_same_selection_slot27();
    if ((*(s32 *)&game_state.field_354) != 0) {
        (*(s32 *)&game_state.field_354) = 0;
        if (ref_other.p->field_d4 == 0) (*(s32 *)&game_state.field_354) = 1;
        ref_other.p->field_d4 = (*(s32 *)&game_state.field_354);
    }
    (*(s32 *)&game_state.field_354) = 1;
    load_palette_anim_from_table_index_slot27(obj);
    return;
done:
    advance_palette_animation_and_upload_slot27(obj);
}
