/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3974_slot04_07(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    func_80141f28(obj, 3);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    if ((obj->field_c2 & 0x8000) == 0) {
        obj->field_0b = 0;
    } else {
        obj->field_0b = 1;
    }
    func_801307e0(obj, 0x18);
}
