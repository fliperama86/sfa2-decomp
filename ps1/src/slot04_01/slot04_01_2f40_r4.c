/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b321c_slot04_01(Object *obj) {
    obj->gauge = 0xb4;
    obj->field_46 = 0;
    obj->state_3++;
    func_80141f28(obj, 3);
    play_sound_at(obj, ((Slot04aObj *)obj)->side ^ 1, 0x31a);
    func_801307e0(obj, 0x18);
}
