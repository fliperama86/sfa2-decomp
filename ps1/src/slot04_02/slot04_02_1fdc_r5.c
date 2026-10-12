/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b631c_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);

/* The call of pop_object_from_pool passes no argument although the callee takes one: the original does not set the first argument register before it. Written with the argument, this function differs from the original in 1 instruction slots. */
void func_801b2660_slot04_02(Object *obj) {
    Object *p;
    u16 t;

    if ((u8)func_80130184(obj) == 0) {
        obj->field_159 = 0;
        obj->field_45 = 0;
        obj->pos_y = obj->ground_y;
        func_801b631c_slot04_02(obj, 1, 0, 3, 2);
        func_801209c4(obj);
        func_80130678(obj, 0x11);
    } else {
        update_sequence(obj);
        if ((u8)obj->step_flags == 1) {
            obj->state_3++;
            obj->step_flags = obj->step_flags & 0xff00;
            p = ((Object *(*)(void))pop_object_from_pool)();
            if (p != 0) {
                p->field_00 = 1;
                p->field_02 = 2;
                p->field_03 = 2;
                p->field_ad = 0;
                p->health = 0;
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
                t = obj->ground_y;
                p->spawn_x = 0x60;
                p->field_3c = obj;
                p->spawn_y = 0x1e0;
                p->ground_y = t;
                p->field_90 = obj->field_90;
                p->field_98 = obj->field_98;
                p->field_9c = obj->field_9c;
                obj->field_14c = (s32)p;
                obj->accel_y = -0x2800;
                obj->field_240++;
                play_side_sound_at(obj, obj->side, 0xb);
            }
        }
    }
}
