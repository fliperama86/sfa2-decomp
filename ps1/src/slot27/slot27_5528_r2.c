/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80028510_slot27[])(Object *obj);
void func_80015838_slot27(Object *obj);

void func_80015650_slot27(Object *object) {
    data_80028510_slot27[object->field_05](object);
    func_8011ffdc(object);
}

void func_800156a4_slot27(Object *obj) {
    int a = game_state.field_d4;
    int d = a - obj->pos_y;
    if (d > 0) {
        if (d >= 9) {
            d = 8;
        }
        obj->pos_y = obj->pos_y + d;
        if (obj->pos_y < a) {
            return;
        }
    }
    obj->field_05++;
    obj->pos_y = a;
}

void func_80015708_slot27(Object *obj) {
    s16 *p = &game_state.field_d2;
    int a = *p;
    if (obj->pos_x - a > 0) {
        obj->pos_x = obj->pos_x - 4;
        if (a < obj->pos_x) {
            return;
        }
    } else {
        obj->pos_x = obj->pos_x + 4;
        if (obj->pos_x < a) {
            return;
        }
    }
    obj->field_05++;
    obj->pos_x = *p;
    func_80015838_slot27(obj);
}
