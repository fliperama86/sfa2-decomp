/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801cc814_slot05_06(Object *obj);
void func_801cc84c_slot05_06(Object *obj);

void func_801cacb4_slot05_06(Object *obj) {
    Object *p = obj->other;

    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        p->field_15b = 1;
        p->hit_flag = 1;
        func_80140770(obj, 0, 0xc, 0x1a, 0, 0, 0);
        if ((s16)p->health < 0) {
            obj->field_167 = (obj->direction_index >> 1) + 0xc;
            game_state.spawn_kind = 4;
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
        obj->active_state = 1;
        func_801307e0(obj, 0x26);
    }
}

void func_801cadc0_slot05_06(Object *obj) {
    func_801cc84c_slot05_06(obj);
    if (obj->pos_y < obj->ground_y) {
        func_801cc814_slot05_06(obj);
    } else {
        obj->active_state = 0;
        obj->pos_y = (u16)obj->ground_y;
        func_801209c4(obj);
        func_801312b8(obj);
    }
}
