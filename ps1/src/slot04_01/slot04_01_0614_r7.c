/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b1b58_slot04_01(Object *obj);
void func_801b1eb8_slot04_01(Object *obj);

void func_801b1df8_slot04_01(Object *obj) {
    if (func_801b1b58_slot04_01(obj) != 0) {
        func_801b1eb8_slot04_01(obj);
    } else if ((s16)obj->step_flags & 0x8000) {
        obj->state_3++;
        func_80130678(obj, 0x21);
    } else {
        func_80130efc(obj);
    }
}

void func_801b1e70_slot04_01(Object *obj) {
    if (func_801b1b58_slot04_01(obj) == 0) {
        func_80130efc(obj);
    } else {
        func_801b1eb8_slot04_01(obj);
    }
}

void func_801b1eb8_slot04_01(Object *obj) {
    if (obj->field_45 != 0) {
        func_801209c4(obj);
    }
    obj->state_3++;
    obj->pos_y_frac = 0;
    obj->field_45 = 0;
    obj->field_159 = 0;
    obj->pos_y = obj->field_70;
    func_80130678(obj, 0x11);
}
