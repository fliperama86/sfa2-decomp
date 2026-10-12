/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3550_slot04_0b(Object *obj);

void func_801b349c_slot04_0b(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    func_80141f28(obj, 3);
    play_sound_at(obj, obj->side ^ 1, 0x31a);
    obj->gauge = 0x64;
    obj->field_46 = 0;
    obj->field_a1 = 0;
    func_801307e0(obj, 0x18);
}

void func_801b3504_slot04_0b(Object *obj) {
    if ((u8)obj->step_flags != 2) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        func_801b3550_slot04_0b(obj);
    }
}
