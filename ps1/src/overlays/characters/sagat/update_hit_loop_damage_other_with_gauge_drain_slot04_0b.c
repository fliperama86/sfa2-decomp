/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../../../game.h"
#include "../../../protos.h"
#include "../../../externs.h"

void update_hit_loop_damage_other_with_gauge_drain_slot04_0b(Object *obj) {
    Object *o = obj->other;
    int a;

    obj->step_count = 0;
    if (*(u8 *)&obj->step_flags != 0) {
        obj->step_count = 1;
        obj->step_flags = obj->step_flags & 0xff00;
        spawn_effect_at_object_offset(obj, 1, -0x54, 0x55);
        a = 0;
        if (obj->offset_index == 0) {
            obj->offset_index++;
            a = 9;
        }
        if (apply_damage_to_other_return_defeated(obj, a, 0) != 0) {
            obj->state_3 = 4;
            play_sound_at(obj, o->side, 0x340);
            play_side_sound_at(obj, obj->side, 0xb);
            set_sequence_by_side_and_mode(obj, 0x19);
        }
        obj->countdown = obj->countdown + 1;
    }
    update_sequence_by_pressed_bytes(obj);
    if ((s16)obj->countdown != 0) {
        if (o->field_15b != 0) {
            if ((u8)drain_gauge_by_other_input(obj) == 0) {
                goto check;
            }
            spawn_effect_at_object_offset(obj, 1, -0x54, 0x55);
        }
        play_sound_at(obj, o->side, 0x30d);
        play_side_sound_at(obj, obj->side, 0xb);
        obj->state_3 = 3;
        set_sequence_by_side_and_mode(obj, 0x19);
    }
check:
    if (obj->step_count != 0) {
        play_sound_at(obj, o->side, 0x309);
        play_side_sound_at(obj, obj->side, 0xa);
    }
    update_sequence(obj);
}
