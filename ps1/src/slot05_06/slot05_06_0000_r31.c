/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801ca7a4_slot05_06(Object *obj) {
    Slot04aObj *s = (Slot04aObj *)obj;
    s32 a;

    s->field_1c2 = s->field_1c2 - 1;
    if (s->field_1c2 & 0x80) {
        obj->state_3 = obj->state_3 + 1;
        play_side_sound_at(obj, obj->side, 5);
        obj->vel_y = -0x100000;
        obj->vel_x = 0;
        obj->accel_x = 0;
        set_sequence_by_side_and_mode(obj, 0x24);
    } else {
        a = 0x4000;
        if (obj->facing_side == 0) {
            a = -0x4000;
        }
        *(s32 *)&obj->pos_x_frac += a;
        *(s32 *)&obj->pos_y_frac += -0x4000;
        update_sequence(obj);
    }
}
