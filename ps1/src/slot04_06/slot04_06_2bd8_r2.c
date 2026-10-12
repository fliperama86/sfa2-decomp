/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void hit_other_with_sound_and_knockback_slot04_06(Object *obj) {
    Object *p = obj->other;

    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        p->field_15b = 1;
        p->hit_flag = 1;
        set_other_hit_reaction_state_0(obj, 0, 0xc, 0x1a, 0, 0, 0);
        if ((s16)p->health < 0) {
            obj->field_167 = (obj->direction_index >> 1) + 0xc;
            game_state.spawn_kind = 4;
            spawn_game_state_kind_object(obj);
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
        set_sequence_by_side_and_mode(obj, 0x26);
    }
}
