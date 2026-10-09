/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_800319b8_slot01[];
void func_8011f38c(Object *o);
void func_80012dec_slot01(Object *obj, u8 *a);

void func_80012d08_slot01(Object *obj) {
    if (obj->field_03 != 0) {
        func_80131094(obj);
        func_80120028(obj);
    } else {
        func_80012dec_slot01(obj, data_800319b8_slot01 + data_801a27d0 * 0xf0);
    }
}

void func_80012d7c_slot01(Object *obj) {
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
