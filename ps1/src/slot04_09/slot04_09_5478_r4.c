/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

Block172 *func_8011f1e0(void);

void func_801b58dc_slot04_09(Object *obj) {
    Object *n;

    n = (Object *)func_8011f1e0();
    if (n != 0) {
        n->field_00 = 1;
        n->field_02 = 0xc;
        n->field_08 = 0x20;
        n->field_03 = 0x10;
        n->field_0e = obj->field_0e;
        n->field_0c = obj->field_0c;
        n->field_0d = obj->field_0d;
        n->field_26 = obj->field_26;
        n->field_3c = obj;
        n->pos_x = obj->pos_x;
        n->pos_y = obj->pos_y;
        n->field_0b = obj->field_0b;
        n->field_90 = obj->field_90;
        n->field_98 = obj->field_98;
        n->field_9c = obj->field_9c;
        n->field_7a = obj->field_7a;
        n->field_7c = obj->field_7c;
        n->field_66 = obj->side;
    }
}
