/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b1938_slot04_0b(Object *obj) {
    int a = 0x1a;

    obj->field_17b = 1;
    obj->state_3++;
    func_80141f28(obj, 4);
    func_80138ae8(&game_state, obj);
    if (obj->field_49 != 0) {
        a = 0x39;
    }
    func_801307e0(obj, (obj->field_12a >> 1) + a);
}

void func_801b19b8_slot04_0b(Object *obj) {
    if (*(u8 *)&obj->step_flags == 1) {
        obj->state_3++;
        play_side_sound_at(obj, obj->side, 0xf);
        play_side_sound_at(obj, obj->side, 4);
    }
    update_sequence(obj);
}
