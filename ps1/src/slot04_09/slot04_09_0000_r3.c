/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7b9c_slot04_09[];

void func_801b03b4_slot04_09(Object *obj) {
    s16 t = obj->countdown;

    if (t != 0) {
        t = t - 1;
        obj->countdown = t;
        if (t == 0) {
            game_state.side_bits_2 |= 1 << obj->side;
        }
    }
    data_801c7b9c_slot04_09[obj->variant_index](obj);
}
