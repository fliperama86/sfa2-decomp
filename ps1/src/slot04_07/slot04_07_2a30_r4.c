/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c2034_slot04_07[];

void func_801b3274_slot04_07(Object *obj);

void func_801b2ef4_slot04_07(Object *obj) {
    int a;

    update_sequence(obj);
    if ((u8)obj->step_flags == 0) {
        obj->active_state = 1;
        *(s32 *)&obj->vel_x = 0xa0000;
        obj->accel_x = -0x6000;
        obj->vel_y = 0x90000;
        obj->state_3++;
        obj->accel_y = -0xa000;
        a = 0;
        obj->field_165 = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 10;
            a = (obj->direction_index >> 1) + 1;
        }
        obj->other_lock = data_801c2034_slot04_07[a];
    }
}

void func_801b2fa4_slot04_07(Object *obj) {
    int y;

    func_801b3274_slot04_07(obj);
    if (obj->vel_y >= 0) {
        obj->state_3 = 5;
        obj->countdown = 0;
    } else {
        if (((Slot04aObj *)obj)->once_flag == 0) {
            ((Slot04aObj *)obj)->once_flag++;
            func_801307e0(obj, 0x21);
        }
        y = obj->ground_y;
        if (obj->pos_y < y) {
            update_sequence(obj);
        } else {
            obj->state_3++;
            obj->pos_y = y;
            obj->active_state = 0;
            obj->other_lock = 0;
            func_801307e0(obj, 0x2e);
        }
    }
}
