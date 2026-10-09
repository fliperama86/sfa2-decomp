/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_80028b8c_slot27[];

void func_80016634_slot27(Object *object) {
    data_80028b8c_slot27[object->field_05](object);
    func_8011ffdc(object);
}

void func_80016688_slot27(Object *obj) {
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

void func_8001670c_slot27(Object *obj) {
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
