/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801c7ce0_slot04_11[];
extern ObjectRef data_80190458;
extern ObjectRef data_80190414;

void func_8011f38c(Object *o);
void func_801b73f4_slot04_11(Object *obj);
void func_801b735c_slot04_11(Object *obj);

void func_801b725c_slot04_11(Object *obj) {
    ref_other.p = obj->field_3c;
    data_80190458.p = ref_other.p->field_3c;
    data_80190414.p = obj->other;
    data_801c7ce0_slot04_11[obj->field_04](obj);
}

void func_801b72d8_slot04_11(Object *obj) {
    obj->field_01 = 1;
    obj->field_0d = 2;
    obj->field_0c = 0;
    obj->field_20 = 0;
    obj->field_24 = 0;
    obj->field_09 = 0;
    obj->field_04++;
    obj->field_1c = ref_other.p->field_1c;
    func_801b73f4_slot04_11(obj);
    func_80130768(obj, 1, seqs_8017c7f8);
    func_801b735c_slot04_11(obj);
}

void func_801b735c_slot04_11(Object *obj) {
    func_801b73f4_slot04_11(obj);
    if ((s16)obj->field_3a < 0) {
        obj->field_04 += 1;
    }
    func_80131094(obj);
}

void func_801b73ac_slot04_11(Object *o) {
    Object **p = (Object **)ref_other.p->field_3c;
    int i;

    i = 4;
    do {
        if (p == (Object **)o) {
            *(u32 *)o = 0;
            break;
        }
        p++;
    } while (--i != 0);
    func_8011f38c(o);
}

void func_801b73f4_slot04_11(Object *obj) {
    obj->pos_x = (u16)ref_other.p->pos_x;
    obj->pos_y = ref_other.p->pos_y + 0x10;
    if (obj->field_03 != 0) {
        obj->pos_x = obj->pos_x - 0xc;
        obj->pos_y = obj->pos_y - 6;
    }
}
