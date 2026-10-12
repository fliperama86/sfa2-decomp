/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

void link_players(GameState *state, Object *a1, Object *a2) {
    a1->other = &player_left + 1;
    a2->other = &player_left;
    a1->image_row = 0x10;
    a2->image_row = 0xb;
    state->player_left_ptr = &player_left;
    state->player_right_ptr = &player_left + 1;
    if (a1->mode_flag_2 != 0) {
        a1->field_d4 = 0;
        if (a1->kind == a2->kind && a2->field_d4 == 0) {
            a1->field_d4 = 1;
        }
    }
    if (a2->mode_flag_2 != 0) {
        a2->field_d4 = 0;
        if (a1->kind == a2->kind && a1->field_d4 == 0) {
            a2->field_d4 = 1;
        }
    }
}

void func_80121730(GameState *state) {
    if (data_8018f5a0->substep_index == 0) {
        func_80121780((Object *)state);
    } else if (data_8018f5a0->substep_index == 1) {
        func_801217d4((Object *)state);
    }
}

void func_80121780(Object *object) {
    data_8018f5a0->substep_index++;
    object->phase_state = 0;
    func_801285e0();
    func_801e1d0c(0);
    object->state_2 = 0xff;
    object->team_id = 0xff;
}

void func_801217d4(Object *object) {
    u8 *flag = &data_801ae02c;
    table_8016e774[data_8018f5a0->step_index_2](object);
    if (*flag != 0) {
        func_801e1b64();
        func_801e1d6c(flag);
    }
    func_80138164();
    func_801510bc();
    func_80135ef0(object);
    func_8011a784();
}

void func_80121874(Object *object) {
    data_8018f5a0->step_index_2++;
    object->field_dc |= 0xe;
    if (object->countdown_2 != 0) {
        data_8018f5a0->step_index_2 = 0xc;
        data_8018f5a0->countdown_timer = 0x10;
        object->ot_depth = 1;
        object->linked_object_2 = 0xff;
        func_8013245c();
        object->phase_state = 0;
        player_left.field_01 = 0;
        player_right.field_01 = 0;
    }
}
