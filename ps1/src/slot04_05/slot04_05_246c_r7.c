/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_801b3048_slot04_05(Object *object);

void func_801b2e04_slot04_05(Object *obj) {
    u8 t;
    update_sequence(obj);
    t = obj->step_flags;
    if (t == 2) {
        obj->vel_x = 0xa0000;
        obj->state_3++;
    } else if (t != 0) {
        if (obj->vel_x >= 0) {
            func_801b3048_slot04_05(obj);
        }
    }
}

void func_801b2e74_slot04_05(Object *o) {
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
        func_801307e0(o, 0x30);
    } else {
        if ((u8)t != 0) {
            switch ((u8)t) {
            default:
            dflt:
                if (o->vel_x >= 0) {
                    func_801b3048_slot04_05(o);
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
