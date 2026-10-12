/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_blink_counter_and_queue_entries_slot0f(int a, int b);
void update_countdown_timer_and_run_frame_slot0f(GameState *g);

void start_phase_with_random_player_scripts_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    HudState *h = data_8018f5a0;
    h->phase_index += 1;
    h->step_index = 0;
    obj->field_c6 = 0x1e0;
    init_counters_from_index_table((GameState *)obj);
    update_table_columns_unless_mode3_inactive((GameState *)obj);
    obj->field_2d = 7;
    player_left.table_column = 0x1f;
    player_right.table_column = 0x1f;
    player_left.script_table_index = (get_random_byte() & 3) << 1;
    player_right.script_table_index = (get_random_byte() & 3) << 1;
    update_blink_counter_and_queue_entries_slot0f(1, 0);
}

void update_countdown_timer_and_advance_phase_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    s16 t;
    update_blink_counter_and_queue_entries_slot0f(1, 1);
    t = obj->field_c6;
    t -= 1;
    obj->field_c6 = t;
    if (t == 0) {
        data_8018f5a0->phase_index += 1;
        player_right.field_01 = 0;
        player_left.field_01 = 0;
        obj->field_4e = 1;
    } else {
        u8 c;
        c = obj->countdown_b - 1;
        obj->countdown_b = c;
        if (c == 0) {
            obj->countdown_b = 0x3b;
            obj->countdown_2--;
        }
        update_countdown_timer_and_run_frame_slot0f((GameState *)o);
    }
}
