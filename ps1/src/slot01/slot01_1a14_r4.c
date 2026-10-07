/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot01State55e94 data_80055e94_slot01;
Block172 *func_8011f1e0(void);

void func_80011f74_slot01(Object *obj) {
    Object *p = (Object *)func_8011f1e0();
    int t;
    if (p != 0) {
        p->field_00 = 1;
        p->field_02 = 0x95;
        p->field_66 = 0;
        p->field_03 = 0x81;
        t = obj->field_48;
        p->field_0b = t;
        if (t != 0) {
            p->field_03 = 0x86;
        }
        p->field_4c = 0;
        p->field_50 = 0;
        p->field_3c = obj;
        obj->field_28 = (u32)p;
    }
}

void func_80011fe8_slot01(Object *obj, Slot01Rec152ec *a1) {
    u16 v;
    int c;
    data_80055e94_slot01.cursor = a1;
    c = a1->count;
    data_80055e94_slot01.count = c;
    v = a1->value;
    data_80055e94_slot01.value = v;
    obj->field_7c = (v & 0x1f) + 0x1e0;
}

void func_80012020_slot01(Object *obj) {
    int t = (u16)data_80055e94_slot01.count - 1;
    data_80055e94_slot01.count = t;
    if ((s16)t == 0) {
        u16 v;
        int c;
        data_80055e94_slot01.cursor++;
        if (*(int *)data_80055e94_slot01.cursor < 0) {
            data_80055e94_slot01.cursor = (Slot01Rec152ec *)*(int *)data_80055e94_slot01.cursor;
        }
        c = data_80055e94_slot01.cursor->count;
        data_80055e94_slot01.count = c;
        v = data_80055e94_slot01.cursor->value;
        data_80055e94_slot01.value = v;
        obj->field_7c = (v & 0x1f) + 0x1e0;
    }
}
