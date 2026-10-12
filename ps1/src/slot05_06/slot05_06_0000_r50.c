/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801dd4e4_slot05_06[];

void func_801cc478_slot05_06(Object *obj) {
    obj->state_3++;
    obj->active_flag_4 = 1;
    ref_other.p = obj->other;
    apply_forced_state_to_other_object(obj);
    add_to_meter_clamped(obj, 6);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    play_sound_at(ref_other.p, ref_other.p->side, 0x31a);
    set_sequence_by_side_and_mode(obj, 0x19);
}

void hit_other_when_mode_set_slot05_06(Object *obj) {
    Object *p;

    if (((Slot04aObj *)obj)->mode != 0) {
        obj->state_3++;
        p = obj->other;
        p->field_15b = 1;
        set_other_hit_reaction_state_0(obj, 0x26, 5, *(s16 *)(data_801dd4e4_slot05_06 + (obj->direction_index & 0xfe)), 4, 0, 1);
        if ((s16)p->health < 0) {
            obj->field_167 = 2;
            if (obj->field_49 != 0) {
                obj->field_167 = 0x16;
                obj->field_255 = 6;
                game_state.spawn_kind = 0;
                spawn_game_state_kind_object(obj);
            }
        }
    }
    update_sequence(obj);
}

void update_sequence_or_end_clear_flag_4_2_slot05_06(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        obj->active_flag_4 = 0;
        reset_object_to_state_1_0_sequence_0(obj);
    }
}
