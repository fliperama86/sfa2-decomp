/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void hit_other_then_reset_on_sequence_end_slot04_06(Object *obj) {
    Object *other;

    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        other = obj->other;
        other->field_15b = 1;
        set_other_hit_reaction_state_0(obj, 0, 0xc, 3, 0, 0, 0);
        if ((s16)other->health < 0) {
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x16;
                obj->field_255 = 6;
                game_state.spawn_kind = 0;
                spawn_game_state_kind_object(obj);
            }
        }
        other->step_applied_flag = 0;
        obj->active_flag_4 = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}
