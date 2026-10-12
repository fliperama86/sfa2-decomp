/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern s8 data_801c1e54_slot04_07[];
extern s8 data_801c1e64_slot04_07[];

void func_801b0460_slot04_07(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->field_46 = 0x3c;
    game_state.field_76 = 0x1e;
    set_sequence_by_side(obj, data_801c1e64_slot04_07[(s8)func_80125734(obj, data_801c1e54_slot04_07[get_random_byte() & 0xf])]);
}
