/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4128_slot04_07(Object *obj);

void func_801b3408_slot04_07(Object *obj) {
    obj->state_3 = obj->state_3 + 1;
    func_80142fbc(obj);
    func_801428a8(obj);
    func_80138b38(&game_state, obj);
    if (obj->field_45 != 0) {
        set_sequence_by_side(obj, 0x2f);
    } else {
        set_sequence_by_side(obj, 0x2b);
    }
}

void func_801b347c_slot04_07(Object *obj) {
    if ((s16)obj->step_flags & 0xff00) {
        obj->field_165 = 0xff;
        obj->field_46 = 0x1e01;
        obj->state_3 = obj->state_3 + 1;
        if (obj->field_45 != 0) {
            func_801495f8(obj, -2, 0x4a);
        } else {
            func_801495f8(obj, -0x1b, 0x69);
        }
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b4128_slot04_07(obj);
    }
    update_sequence(obj);
}
