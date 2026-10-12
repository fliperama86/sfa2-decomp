/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2810_slot04_02(Object *obj) {
    int n;

    if ((u8)func_80130184(obj) != 0) {
        if ((s16)obj->step_flags & 0x8000) {
            obj->state_3++;
            obj->active_flag_3 = 0;
            set_sequence_by_side(obj, 0x21);
        } else {
            update_sequence(obj);
        }
    } else {
        obj->state_3 = 4;
        obj->active_flag_3 = 0;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        func_801209c4(obj);
        n = 0x5e;
        if (obj->variant_index != 1) {
            n = 0x5f;
        }
        func_801307e0(obj, n);
    }
}

void func_801b28bc_slot04_02(Object *obj) {
    int n;

    if ((u8)func_80130184(obj) == 0) {
        obj->state_3 = 4;
        obj->active_flag_3 = 0;
        obj->active_state = 0;
        obj->pos_y = obj->ground_y;
        func_801209c4(obj);
        n = 0x5e;
        if (obj->variant_index != 1) {
            n = 0x5f;
        }
        func_801307e0(obj, n);
    } else {
        update_sequence(obj);
    }
}
