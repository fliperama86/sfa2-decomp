/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern BoxTables *data_801be124_slot04_0c[];
extern BoxTables *data_801be150_slot04_0c[];
extern BoxTables *data_801be188_slot04_0c[];
extern void (*data_801be1e0_slot04_0c[])(Object *, Object *);

void func_801b3ff0_slot04_0c(Object *obj, Object *parent);
void func_8011f14c(Slab172 *o);

void func_801b3e10_slot04_0c(Object *obj) {
    obj->field_05++;
    obj->field_3c->field_14c = 0;
    func_80138070(obj, 6);
}

void func_801b3e44_slot04_0c(Object *obj) {
    if ((s16)obj->field_3a & 0x8000) {
        obj->field_04 = 3;
        obj->field_05 = 0;
        obj->field_06 = 0;
        obj->field_07 = 0;
    }
    func_80131094(obj);
}

void func_801b3e88_slot04_0c(Object *o) {
    Object *p = o->field_3c;
    u8 n = p->field_240 - 1;
    p->field_240 = n;
    if (n == 0) {
        p->field_14c = 0;
    }
    func_8011f14c((Slab172 *)o);
}

void func_801b3ed0_slot04_0c(Object *obj) {
    if (obj->field_ac == 6) {
        obj->box_tables = data_801be124_slot04_0c[(s16)obj->field_5c];
    } else if (obj->field_ac == 8) {
        obj->box_tables = data_801be150_slot04_0c[(s16)obj->field_5c];
    } else {
        obj->box_tables = data_801be188_slot04_0c[(s16)obj->field_5c];
    }
}

void func_801b3f54_slot04_0c(Object *obj) {
    data_801be1e0_slot04_0c[obj->field_04](obj, obj->field_3c);
}

void func_801b3f94_slot04_0c(Object *obj, Object *parent) {
    obj->field_04++;
    obj->field_1c = parent->field_1c;
    obj->field_03 = parent->kind;
    obj->field_0c = parent->field_0c;
    obj->field_0e = parent->field_0e;
    obj->field_48 = 0;
    func_801b3ff0_slot04_0c(obj, parent);
}
