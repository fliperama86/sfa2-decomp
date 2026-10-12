/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80015850_slot12(Object *obj);

void slide_base_x_to_target_slot12(Object *obj) {
    s16 v;
    u8 *p = &game_state.field_2bc;
    u8 a = *p;
    if (a == 4) {
        v = obj->base_x;
        if (v != 0x20) {
            obj->base_x = v - 8;
            func_80015850_slot12(obj);
        } else {
            obj->state_1 = obj->state_1 + 1;
            if (obj->table_index == a) {
                *p = 6;
            }
        }
    }
}
