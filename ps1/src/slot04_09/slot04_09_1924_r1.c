/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c7c44_slot04_09[];
void func_801b536c_slot04_09(Object *obj);

void func_801b1924_slot04_09(Object *obj) {
    int n = 0x25;

    obj->active_flag_4 = 1;
    obj->field_225 = 1;
    obj->state_3++;
    if (obj->counter == 0) {
        obj->field_225 = 0;
        func_80141f28(obj, 4);
    }
    func_80138ae8(&game_state, obj);
    play_side_sound_at(obj, obj->side, 4);
    if (obj->field_49 != 0) {
        n = 0x43;
    }
    func_801307e0(obj, (obj->direction_index >> 1) + n);
}

void func_801b19c8_slot04_09(Object *obj) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        obj->step_flags = obj->step_flags & 0xff00;
        func_801b536c_slot04_09(obj);
        obj->countdown = data_801c7c44_slot04_09[obj->direction_index >> 1];
    }
    update_sequence(obj);
}

void func_801b1a44_slot04_09(Object *obj) {
    s16 t;

    if ((s16)obj->step_flags & 0x8000) {
        obj->variant_index_2 = 0;
        func_801312b8(obj);
    } else {
        t = obj->countdown;
        if (t != 0) {
            t = t - 1;
            obj->countdown = t;
            if (t != 0) {
                goto tail;
            }
            obj->active_flag_4 = 0;
        }
        func_80142adc(obj);
    tail:
        update_sequence(obj);
    }
}
