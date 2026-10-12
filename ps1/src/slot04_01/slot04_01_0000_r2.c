/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s8 data_801beeb0_slot04_01[];
extern s8 data_801beec0_slot04_01[];

void func_801b0220_slot04_01(Object *obj) {
    int v;

    obj->countdown = 0x3c;
    obj->state_2 = obj->state_2 + 1;
    if (game_state.config->countdown_2 == 0) {
        game_state.config->countdown_2 = 0x1e;
    }
    v = (s8)func_80125734(obj, data_801beeb0_slot04_01[get_random_byte() & 0xf]);
    set_sequence_by_side(obj, data_801beec0_slot04_01[v]);
}
