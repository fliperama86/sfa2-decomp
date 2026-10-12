/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b29c8_slot04_01(Object *obj) {
    int a = 0x3e;

    obj->active_flag_4 = 1;
    obj->state_3++;
    if (obj->field_49 == 0) {
        obj->remaining_count--;
    }
    obj->active_flag_3 = 0;
    obj->field_157 = 0;
    if (obj->input_bits_2 & 0x8000) {
        a = 0x3f;
    }
    func_801307e0(obj, a);
}

void func_801b2a30_slot04_01(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        func_801312b8(obj);
    } else {
        if (t & 0xff) {
            obj->step_flags = t & 0xff00;
            play_side_sound_at(obj, obj->side, 9);
        }
        update_sequence(obj);
    }
}
