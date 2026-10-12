/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3974_slot04_07(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    add_to_meter_clamped(obj, 3);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    if ((obj->input_bits & 0x8000) == 0) {
        obj->facing_side = 0;
    } else {
        obj->facing_side = 1;
    }
    set_sequence_by_side_and_mode(obj, 0x18);
}
