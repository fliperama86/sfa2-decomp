/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void *data_801c7720_slot04_02[];
extern void *data_801c7714_slot04_02[];
extern void *data_801c7718_slot04_02[];
void func_801b78ec_slot04_02(Object *obj, void **tbl);
extern void *data_801c76f4_slot04_02[];
extern void *data_801c76cc_slot04_02[];
extern void *data_801c76dc_slot04_02[];
extern void *data_801c76ac_slot04_02[];
extern void *data_801c7684_slot04_02[];
extern void *data_801c7694_slot04_02[];

void func_801b77fc_slot04_02(Object *obj, u8 a) {
    void **t = data_801c7720_slot04_02;

    if (a == 6) {
        t = data_801c7714_slot04_02;
    }
    if (a == 7) {
        t = data_801c7718_slot04_02;
    }
    func_801b78ec_slot04_02(obj, t);
}

void func_801b784c_slot04_02(Object *obj, u8 a) {
    void **t = data_801c76f4_slot04_02;

    if (a == 0xc) {
        t = data_801c76cc_slot04_02;
    }
    if (a == 0xd) {
        t = data_801c76dc_slot04_02;
    }
    func_801b78ec_slot04_02(obj, t);
}

void func_801b789c_slot04_02(Object *obj, u8 a) {
    void **t = data_801c76ac_slot04_02;

    if (a == 9) {
        t = data_801c7684_slot04_02;
    }
    if (a == 0xa) {
        t = data_801c7694_slot04_02;
    }
    func_801b78ec_slot04_02(obj, t);
}

void func_801b78ec_slot04_02(Object *o, void **tbl) {
    Slot04bObj *obj = (Slot04bObj *)o;

    obj->field_6c = tbl[obj->field_5c];
}

void func_801b7908_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d) {
    obj->field_04 = a;
    obj->field_05 = b;
    obj->field_06 = c;
    obj->field_07 = d;
}

void func_801b7920_slot04_02(Object *obj, u8 a) {
    if (obj->field_66 != 0) {
        func_80130700(obj, seqs_154_right[a]);
    } else {
        func_80130700(obj, seqs_a4_left[a]);
    }
}
