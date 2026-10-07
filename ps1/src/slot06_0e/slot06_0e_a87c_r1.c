/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep *data_801ef40c_slot06_0e[];
extern SequenceStep *data_801ef418_slot06_0e[];
extern u8 data_801ef4b4_slot06_0e[];
extern u16 data_801ef4bc_slot06_0e[];
extern ObjectFn data_801ef4c4_slot06_0e[];
extern u8 data_801ef4d4_slot06_0e[];
extern s16 data_801f8474_slot06_0e;
extern u8 data_801fb1f8_slot06_0e[];

int rand(void);
void func_801eaa28_slot06_0e(Object *obj);

void func_801ea9f4_slot06_0e(Object *obj) {
    obj->field_04++;
}

void func_801eaa08_slot06_0e(Object *obj) {
    func_8011f240((Slab172 *)obj);
}

void func_801eaa28_slot06_0e(Object *obj) {
    Object *c = (Object *)obj->field_34;

    data_801f8474_slot06_0e = obj->field_03 + obj->field_46;
    data_801f8474_slot06_0e = data_801fb1f8_slot06_0e[data_801f8474_slot06_0e];
    c->field_01 = 0;
    if (data_801f8474_slot06_0e == 3) {
        c->field_01 = 1;
    }
    obj->pos_x = data_801ef4bc_slot06_0e[data_801f8474_slot06_0e];
    data_801f8474_slot06_0e = data_801ef4b4_slot06_0e[data_801f8474_slot06_0e];
    func_80130700(obj, data_801ef40c_slot06_0e[data_801f8474_slot06_0e]);
}

void func_801eaafc_slot06_0e(Object *obj) {
    data_801ef4c4_slot06_0e[obj->field_04](obj);
}

void func_801eab3c_slot06_0e(Object *obj) {
    u32 i;
    int off;
    u8 *tbl;

    obj->field_0a = 1;
    obj->field_0f = 1;
    obj->field_04 = 1;
    obj->field_46 = 0;
    obj->field_0d = 0;
    obj->field_0c = 0;
    obj->field_81 = 4;
    func_80130700(obj, data_801ef418_slot06_0e[0]);
    off = rand() & 0xf;
    off <<= 3;
    i = 0;
    tbl = data_801ef4d4_slot06_0e;
    for (; i < 8; i++) {
        data_801fb1f8_slot06_0e[i] = *(u8 *)(off + i + (u32)tbl);
    }
}

void func_801eabd4_slot06_0e(Object *obj) {
    if ((game_state.field_65 | game_state.field_74) == 0) {
        func_80131094(obj);
    }
    if (((Slot06Layer *)cam_obj)->field_16 >= 0x500) {
        if (((Slot06Layer *)data_801aa544)->field_05 == 2) {
            obj->field_04++;
        }
        func_8011ffdc(obj);
    }
}

void func_801eac5c_slot06_0e(Object *obj) {
    obj->field_04++;
}

void func_801eac70_slot06_0e(Object *obj) {
    func_8011f240((Slab172 *)obj);
}
