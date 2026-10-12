/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void stop_x_motion_against_facing_then_move_slot04_09(Object *obj);
void func_801b5ba0_slot04_09(Object *obj);

void func_801b3040_slot04_09(Object *obj) {
    Object *p;

    stop_x_motion_against_facing_then_move_slot04_09(obj);
    func_801b5ba0_slot04_09(obj);
    p = obj->other;
    if (obj->vel_y > 0) {
        if (p->field_163 == 0 && p->active_state != 0 && (u8)func_8013fd98(obj, -0x20, 0x20, 0, 0x30) != 0) {
            obj->step_count = 0xff;
            obj->countdown = 4;
            obj->state_3++;
            if (obj->vel_x >= 0) {
                obj->vel_x = 0x80000;
            } else {
                obj->vel_x = 0xfff80000;
            }
            play_sound_at(obj, obj->side ^ 1, 0x31a);
            play_side_sound_at(obj, obj->side, 5);
            set_sequence_by_side_and_mode(obj, 0x21);
        } else {
            update_sequence(obj);
        }
    } else {
        obj->state_3 = 7;
        obj->vel_x = 0;
        obj->accel_x = 0;
        set_sequence_by_side_and_mode(obj, 0x23);
    }
}
