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
        set_sequence_by_side(obj, 0x21);
    } else {
        update_sequence(obj);
    }
}

void func_801b1e70_slot04_01(Object *obj) {
    if (func_801b1b58_slot04_01(obj) == 0) {
        update_sequence(obj);
    } else {
        func_801b1eb8_slot04_01(obj);
    }
}

void func_801b1eb8_slot04_01(Object *obj) {
    if (obj->active_state != 0) {
        func_801209c4(obj);
    }
    obj->state_3++;
    obj->pos_y_frac = 0;
    obj->active_state = 0;
    obj->active_flag_3 = 0;
    obj->pos_y = obj->ground_y;
    set_sequence_by_side(obj, 0x11);
}
