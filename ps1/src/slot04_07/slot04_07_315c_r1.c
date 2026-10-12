/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3274_slot04_07(Object *obj);
void func_801b3120_slot04_07(Object *obj);
void func_801b320c_slot04_07(Object *obj);

void func_801b315c_slot04_07(Object *obj) {
    func_801b3274_slot04_07(obj);
    if (((Slot04aObj *)obj)->field_1cc == 0 && obj->field_67 != 0) {
        obj->vel_x = 0x10000;
        obj->vel_y = 0x10000;
        obj->accel_x = 0;
        obj->accel_y = 0;
        ((Slot04aObj *)obj)->field_1cc++;
    }
    if ((s16)obj->step_flags & 0x8000) {
        if (obj->field_67 == 0) {
            func_801b3120_slot04_07(obj);
            return;
        }
        obj->state_3 = 5;
        obj->field_46 += 2;
    }
    update_sequence(obj);
}

void func_801b320c_slot04_07(Object *obj) {
    int t;

    func_801b3274_slot04_07(obj);
    t = obj->ground_y;
    if (obj->pos_y < t) {
        update_sequence(obj);
    } else {
        obj->state_3 = 4;
        obj->pos_y = t;
        obj->field_45 = 0;
        func_801307e0(obj, 0x2e);
    }
}

void func_801b3274_slot04_07(Object *obj) {
    if (obj->field_0b != 0) {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
    } else {
        *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
    }
    obj->vel_x = obj->vel_x + obj->accel_x;
    if (obj->vel_x < 0) {
        obj->vel_x = 0;
        obj->accel_x = 0;
    }
    *(s32 *)&obj->pos_y_frac = *(s32 *)&obj->pos_y_frac - obj->vel_y;
    obj->vel_y = obj->vel_y + obj->accel_y;
}
