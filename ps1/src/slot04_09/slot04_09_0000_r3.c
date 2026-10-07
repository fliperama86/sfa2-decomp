/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7b9c_slot04_09[];

void func_801b03b4_slot04_09(Object *obj) {
    s16 t = obj->field_46;

    if (t != 0) {
        t = t - 1;
        obj->field_46 = t;
        if (t == 0) {
            game_state.field_4b |= 1 << obj->side;
        }
    }
    data_801c7b9c_slot04_09[obj->field_48](obj);
}
