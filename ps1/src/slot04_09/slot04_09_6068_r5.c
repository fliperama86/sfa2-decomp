/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_8011f38c(Object *o);

extern u8 data_801c7e24_slot04_09[];

void func_801b6550_slot04_09(Object *o, Object *p) {
    if ((s32)o == *(s32 *)&p->field_2c) {
        *(s32 *)&p->field_2c = 0;
    }
    func_8011f38c(o);
}

void func_801b6584_slot04_09(Object *obj, Object *parent, int i) {
    u16 x; int t;
    obj->field_0b = parent->field_0b;
    if (obj->field_0b != 0) {
        t = data_801c7e24_slot04_09[i * 2] - 0x20;
        x = parent->pos_x;
        x += t;
        obj->pos_x = x;
    } else {
        t = data_801c7e24_slot04_09[i * 2] - 0x20;
        x = parent->pos_x;
        x -= t;
        obj->pos_x = x;
    }
    t = data_801c7e24_slot04_09[i * 2 + 1] - 0x20;
    x = parent->pos_y;
    x -= t;
    obj->pos_y = x;
}
