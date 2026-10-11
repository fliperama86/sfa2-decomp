/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3530_slot04_03(Object *obj);

void func_801b3478_slot04_03(Object *obj) {
    obj->field_46 = 0xc;
    obj->state_3 = obj->state_3 + 1;
    func_80141f28(obj, 4);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    func_801307e0(obj, 0x19);
}

void func_801b34d8_slot04_03(Object *obj) {
    obj->field_46 = (s16)obj->field_46 - 1;
    if ((s16)obj->field_46 != 0) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        func_801b3530_slot04_03(obj);
    }
}
