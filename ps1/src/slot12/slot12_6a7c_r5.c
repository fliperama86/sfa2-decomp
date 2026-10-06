/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80016e24_slot12(Object *obj);
void func_80016e78_slot12(Object *obj);
void func_80016ee0_slot12(Object *obj);
void func_80016f38_slot12(Object *obj);
extern Slot12Count data_8002c3a8_slot12[4];

void func_80016de4_slot12(Object *obj) {
    func_80016e24_slot12(obj);
    func_80016e78_slot12(obj);
    func_80016ee0_slot12(obj);
    func_80016f38_slot12(obj);
}

void func_80016e24_slot12(Object *o) {
    Slot12Obj *obj = (Slot12Obj *)o;
    u8 *p;
    int n;
    int i;

    data_8002c3a8_slot12[0].value = 0;
    p = (u8 *)obj->field_50;
    i = obj->field_5c;
    n = 0;
    if (*p != 0xff) {
        do {
            if (*p++ & 0x80) {
                n++;
            }
            i--;
        } while (i >= 0);
    }
    data_8002c3a8_slot12[0].value = n;
}

void func_80016e78_slot12(Object *o) {
    Slot12Obj *obj = (Slot12Obj *)o;
    u8 *p;
    int n;
    int i;
    int v;

    data_8002c3a8_slot12[1].value = 0;
    p = (u8 *)obj->field_50;
    i = obj->field_5c;
    n = 0;
    if (*p != 0xff) {
        do {
            v = *p++ & 0x7f;
            if ((unsigned)(v - 0xc) < 2 | (unsigned)(v - 0xe) < 2) {
                n++;
            }
            i--;
        } while (i >= 0);
    }
    data_8002c3a8_slot12[1].value = n;
}
