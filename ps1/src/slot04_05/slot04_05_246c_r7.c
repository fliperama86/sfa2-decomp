/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int move_x_by_vel_x_facing_and_accelerate_slot04_05(Object *object);

void update_sequence_then_set_vel_x_on_flag_2_slot04_05(Object *obj) {
    u8 t;
    update_sequence(obj);
    t = obj->step_flags;
    if (t == 2) {
        obj->vel_x = 0xa0000;
        obj->state_3++;
    } else if (t != 0) {
        if (obj->vel_x >= 0) {
            move_x_by_vel_x_facing_and_accelerate_slot04_05(obj);
        }
    }
}

void update_slide_with_input_branch_or_start_jump_slot04_05(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    s16 t = o->step_flags;
    if (t & 0x8000) {
        o->state_3++;
        if (o->facing_side != 0) {
            o->vel_x = 0x40000;
        } else {
            o->vel_x = 0xfffc0000;
        }
        o->vel_y = 0x90000;
        o->accel_y = -0x7000;
        o->accel_x = 0;
        o->active_state = 1;
        set_sequence_by_side_and_mode(o, 0x30);
    } else {
        if ((u8)t != 0) {
            switch ((u8)t) {
            default:
            dflt:
                if (o->vel_x >= 0) {
                    move_x_by_vel_x_facing_and_accelerate_slot04_05(o);
                }
                break;
            case 3:
                o->vel_x = 0x40000;
                o->accel_x = 0xfffff000;
                break;
            case 4:
                goto c4;
            }
        }
        update_sequence(o);
        return;
    c4:
        if (o->mode_flag_2 != 0) {
            obj->field_1a6 = o->input_dir_second;
        }
        if (obj->field_1a6 == 0) {
            goto dflt;
        }
        obj->field_1a5 = 0x18;
        o->state_3 += 3;
    }
}
