/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b7920_slot04_02(Object *obj, u8 a);
void func_801b7908_slot04_02(Object *obj, u8 a, u8 b, u8 c, u8 d);
void func_801b77ac_slot04_02(Object *obj, u8 a);
void func_801b77fc_slot04_02(Object *obj, u8 a);
void func_801b784c_slot04_02(Object *obj, u8 a);
void func_801b789c_slot04_02(Object *obj, u8 a);
extern void *data_801c7734_slot04_02[];
extern void *data_801c772c_slot04_02[];
extern void *data_801c7730_slot04_02[];
void func_801b78ec_slot04_02(Object *obj, void **tbl);

void func_801b7660_slot04_02(Object *obj) {
    int a = 0x10;

    obj->state_1++;
    if (obj->table_index != 2 && obj->table_index != 8) {
        a = 0xf;
    }
    func_801b7920_slot04_02(obj, a);
}

void func_801b76ac_slot04_02(Object *obj) {
    if ((s16)obj->step_flags < 0) {
        func_801b7908_slot04_02(obj, 3, 0, 0, 0);
    }
    advance_sequence_checked(obj);
}

void func_801b76f8_slot04_02(Object *obj) {
    Object *p = obj->parent;
    u8 n = p->link_flags - 1;

    p->linked_object_3 = 0;
    p->link_flags = n;
    func_8011f14c((Slab172 *)obj);
}

int func_801b772c_slot04_02(Object *obj, Object *parent) {
    u8 a = obj->vel_table_index;

    if (obj->table_index == 6) {
        func_801b789c_slot04_02(obj, a);
        return 1;
    }
    if (obj->table_index == 8) {
        func_801b784c_slot04_02(obj, a);
        return 1;
    }
    if (obj->table_index == 4) {
        func_801b77fc_slot04_02(obj, a);
        return 1;
    }
    if (obj->table_index == 2) {
        func_801b77ac_slot04_02(obj, a);
        return 1;
    }
    return -1;
}

void func_801b77ac_slot04_02(Object *obj, u8 a) {
    void **t = data_801c7734_slot04_02;

    if (a == 3) {
        t = data_801c772c_slot04_02;
    }
    if (a == 4) {
        t = data_801c7730_slot04_02;
    }
    func_801b78ec_slot04_02(obj, t);
}
