/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b3e00_slot04_0a(Object *obj);

void func_801b2b60_slot04_0a(Object *obj) {
    s16 y;
    int a;
    func_801b3e00_slot04_0a(obj);
    y = obj->ground_y;
    if (obj->pos_y < y) {
        update_sequence(obj);
    } else {
        obj->pos_y = y;
        obj->pos_y_frac = 0;
        obj->field_45 = 0;
        obj->state_3 = 5;
        if (obj->field_12a == 0) {
            a = 0x36;
        } else {
            a = 0x37;
            if (obj->field_12a == 4) {
                obj->state_3 = 3;
                a = 0x38;
            }
        }
        func_801307e0(obj, a);
    }
}

void func_801b2bf4_slot04_0a(Object *obj) {
    int a;
    int b;
    if (((Slot04aObj *)obj)->field_3a != 0) {
        a = 0xe0000;
        b = -0x8000;
        obj->state_3++;
        if (obj->field_0b == 0) {
            a = 0xfff20000;
            b = 0x8000;
        }
        obj->vel_x = a;
        obj->accel_x = b;
    }
    update_sequence(obj);
}

void func_801b2c4c_slot04_0a(Object *obj) {
    SequenceStep *p;
    FrameRecord *fr;
    *(s32 *)&obj->pos_x_frac += obj->vel_x;
    obj->vel_x += obj->accel_x;
    if (obj->vel_x == 0) {
        obj->pos_x_frac = 0;
        obj->state_3++;
        p = obj->sequence;
        p++;
        obj->sequence = p;
        obj->step_timer = p->duration;
        fr = obj->frames + obj->sequence->frame_index;
        obj->frame = fr;
        obj->field_4a = fr->field_0d;
        obj->sequence_changed_flag = 1;
    }
    update_sequence(obj);
}
