/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

Block172 *func_8011f1e0(void);
void func_800e7f90_slot0f(u8 *src, u8 *dst, int mode) {
    while (*src != 0) {
        *dst++ = *src++;
    }
    switch (mode) {
    case 0:
        break;
    case 1:
        *dst++ = 'P';
        *dst++ = 'T';
        *dst++ = 'S';
        break;
    case 2:
        *dst++ = 'W';
        *dst++ = 'I';
        *dst++ = 'N';
        break;
    case 3:
        *dst++ = 'W';
        *dst++ = 'I';
        *dst++ = 'N';
        *dst++ = 'S';
        break;
    }
    *dst++ = '@';
    *dst++ = '@';
    *dst = 0;
}

void func_800e8064_slot0f(Object *obj, int arg) {
    Object *b;
    if (ptr_8019040c->field_0a != 0) {
        b = (Object *)func_8011f1e0();
        if (b != 0) {
            b->field_00 = 1;
            b->field_02 = 0x97;
            b->field_48 = 2;
            b->field_09 = 0;
            b->pos_x = obj->field_7a;
            b->pos_y = obj->field_7c;
            b->field_03 = 0;
            b->field_5c = obj->field_5c;
        }
    }
}
