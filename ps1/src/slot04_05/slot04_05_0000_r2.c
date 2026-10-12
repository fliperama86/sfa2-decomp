/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s8 data_801c1340_slot04_05[];
extern ObjectFn data_801c1350_slot04_05[];

void func_801b0224_slot04_05(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    u8 r;

    o->state_3 = 0;
    o->countdown = 0x3c;
    o->state_2 = o->state_2 + 1;
    game_state.countdown = 0x1e;
    o->facing_side = o->saved_facing;
    r = func_80125734(o, data_801c1340_slot04_05[get_random_byte() & 0xf]);
    obj->handler_index = r;
    set_sequence_by_side(o, r + 0x23);
}

void func_801b02a8_slot04_05(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    data_801c1350_slot04_05[obj->handler_index](o);
}

void func_801b02e8_slot04_05(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
}
