/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void repeat_hit_other_until_gauge_drained_slot04_08(Object *obj, Object *unused) {
    int a;
    Object **q;
    Object **r = &ref_other.p;
    u16 t;
    *r = obj->other;
    t = obj->step_flags;
    if ((u8)t != 0) {
        obj->step_flags = t & 0xff00;
        spawn_effect_at_object_offset(obj, 1, -0x1d, 0x52);
        a = 1;
        if (((Slot04aObj *)obj)->done_flag == 0) {
            ((Slot04aObj *)obj)->done_flag = 0xff;
            a = 0xa;
        }
        if ((u8)apply_damage_to_other_return_defeated(obj, a, 0) != 0) {
            set_other_hit_reaction_state_0(obj, 0, 5, 0, 0, 0, 1);
            play_sound_at(game_state.other_object, ref_other.p->side, 0x336);
            goto tail;
        }
        obj->countdown++;
        play_side_sound_at(obj, obj->side, 0xc);
        q = &game_state.other_object;
        play_sound_at(*q, ref_other.p->side, 0x304);
        update_sequence_by_pressed_bytes(obj);
        if ((s16)obj->countdown != 0) {
            *q = obj->other;
            if (game_state.other_object->field_15b == 0 || (u8)drain_gauge_by_other_input(obj) != 0) {
                goto act;
            }
        }
        update_sequence(obj);
        return;
    }
    update_sequence_by_pressed_bytes(obj);
    if ((s16)obj->countdown != 0) {
        *r = obj->other;
        if (ref_other.p->field_15b == 0 || (u8)drain_gauge_by_other_input(obj) != 0) {
act:
            set_other_hit_reaction_state_0(obj, 0, 5, 0, 0, 0, 1);
            play_side_sound_at(obj, obj->side, 0xd);
tail:
            obj->state_3++;
            set_sequence_by_side_and_mode(obj, 0x1b);
            return;
        }
    }
    update_sequence(obj);
}
