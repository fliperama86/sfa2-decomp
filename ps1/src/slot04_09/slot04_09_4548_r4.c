/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b5348_slot04_09(Object *obj);

void func_801b484c_slot04_09(Object *obj) {
    int a = 0x2b;
    obj->state_3 = obj->state_3 + 1;
    func_80142fbc(obj);
    func_801428a8(obj);
    func_80138b38(&game_state, obj);
    play_side_sound_at(obj, obj->side, 7);
    if (obj->active_state != 0) {
        a = 0x2f;
    }
    set_sequence_by_side(obj, a);
}

void func_801b48d4_slot04_09(Object *obj) {
    int a;
    if ((s16)obj->step_flags & 0xff00) {
        a = 0xa;
        obj->field_165 = 0xff;
        obj->countdown = 0x1e01;
        obj->state_3 = obj->state_3 + 1;
        if (obj->active_state != 0) {
            a = -7;
        }
        func_801495f8(obj, a, 0x50);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b5348_slot04_09(obj);
    }
    update_sequence(obj);
}
