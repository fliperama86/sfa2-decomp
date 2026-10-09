/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b223c_slot04_03(Object *obj);

void func_801b1fd0_slot04_03(Object *obj) {
    s16 t;

    if (obj->field_12a >= 3) {
        func_80130efc(obj);
        t = obj->field_3a;
        if (t & 0x8000) {
            if (obj->field_12a != 0) {
                obj->field_07++;
                func_801204f4(obj, obj->side, 5);
                func_801204f4(obj, obj->side, 0xd);
                obj->field_4c = 0xc0000;
                obj->field_54 = -0x8000;
                obj->field_50 = -0x90000;
                obj->field_58 = 0x6000;
                func_801307e0(obj, 0x38);
            } else {
                obj->field_07 = 6;
                func_801307e0(obj, 0x28);
            }
        } else if ((u8)t == 0) {
            func_801b223c_slot04_03(obj);
        }
    } else if ((u8)obj->field_3a == 0) {
        obj->field_45 = 1;
        func_801b223c_slot04_03(obj);
        *(s32 *)&obj->field_14 += obj->field_50;
        obj->field_50 += obj->field_58;
        if (obj->field_70 < obj->pos_y) {
            obj->pos_y = obj->field_70;
            obj->field_45 = 0;
            func_801209c4(obj);
            obj->field_07 = 6;
            func_801307e0(obj, 0x28);
        } else {
            func_80130efc(obj);
        }
    } else {
        func_80130efc(obj);
    }
}
