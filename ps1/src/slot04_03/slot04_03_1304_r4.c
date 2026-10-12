/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b15f4_slot04_03(Object *obj) {
    Object *p;
    s16 y;

    update_sequence(obj);
    if (obj->step_flags != 0) {
        obj->state_3++;
        p = pop_object_from_pool(obj);
        if (p != 0) {
            p->field_00 = 1;
            p->field_02 = 3;
            p->field_03 = 0;
            p->field_66 = obj->field_66;
            p->field_65 = obj->field_65;
            p->ground_y = 0;
            p->field_ac = obj->field_12a;
            p->field_ad = 0;
            p->field_0e = obj->field_0e;
            p->field_0b = obj->field_0b;
            p->field_0c = obj->field_0c;
            p->field_0d = obj->field_0d;
            p->field_26 = obj->field_26;
            p->pos_x = obj->pos_x;
            y = obj->pos_y;
            p->spawn_x = 0x60;
            p->health = 0;
            p->spawn_y = 0x1e0;
            p->pos_y = y;
            p->field_90 = obj->field_90;
            p->field_98 = obj->field_98;
            p->field_9c = obj->field_9c;
            p->field_3c = obj;
            obj->field_14c = (s32)p;
            obj->field_240++;
        }
    }
}

void func_801b171c_slot04_03(Object *obj) {
    s16 t = obj->step_flags;

    if (t == 0x100) {
        obj->step_flags = (u8)t;
        obj->field_17b = 0;
    }
    if ((s16)obj->step_flags & 0x8000) {
        func_801312b8(obj);
    } else {
        func_80142adc(obj);
        update_sequence(obj);
    }
}
