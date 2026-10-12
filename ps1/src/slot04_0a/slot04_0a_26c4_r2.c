/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3f10_slot04_0a(Object *obj);

void func_801b2804_slot04_0a(Object *obj) {
    u8 t;
    Slot04aObj *s = (Slot04aObj *)obj;
    if (obj->active_flag != 0) {
        obj->active_flag = 0;
        t = s->countdown;
        if ((t & 0x80) == 0) {
            s->countdown = t - 1;
            if (s->countdown & 0x80) {
                func_801307e0(obj, 0x42);
            }
        }
    }
    obj->countdown = (s16)obj->countdown - 1;
    if ((s16)obj->countdown >= 0 && obj->facing_side != obj->bound_push_flag - 1) {
        *(s32 *)&obj->pos_x_frac += obj->vel_x;
        update_sequence(obj);
    } else {
        obj->state_3++;
        func_801b3f10_slot04_0a(obj);
        func_801307e0(obj, (obj->direction_index >> 1) + 0x43);
    }
}

void func_801b28e0_slot04_0a(Object *obj) {
    if (((s16)obj->step_flags & 0x8000) == 0) {
        update_sequence(obj);
    } else {
        obj->pos_y_frac = 0;
        obj->active_state = 0;
        obj->pos_y = ((Slot04aObj *)obj)->saved_pos_y;
        func_801312b8(obj);
    }
}
