/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80015b50_slot28(Object *obj) {
    if (--obj->field_4c == 0) {
        obj->field_4c = 0x20;
        obj->field_05 = 0;
        if (((u8 *)&obj->field_46)[1] == 2) {
            obj->field_01 = 0;
            obj->field_04++;
        } else {
            ((u8 *)&obj->field_46)[1]++;
            if (((u8 *)&obj->field_46)[1] == 2) {
                *(u16 *)&obj->pos_y -= 8;
            }
            func_80130768(obj, ((u8 *)&obj->field_46)[1] + 7, (SequenceStep **)obj->box_tables);
        }
    } else {
        obj->field_48 = obj->field_48 ^ 1;
        if (obj->field_48 != 0) {
            obj->field_01 = 1;
        } else {
            obj->field_01 = 0;
        }
    }
}

void func_80015c1c_slot28(Object *obj) {
    *(int *)&obj->field_10 += 0x4000;
    if (obj->pos_x > 0x180) {
        obj->field_04++;
    }
    obj->field_48 = obj->field_48 ^ 1;
    if (obj->field_48 != 0) {
        obj->field_01 = 0;
    } else {
        obj->field_01 = 1;
    }
    func_80131094(obj);
}
