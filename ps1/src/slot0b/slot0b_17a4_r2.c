/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern HudSlot data_801e7648_slot0b[];
extern void (*data_801e4d50_slot0b[])(Object *);
void func_801e1908_slot0b(Object *obj, Object *other);

void func_801e18d0_slot0b(u8 *src, u8 *dst) {
    if (*src != 0) {
        do {
            *dst = *src;
            src++;
            dst++;
        } while (*src != 0);
        *dst = 0;
    }
}

void func_801e1908_slot0b(Object *obj, Object *other) {
    HudSlot *s = &data_801e7648_slot0b[other->side];
    s->pos_x = obj->pos_x - 0x38;
    s->pos_y = obj->pos_y - 8;
}

void func_801e193c_slot0b(Object *obj) {
    Object *o = obj->field_3c;
    if (data_80190468.p->state_0 == 0) {
        data_801e4d50_slot0b[obj->state_1](obj);
        func_801e1908_slot0b(obj, o);
        if (data_80190a40 != 1) {
            func_801519b4(&data_801e7648_slot0b[o->side]);
        }
    } else {
        obj->state_0 = 2;
    }
}
