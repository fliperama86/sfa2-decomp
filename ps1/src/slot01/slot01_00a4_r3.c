/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Object *data_8002ceb0_slot01;
extern Object *data_80055f40_slot01;

void func_80010498_slot01(void) {
    int t = game_state.timer_or_bits - 1;
    game_state.timer_or_bits = t;
    if ((s16)t < 0 || func_801252f0() != 0) {
        data_8018f5a0->substep_index++;
        game_state.status_flags = 0xff;
        if (game_state.mode != 0) {
            game_state.field_09 = 0;
        }
    }
}

void func_80010524_slot01(void) {
    Object *s0;
    Object *p;

    s0 = game_state.focus_object;
    data_8018f5a0->substep_index++;
    game_state.timer_or_bits = 0xb4;
    data_8002ceb0_slot01->spawn_y = 0x1e9;
    p = (Object *)func_8011f1e0();
    if (p) {
        p->active_count = 1;
        p->field_02 = 0x1b;
        p->table_index = 0x80;
        p->spawn_x = 0x70;
        p->parent = s0;
        p->spawn_y = 0x1ea;
        data_80055f40_slot01 = p;
    }
}
