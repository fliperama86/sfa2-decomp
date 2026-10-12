/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3e68_slot04_0a(Object *obj);

void func_801b235c_slot04_0a(Object *obj) {
    int a;

    obj->state_3++;
    func_80141f28(obj, 2);
    func_80138ae8(&game_state, obj);
    play_side_sound_at(obj, obj->side, 0xf);
    a = 0x2e;
    if (obj->field_49 != 0) {
        a = 0x4b;
    }
    func_801307e0(obj, a);
}

void func_801b23d0_slot04_0a(Object *obj) {
    u16 t;

    update_sequence(obj);
    t = obj->step_flags;
    if ((t & 0xff) != 0) {
        obj->step_flags = t & 0xff00;
        func_801b3e68_slot04_0a(obj);
    }
    if ((s16)obj->step_flags & 0x8000) {
        obj->countdown = 0xc;
        obj->state_3++;
    }
}
