/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80015ce0_slot01[];

void func_800145b8_slot01(Object *object) {
    data_80015ce0_slot01[object->field_05](object);
    func_8011ffdc(object);
}

void func_8001460c_slot01(Object *obj) {
    if (game_state.field_ab != 0) {
        obj->pos_x = obj->field_5c;
        obj->field_04++;
    } else {
        int t = obj->field_46 - 1;
        obj->field_46 = t;
        if ((s16)t >= 0) {
            obj->pos_x = obj->pos_x - 0xc;
        } else {
            obj->field_46 = 0x14;
            obj->field_05++;
            obj->field_5c = obj->field_5c + obj->pos_x;
        }
    }
}

void func_80014690_slot01(Object *obj) {
    if (game_state.field_ab != 0) {
        obj->pos_x = obj->field_5c;
        obj->field_04++;
    } else {
        int t = obj->field_46 - 1;
        obj->field_46 = t;
        if ((s16)t < 0) {
            obj->pos_x += 8;
            if (obj->pos_x >= (s16)obj->field_5c) {
                obj->pos_x = (s16)obj->field_5c;
                obj->field_04++;
            }
        }
    }
}
