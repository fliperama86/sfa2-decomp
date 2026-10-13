/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../game.h"
#include "../../protos.h"
#include "../../externs.h"

extern u16 data_800285d4_slot28[];
extern void (*data_80028a4c_slot28[])(Object *);

void reset_hud_and_game_state_fields_slot28(void) {
    HudState *h = screen_state;
    h->phase_index = 9;
    game_state.step_delay_timer = 0;
    h->step_index = 0;
    game_state.countdown_2 = 0;
    h->substep_index = 0;
    game_state.timer_or_bits = 0;
    h->step_index_2 = 0;
    game_state.countdown_timer_2 = 0;
    h->pass_index = 0;
    game_state.field_cc = 0;
    game_state.active_flag = 0;
    game_state.state_2c_flag = 0;
    game_state.bit_mask = 0;
    game_state.field_65 = 1;
    game_state.status_flags = 1;
    game_state.field_4f = 0;
    game_state.field_09 = 1;
    game_state.state_46_flag = 0;
    game_state.field_44 = 1;
}

void copy_table_twice_call_state_handler_slot28(Object *o) {
    u16 *a = data_801a2be4;
    u16 *b = a + 0xa00;
    int i;
    for (i = 0; i < 0x200; i++) {
        a[i] = data_800285d4_slot28[i];
        b[i] = data_800285d4_slot28[i];
    }
    data_80028a4c_slot28[((Slot28Obj *)o)->state_index & 0x7f](o);
}
