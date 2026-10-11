/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2cb4_slot04_06(Object *obj) {
    Object *p = obj->other;

    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        p->field_15b = 1;
        p->field_260 = 1;
        func_80140770(obj, 0, 0xc, 0x1a, 0, 0, 0);
        if ((s16)p->field_5c < 0) {
            obj->field_167 = (obj->field_12a >> 1) + 0xc;
            game_state.field_6b = 4;
            func_80147000(obj);
            play_sound_at(p, p->side, 0x34f);
        } else {
            play_sound_at(p, p->side, 0x34e);
        }
        obj->vel_x = 0xfffe0000;
        obj->vel_y = 0x44000;
        obj->accel_x = 0;
        obj->accel_y = -0x4800;
        obj->state_3++;
        obj->field_45 = 1;
        func_801307e0(obj, 0x26);
    }
}
