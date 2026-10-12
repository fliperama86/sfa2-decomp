/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80012f64_slot12(Object *obj);
void func_80013608_slot12(Object *obj);

void func_80012eb0_slot12(Object *obj) {
    if ((game_state.field_08 >> game_state.object->side) & 1) {
        if ((game_state.field_ab & 0x80) == 0) {
            func_80012f64_slot12(obj);
            func_80013608_slot12(obj);
            obj->field_5e = obj->health - 1;
        }
    } else {
        obj->state_0++;
    }
}

void func_80012f44_slot12(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
