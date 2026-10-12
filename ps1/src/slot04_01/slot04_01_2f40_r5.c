/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void update_repeated_hit_with_gauge_drain_slot04_01(Object *obj) {
    u16 t = obj->step_flags;
    Object *p = obj->other;

    if ((u8)t != 0) {
        obj->step_flags = t & 0xff00;
        obj->state_3++;
        spawn_effect_at_object_offset(obj, 1, -0x3e, 0x39);
        play_side_sound_at(obj, ((Slot04aObj *)obj)->side, 8);
        if ((u8)apply_damage_to_other_return_defeated(obj, 8, 0) != 0) {
            set_other_hit_reaction_state_0(obj, 1, 5, 0, 0, 0, 0);
            play_sound_at(p, ((Slot04aObj *)p)->side, 0x30d);
            goto done;
        }
        obj->countdown++;
        play_sound_at(p, ((Slot04aObj *)p)->side, 0x30b);
    }
    update_sequence_by_pressed_bytes(obj);
    if ((s16)obj->countdown == 0 || (p->field_15b != 0 && (u8)drain_gauge_by_other_input(obj) == 0)) {
        update_sequence(obj);
        return;
    }
    spawn_effect_at_object_offset(obj, 1, -0x3e, 0x39);
    set_other_hit_reaction_state_0(obj, 1, 5, 8, 0, 0, 1);
    play_sound_at(obj, ((Slot04aObj *)obj)->side, 0x30d);
done:
    obj->state_3 = 3;
    play_side_sound_at(obj, ((Slot04aObj *)obj)->side, 7);
    set_sequence_by_side_and_mode(obj, 0x2f);
}
