/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_8007985c_slot2b[];

void func_80077660_slot2b(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_80131468(obj);
    } else {
        if (func_80149b80(obj) & 0xff) {
            obj->state_3 = 0;
        }
        update_sequence(obj);
    }
}

void func_800776c4_slot2b(Object *obj) {
    data_8007985c_slot2b[obj->state_3](obj);
}

void func_80077704_slot2b(Object *obj) {
    obj->active_flag_3 = 1;
    obj->vel_x = 0xa0000;
    obj->state_3++;
    obj->accel_x = 0xffff4000;
    obj->facing_side = obj->saved_facing;
    if (obj->saved_facing == 0) {
        obj->vel_x = -obj->vel_x;
        obj->accel_x = -obj->accel_x;
    }
    func_80130dc0(obj);
}

void func_80077778_slot2b(Object *o) {
    Slot2bObj *obj = (Slot2bObj *)o;
    if (obj->active_flag != 0) {
        o->state_3++;
        play_sound_at(o, o->side, 0x324);
    }
    update_sequence(o);
}
