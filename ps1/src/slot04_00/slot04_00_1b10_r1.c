/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b1c5c_slot04_00(Object *obj);

void func_801b1b10_slot04_00(Object *obj) {
    if ((s16)obj->step_flags & 0x8000) {
        play_sound_at(obj, obj->side, 0x320);
        ((Slot04aObj *)obj)->field_1a4--;
        if (((Slot04aObj *)obj)->field_1a4 & 0x80) {
            obj->state_3++;
            func_801307e0(obj, 0x1f);
            return;
        }
    }
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    update_sequence(obj);
}

void func_801b1ba4_slot04_00(Object *obj) {
    if (func_801b1c5c_slot04_00(obj) != 0) {
        func_801209c4(obj);
        obj->pos_y_frac = 0;
        obj->field_45 = 0;
        obj->pos_y = obj->ground_y;
        obj->state_3++;
        set_sequence_by_side(obj, 0x11);
    } else {
        update_sequence(obj);
    }
}
