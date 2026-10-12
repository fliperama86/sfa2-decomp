/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s8 data_801bfd3c_slot04_00[];
extern s8 data_801bfd4c_slot04_00[];

void func_801b0240_slot04_00(Object *obj) {
    int r;

    obj->state_2 = obj->state_2 + 1;
    obj->countdown = 0x3c;
    if (game_state.countdown == 0) {
        game_state.countdown = 0x1e;
    }
    r = func_80125734(obj, data_801bfd3c_slot04_00[get_random_byte() & 0xf]);
    set_sequence_by_side(obj, data_801bfd4c_slot04_00[(s8)r]);
}
