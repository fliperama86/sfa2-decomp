/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

Block172 *func_8011f1e0(void);

void func_801e99ac_slot06_11(Object *obj, int a) {
    ref_other.p = (Object *)func_8011f1e0();
    if (ref_other.p != 0) {
        ref_other.p->field_00 += 1;
        ref_other.p->field_02 = 0x4b;
        ref_other.p->field_03 = 1;
        ref_other.p->pos_x = (u16)obj->pos_x;
        ref_other.p->pos_y = (u16)obj->field_70;
        ref_other.p->field_45 = a;
    }
}

void func_801e9a64_slot06_11(Object *obj, int a) {
    ref_other.p = (Object *)func_8011f1e0();
    if (ref_other.p != 0) {
        ref_other.p->field_00 += 1;
        ref_other.p->field_02 = 0x4b;
        ref_other.p->field_03 = 2;
        ref_other.p->field_03 = a + ref_other.p->field_03;
        ref_other.p->pos_x = (u16)obj->pos_x;
        ref_other.p->pos_y = (u16)obj->field_70;
        ref_other.p->field_0b = obj->side;
        ref_other.p->field_3c = obj;
    }
}

void func_801e9b4c_slot06_11(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        obj->field_04 += 1;
    }
    func_80131094(obj);
    func_8011ffdc(obj);
}

void func_801e9ba0_slot06_11(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        obj->field_04 += 1;
    }
    obj->field_0b = obj->field_0b ^ 1;
    ref_other.p = obj->field_3c;
    obj->pos_x = (u16)ref_other.p->pos_x;
    func_80131094(obj);
    func_8011ffdc(obj);
}

void func_801e9c1c_slot06_11(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        obj->field_04 += 1;
    }
    obj->field_0b = obj->field_0b ^ 1;
    func_80131094(obj);
    func_8011ffdc(obj);
}

void func_801e9c7c_slot06_11(Object *obj) {
    if ((s16)obj->field_3a < 0) {
        obj->field_04 += 1;
    }
    func_80131094(obj);
    if (((game_state.field_1d + obj->field_0b) & 1) != 0) {
        func_8011ffdc(obj);
    } else {
        obj->field_01 = 0;
    }
}

void func_801e9cfc_slot06_11(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
