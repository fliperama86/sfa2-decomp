/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void enter_state_add_meter_start_sequence_19_sound_slot04_08(Object *obj, Object *unused) {
    obj->state_3++;
    add_to_meter_clamped(obj, 3);
    play_sound_at(obj, obj->side, 0x31a);
    set_sequence_by_side_and_mode(obj, 0x19);
}

void hit_other_when_step_flag_byte_set_slot04_08(Object *obj, Object *unused) {
    if (*(u8 *)&obj->step_flags != 0) {
        obj->state_3++;
        obj->step_flags = obj->step_flags & 0xff00;
        play_side_sound_at(obj, obj->side, 0xd);
        set_other_hit_reaction_state_0(obj, 0, 0xf, 0x12, 0, 1, 0);
    }
    update_sequence(obj);
}
