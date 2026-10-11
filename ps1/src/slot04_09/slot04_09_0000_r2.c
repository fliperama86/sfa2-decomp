/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b5588_slot04_09(Object *obj);

void func_801b019c_slot04_09(Object *obj) {
    obj->field_45 = 1;
    obj->state_2 = obj->state_2 + 1;
    func_801b5588_slot04_09(obj);
    play_side_sound_at(obj, obj->side, 0x10);
    func_80130678(obj, 0x22);
}

void func_801b01f4_slot04_09(Object *obj) {
    if (((s16)obj->step_flags & 0x8000) != 0) {
        obj->state_0 = 1;
        obj->state_1 = 0;
        obj->state_2 = 0;
        obj->state_3 = 0;
        obj->field_45 = 0;
        obj->pos_y_frac = 0;
        obj->pos_y = obj->field_70;
        func_80130678(obj, 0);
    } else {
        update_sequence(obj);
    }
}
