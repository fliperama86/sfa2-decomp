/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

int func_80130184(Object *object);
void func_80130678(Object *object, int arg);
void func_801b631c_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

/* The call of func_8011f0e8 passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 1 instruction slots. */
void func_801b2660_slot04_02(Object *obj) {
    Object *p;
    u16 t;

    if ((u8)func_80130184(obj) == 0) {
        obj->field_159 = 0;
        obj->field_45 = 0;
        obj->pos_y = obj->field_70;
        func_801b631c_slot04_02(obj, 1, 0, 3, 2);
        func_801209c4(obj);
        func_80130678(obj, 0x11);
    } else {
        func_80130efc(obj);
        if ((u8)obj->field_3a == 1) {
            obj->field_07++;
            obj->field_3a = obj->field_3a & 0xff00;
            p = ((Object *(*)(void))func_8011f0e8)();
            if (p != 0) {
                p->field_00 = 1;
                p->field_02 = 2;
                p->field_03 = 2;
                p->field_ad = 0;
                p->field_5c = 0;
                p->field_66 = obj->field_66;
                p->field_65 = obj->field_65;
                p->field_ac = (obj->field_12a >> 1) + 3;
                p->field_0b = obj->field_0b;
                p->field_0e = obj->field_0e;
                p->field_0c = obj->field_0c;
                p->field_0d = obj->field_0d;
                p->field_26 = obj->field_26;
                p->pos_x = obj->pos_x;
                p->pos_y = obj->pos_y;
                t = obj->field_70;
                p->field_7a = 0x60;
                p->field_3c = obj;
                p->field_7c = 0x1e0;
                p->field_70 = t;
                p->field_90 = obj->field_90;
                p->field_98 = obj->field_98;
                p->field_9c = obj->field_9c;
                obj->field_14c = (s32)p;
                obj->field_58 = -0x2800;
                obj->field_240++;
                func_801204f4(obj, obj->side, 0xb);
            }
        }
    }
}
