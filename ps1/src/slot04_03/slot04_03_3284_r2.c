/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3530_slot04_03(Object *obj);

void func_801b3478_slot04_03(Object *obj) {
    obj->countdown = 0xc;
    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, 4);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    set_sequence_by_side_and_mode(obj, 0x19);
}

void func_801b34d8_slot04_03(Object *obj) {
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown != 0) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        func_801b3530_slot04_03(obj);
    }
}
