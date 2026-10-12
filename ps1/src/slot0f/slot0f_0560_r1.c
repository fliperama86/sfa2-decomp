/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s8 data_800f01fc_slot0f;
void queue_item_from_table_by_index_slot0f(int a);
void update_frame_objects_and_effects_slot0f(GameState *g, int x);

void advance_state_index_and_reinit_slot0f(Object *obj) {
    HudState *h = data_8018f5a0;
    h->state_index++;
    h->phase_index = 0;
    h->step_index = 0;
    stop_all_keys_clear_table();
    clear_players_arrays_state_and_init_pools();
}

void update_blink_counter_and_queue_entries_slot0f(int a, int b) {
    /* The unused local reproduces the stack frame of the original, which reserves the space and never uses it. It is a stand-in, not an explanation. */
    int unused[2];
    int hi;
    int lo;
    int v;
    if (b == 0) {
        data_800f01fc_slot0f = 0;
        return;
    }
    if (a == 0) {
        queue_item_from_table_by_index_slot0f(1);
    }
    if (data_800f01fc_slot0f & 0x80) {
        queue_item_from_table_by_index_slot0f(0);
    }
    v = data_800f01fc_slot0f;
    hi = v & 0x80;
    lo = v & 0x7f;
    lo++;
    if (lo == 0x20) {
        if (hi != 0) {
            lo = 0;
            hi = 0;
        } else {
            lo = 0x80;
        }
    }
    data_800f01fc_slot0f = hi | lo;
}

void update_countdown_timer_and_run_frame_slot0f(GameState *g) {
    int x;
    if (g->field_4f != 0) {
        return;
    }
    x = 0;
    if (g->countdown_timer != 0) {
        g->countdown_timer_b--;
        x = 1;
        if (g->countdown_timer_b == 0) {
            x = 0;
            g->countdown_timer--;
            g->countdown_timer_b = g->reload_value;
        }
    }
    update_frame_objects_and_effects_slot0f(g, x);
}
