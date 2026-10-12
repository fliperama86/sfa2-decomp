/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_800e000c_slot0f(GameState *state, Menu *menu) {
    request_sound_command_and_switch_thread(0x3c);
    scratch_word_00 = -1;
    scratch_word_04 = 1;
    load_and_exec_program_by_index(2);
}

void func_800e004c_slot0f(Object *obj) {
    u8 t = obj->field_02;
    obj->field_02 = 0;
    obj->table_index = t;
    if (data_801a89a8[0] == 0) {
        obj->field_02 = 1;
    }
    if (data_801a89a8[4] == 0) {
        obj->field_02 |= 2;
    }
}

void reset_hud_state_and_sound_by_arg_slot0f(Object *obj, int arg) {
    HudState *h = data_8018f5a0;
    h->state_index = 0;
    h->phase_index = 0;
    h->step_index = 0;
    h->substep_index = 0;
    h->step_index_2 = 0;
    h->pass_index = 0;
    obj->input_bits = 0;
    obj->prev_input_bits = 0;
    obj->meter = 0;
    obj->field_c8 = 0;
    obj->field_ca = 0;
    *(u16 *)&obj->max_value_cc = 0;
    obj->field_01 = 0;
    *(u8 *)&obj->pos_y = 1;
    player_left.side = 0;
    player_right.side = 1;
    close_voice_thread(1);
    if (arg == 0) {
        set_voice_thread_entry_and_open(1, 5);
        func_80119198(1);
    } else if (arg == 1) {
        func_80119198(3);
    } else {
        func_80119198(1);
    }
}
