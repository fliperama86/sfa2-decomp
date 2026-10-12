/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u16 data_801c5510_slot04_06[];
extern u16 data_801c5520_slot04_06[];
extern ObjectFn data_801c552c_slot04_06[];

void func_801b4960_slot04_06(Object *obj) {
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
        v = data_801c5510_slot04_06[t & 7];
    }
    v = (s8)get_input_mapped_value_or_default(obj, (s8)v);
    *p = v + v;
    set_sequence_by_side(obj, data_801c5520_slot04_06[v]);
}

void func_801b4a24_slot04_06(Object *obj) {
    data_801c552c_slot04_06[((Slot04aObj *)obj)->field_1c0 >> 1](obj);
}
