/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80013a24_slot01(Object *obj);

void func_800138a4_slot01(Object *obj) {
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

void func_80013908_slot01(Object *obj) {
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
    func_80013a24_slot01(obj);
}

void func_8001398c_slot01(Object *obj) {
    if (*(u16 *)&obj->field_3c->field_04 == 0x101) {
        obj->field_04++;
    }
}

void func_800139bc_slot01(Object *obj) {
}
