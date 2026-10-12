/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b6140_slot04_02(Object *object) {
    if (game_state.field_5c == 0) {
        object->state_2 = object->state_2 + 1;
    }
    update_sequence(object);
}

void func_801b617c_slot04_02(Object *obj) {
    int idx = 0x28;
    obj->state_2 = obj->state_2 + 1;
    obj->field_0b = obj->other->field_0b;
    obj->field_46 = 0x78;
    if (game_state.field_a6 != 0) {
        idx = 0x29;
    }
    set_sequence_by_side(obj, idx);
}

void func_801b61d4_slot04_02(Object *obj) {
    s16 t = obj->field_46;
    if (t != 0) {
        t = t - 1;
        obj->field_46 = t;
        if (t == 0) {
            game_state.field_4b |= 1 << obj->side;
        }
    }
    update_sequence(obj);
}
