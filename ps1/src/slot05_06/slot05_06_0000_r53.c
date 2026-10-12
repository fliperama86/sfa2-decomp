/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801dd50c_slot05_06[];
extern u16 data_801dd51c_slot05_06[];
extern ObjectFn data_801dd528_slot05_06[];

void restore_facing_and_set_sequence_0_slot05_06(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->facing_side = obj->saved_facing;
    set_sequence_by_side(obj, 0);
}

void advance_state_2_when_timers_clear_slot05_06(Object *obj) {
    if (game_state.field_64 == 0 && game_state.countdown_timer == 0) {
        obj->state_2 = obj->state_2 + 1;
    }
    update_sequence(obj);
}

void pick_random_move_store_doubled_index_slot05_06(Object *obj) {
    int t;
    u16 v;
    u16 *p;

    obj->state_2 = obj->state_2 + 1;
    ((Slot04aObj *)obj)->timer = 0x3c;
    game_state.countdown = 0x1e;
    obj->facing_side = obj->saved_facing;
    t = get_random_byte() & 0x3f;
    p = &((Slot04aObj *)obj)->field_1c0;
    if (t == 0) {
        v = 4;
    } else {
        v = data_801dd50c_slot05_06[t & 7];
    }
    v = (s8)get_input_mapped_value_or_default(obj, (s8)v);
    *p = v + v;
    set_sequence_by_side(obj, data_801dd51c_slot05_06[v]);
}

void func_801cca20_slot05_06(Object *obj) {
    data_801dd528_slot05_06[((Slot04aObj *)obj)->field_1c0 >> 1](obj);
}

void increment_state_2_slot05_06(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
}
