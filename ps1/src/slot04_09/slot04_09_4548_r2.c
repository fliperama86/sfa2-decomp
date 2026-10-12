/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4688_slot04_09(Object *obj) {
    obj->active_flag_4 = 1;
    obj->state_3++;
    obj->field_157 = 0;
    if (obj->field_49 == 0) {
        obj->remaining_count--;
    }
    play_side_sound_at(obj, obj->side, 0xd);
    func_801307e0(obj, 0x34);
}
