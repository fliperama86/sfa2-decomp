/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80015850_slot12(Object *obj);

void func_8001562c_slot12(Object *obj) {
    s16 v;
    u8 *p = &game_state.field_2bc;
    u8 a = *p;
    if (a == 4) {
        v = obj->field_76;
        if (v != 0x20) {
            obj->field_76 = v - 8;
            func_80015850_slot12(obj);
        } else {
            obj->field_05 = obj->field_05 + 1;
            if (obj->field_03 == a) {
                *p = 6;
            }
        }
    }
}
