/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b16a0_slot04_00(Object *obj);
void func_801b1554_slot04_00(Object *obj);

void func_801b1438_slot04_00(Object *obj) {
    func_801b16a0_slot04_00(obj);
    if (obj->pos_y >= obj->ground_y) {
        func_801b1554_slot04_00(obj);
    } else {
        if (obj->field_0b == 0) {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac - obj->vel_x;
        } else {
            *(s32 *)&obj->pos_x_frac = *(s32 *)&obj->pos_x_frac + obj->vel_x;
        }
        obj->vel_x = obj->vel_x + obj->accel_x;
        if (obj->vel_x < 0) {
            obj->state_3++;
        }
        update_sequence(obj);
    }
}

void func_801b14ec_slot04_00(Object *obj) {
    func_801b16a0_slot04_00(obj);
    if (obj->pos_y >= obj->ground_y) {
        func_801b1554_slot04_00(obj);
    } else if (*(u8 *)&obj->step_flags == 0) {
        update_sequence(obj);
    }
}

void func_801b1554_slot04_00(Object *obj) {
    obj->state_3++;
    obj->field_45 = 0;
    obj->field_159 = 0;
    obj->field_17b = 0;
    obj->pos_y = (u16)obj->ground_y;
    func_801209c4(obj);
    update_sequence(obj);
}
