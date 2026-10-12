/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void move_until_ground_then_damage_other_slot04_03(Object *obj) {
    *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    obj->vel_x = obj->vel_x + obj->accel_x;
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
    if (obj->ground_y > obj->pos_y) {
        update_sequence(obj);
    } else {
        obj->state_3 = obj->state_3 + 1;
        obj->pos_y = obj->ground_y;
        game_state.state_param = 0x30;
        spawn_effect_at_object_offset(obj, 2, 0, 0x40);
        if ((apply_damage_to_other_return_defeated(obj, 0x10, 0) & 0xff) != 0) {
            ref_other.p = obj->other;
            play_sound_at(ref_other.p, ref_other.p->side, 0x336);
        } else {
            ref_other.p = obj->other;
            play_sound_at(ref_other.p, ref_other.p->side, 0x304);
        }
        set_sequence_by_side_and_mode(obj, 0x1a);
    }
}

void set_other_hit_reaction_on_step_flags_then_update_slot04_03(Object *obj) {
    if (obj->step_flags != 0) {
        obj->state_3 = obj->state_3 + 1;
        set_other_hit_reaction_state_0(obj, 0, 0xa, -0x200, 0, 0, 0);
    }
    update_sequence(obj);
}
