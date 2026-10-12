/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b2b50_slot04_0b(Object *obj) {
    obj->state_3++;
    func_801428e4(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    func_80145d20(obj);
    set_sequence_by_side_and_mode(obj, 0x2c);
}

void func_801b2bac_slot04_0b(Object *obj) {
    if ((s16)obj->step_flags & 0xff00) {
        obj->state_3++;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        spawn_children_per_direction_active_ff(obj, -0x10, 0x47);
        play_sound_at(obj, obj->side, 0x31c);
    }
    update_sequence(obj);
}

void set_other_lock_3_with_sounds_slot04_0b(Object *obj) {
    update_sequence(obj);
    if (((s16)obj->step_flags & 0xff00) == 0) {
        obj->field_165 = 0;
        obj->state_3++;
        play_side_sound_at(obj, obj->side, 0xf);
        play_side_sound_at(obj, obj->side, 4);
        obj->other_lock = 3;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
        }
    }
}
