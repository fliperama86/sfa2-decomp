/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801e766c_slot0b[];
void func_801e0ea8_slot0b(Object *obj, u8 *a);

void func_801e01dc_slot0b(Object *obj) {
    obj->pos_x = ref_other.p->pos_x + ((u16 *)&obj->field_4c)[1];
    obj->pos_y = ref_other.p->pos_y + ((u16 *)&obj->field_4c)[1];
    if (obj->field_03 == 0 && data_80190a40 != 1) {
        func_801e0ea8_slot0b(obj, data_801e766c_slot0b + obj->field_48 * 0x1e0 + data_801a27d0 * 0xf0);
    }
}

void func_801e0284_slot0b(Object *obj) {
    Object **p = (Object **)obj->field_3c->field_28;
    int n;

    for (n = 4; n > 0; n--) {
        if (obj == *p) {
            *p = 0;
            break;
        }
        p++;
    }
    func_8011f38c(obj);
}

void func_801e02d4_slot0b(Object *obj) {
    func_801e0284_slot0b(obj);
}
