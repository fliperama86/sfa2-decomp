/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4c6c_slot04_04(Object *obj) {
    Object *p;
    SequenceStep *s;

    obj->field_44 = 1;
    p = obj->field_3c;
    obj->state_0++;
    obj->field_45 = 0;
    if (obj->field_0b != 0) {
        obj->vel_x = 0x10000;
    } else {
        obj->vel_x = 0xffff0000;
    }
    if (obj->field_03 == 0) {
        if (obj->field_0b != 0) {
            obj->pos_x = obj->pos_x + 0x80;
        } else {
            obj->pos_x = obj->pos_x - 0x80;
        }
        obj->accel_y = obj->pos_x;
        s = seqs_8017c7f8[15];
    } else {
        obj->accel_y = obj->pos_x;
        if (p->side == 0) {
            s = *data_1f8000b4;
        } else {
            s = *data_1f800164;
        }
    }
    func_80130700(obj, s);
}

void func_801b4d58_slot04_04(Object *obj) {
    if (game_state.field_65 == 0) {
        *(s32 *)&obj->pos_x_frac += obj->vel_x;
        if ((u32)(obj->pos_x - obj->accel_y + 0x20) >= 0x40) {
            obj->state_0++;
        }
    }
    func_80131094(obj);
    func_8011ff74(obj);
    func_8011ffdc(obj);
}
