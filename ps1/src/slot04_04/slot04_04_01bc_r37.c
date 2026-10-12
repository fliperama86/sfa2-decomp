/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3530_slot04_04(Object *obj);

void func_801b3294_slot04_04(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    func_80142fbc(obj);
    func_801428a8(obj);
    func_80138b38(&game_state, obj);
    if (obj->active_state != 0) {
        set_sequence_by_side(obj, 0x2f);
    } else {
        play_side_sound_at(obj, obj->side, 8);
        set_sequence_by_side(obj, 0x2b);
    }
}

void func_801b3318_slot04_04(Object *obj) {
    s16 a = 0;
    int b = 0x3b;
    if ((s16)obj->step_flags & 0xff00) {
        obj->field_165 = 0xff;
        obj->countdown = 0x1e01;
        obj->state_3 = obj->state_3 + 1;
        if (obj->active_state != 0) {
            a = -3;
            b = 0x50;
        } else {
            a = -7;
            b = 0x47;
        }
        func_801495f8(obj, a, b);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b3530_slot04_04(obj);
    }
    update_sequence(obj);
}
