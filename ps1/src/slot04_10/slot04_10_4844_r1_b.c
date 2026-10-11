/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80130184(Object *object);
int func_8013ffe4(Object *object, s16 a, s16 b, s16 c, u16 d);
void func_80145f98(Object *object);
void func_801b1078_slot04_10(Object *object);
void func_801b4ec0_slot04_10(Object *object);

void func_801b4d84_slot04_10(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 0;
        obj->field_45 = 1;
        func_801b4ec0_slot04_10(obj);
        if (obj->vel_y < 0) {
            obj->state_3++;
            obj->vel_x = 0;
            obj->accel_x = 0;
            func_801307e0(obj, 0x32);
        } else if (func_8013ffe4(obj, -0x3e, 0x18, 0x40, 0x1a) != 0) {
            play_sound_at(obj, obj->side ^ 1, 0x31a);
            func_80145f98(obj);
            if (obj->field_12a == 4) {
                play_side_sound_at(obj, obj->side, 7);
                obj->state_3 = 0xe;
                func_801307e0(obj, 0x37);
            } else if (obj->field_12a == 2) {
                obj->state_3 = 9;
                func_801307e0(obj, 0x34);
            } else {
                obj->state_3 = 6;
                func_801307e0(obj, 0x31);
            }
        }
    }
}

void func_801b4ec0_slot04_10(Object *obj) {
    if (obj->vel_x >= 0) {
        if (obj->field_0b == 0) {
            obj->vel_x = 0;
            obj->accel_x = 0;
        }
    } else if (obj->field_0b != 0) {
        obj->vel_x = 0;
        obj->accel_x = 0;
    }
    *(s32 *)&obj->pos_y_frac -= obj->vel_y;
    obj->vel_y += obj->accel_y;
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    obj->vel_x += obj->accel_x;
}

void func_801b4f48_slot04_10(Object *obj) {
    if ((u8)func_80130184(obj) != 0) {
        update_sequence(obj);
    } else {
        obj->state_3++;
        obj->field_45 = 0;
        obj->field_159 = 0;
        obj->pos_y = obj->field_70;
        *(s32 *)&obj->pos_y_frac &= 0xffff0000;
        func_801209c4(obj);
        func_801307e0(obj, 0x33);
    }
}

void func_801b4fc8_slot04_10(Object *obj) {
    if ((s16)obj->step_flags >= 0) {
        update_sequence(obj);
    } else {
        ref_other.p = obj->other;
        ref_other.p->field_249 = 5;
        func_801b1078_slot04_10(obj);
    }
}
