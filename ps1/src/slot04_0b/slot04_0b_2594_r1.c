/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c2608_slot04_0b[];

void func_801b2594_slot04_0b(Object *obj) {
    obj->accel_y = -0x3000;
    obj->vel_y = 0x20000;
    obj->accel_x = 0;
    obj->state_3++;
    if (obj->facing_side == 0) {
        obj->vel_x = -0x80000;
    } else {
        obj->vel_x = 0x80000;
    }
    func_801428e4(obj);
    set_flag_225_add_direction_to_counter(&game_state, obj);
    func_80145d20(obj);
    set_sequence_by_side_and_mode(obj, (obj->direction_index >> 1) + 0x3f);
}

void func_801b2624_slot04_0b(Object *obj) {
    if ((s16)obj->step_flags & 0xff00) {
        obj->state_3++;
        if (obj->field_4b != 0) {
            obj->field_165 = 1;
        } else {
            obj->field_165 = 0xff;
        }
        spawn_children_per_direction_index(obj, -0x28, 0x62);
        play_sound_at(obj, obj->side, 0x31c);
        play_side_sound_at(obj, obj->side, 7);
    }
    update_sequence(obj);
}

void set_other_lock_from_table_slot04_0b(Object *obj) {
    update_sequence(obj);
    if (((s16)obj->step_flags & 0xff00) == 0) {
        obj->field_165 = 0;
        obj->state_3++;
        obj->other_lock = data_801c2608_slot04_0b[obj->direction_index >> 1];
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
        }
    }
}
