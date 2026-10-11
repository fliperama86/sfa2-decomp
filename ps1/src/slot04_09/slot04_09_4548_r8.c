/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4e3c_slot04_09(Object *obj, Object *unused) {
    obj->state_3++;
    if (obj->field_246 == 0) {
        func_80141f28(obj, 3);
    }
    play_sound_at(obj, obj->side ^ 1, 0x31b);
    func_801307e0(obj, 0x18);
    func_80141e5c(obj);
}
