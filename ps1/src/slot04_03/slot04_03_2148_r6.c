/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2774_slot04_03(Object *obj) {
    s16 t = obj->step_flags;

    if (t & 0x8000) {
        obj->state_3 = 0xb;
        set_sequence_by_side_and_mode(obj, 0x31);
    } else if (t == 0) {
        if (obj->direction_index < 4) {
            obj->state_3 = 0xb;
            set_sequence_by_side_and_mode(obj, 0x31);
        } else if ((obj->pressed_bits & 0x95) != 0) {
            obj->state_3++;
            play_side_sound_at(obj, obj->side, 6);
            play_side_sound_at(obj, obj->side, 0xc);
            set_sequence_by_side_and_mode(obj, 0x2e);
        } else {
            update_sequence(obj);
        }
    } else {
        update_sequence(obj);
    }
}
