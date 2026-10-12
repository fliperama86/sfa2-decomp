/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2600_slot04_03(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        obj->state_3 = 0xb;
        func_801307e0(obj, 0x2f);
    } else if (t == 0 && (obj->pressed_bits & 0x95) != 0) {
        obj->state_3++;
        play_side_sound_at(obj, obj->side, 6);
        play_side_sound_at(obj, obj->side, 0xc);
        func_801307e0(obj, 0x2c);
    } else {
        update_sequence(obj);
    }
}

void func_801b26ac_slot04_03(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        obj->state_3 = 0xb;
        func_801307e0(obj, 0x30);
    } else if (t == 0) {
        if (obj->direction_index == 0) {
            obj->state_3 = 0xb;
            func_801307e0(obj, 0x30);
        } else if ((obj->pressed_bits & 0x95) != 0) {
            obj->state_3++;
            play_side_sound_at(obj, obj->side, 6);
            play_side_sound_at(obj, obj->side, 0xc);
            func_801307e0(obj, 0x2d);
        } else {
            update_sequence(obj);
        }
    } else {
        update_sequence(obj);
    }
}
