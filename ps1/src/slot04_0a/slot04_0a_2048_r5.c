/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void store_image_rows_for_object_slot04_0a(Object *obj);

void advance_state_3_gain_meter_play_side_sound_slot04_0a(Object *obj) {
    int a;

    obj->state_3++;
    add_to_meter_clamped(obj, 2);
    add_one_to_ec4_if_object_unflagged(&game_state, obj);
    play_side_sound_at(obj, obj->side, 0xf);
    a = 0x2e;
    if (obj->field_49 != 0) {
        a = 0x4b;
    }
    set_sequence_by_side_and_mode(obj, a);
}

void update_sequence_then_check_step_flags_slot04_0a(Object *obj) {
    u16 t;

    update_sequence(obj);
    t = obj->step_flags;
    if ((t & 0xff) != 0) {
        obj->step_flags = t & 0xff00;
        store_image_rows_for_object_slot04_0a(obj);
    }
    if ((s16)obj->step_flags & 0x8000) {
        obj->countdown = 0xc;
        obj->state_3++;
    }
}
