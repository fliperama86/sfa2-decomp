/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern Slot12Count data_8002c3a8_slot12[4];
extern void (*data_80028a30_slot12[])(Object *);
void func_80016ff4_slot12(Object *obj);

void func_80016ee0_slot12(Object *o) {
    Slot12Obj *obj = (Slot12Obj *)o;
    u8 *p;
    int n;
    int i;
    int v;

    data_8002c3a8_slot12[2].value = 0;
    p = (u8 *)obj->field_50;
    i = obj->field_5c;
    n = 0;
    if (*p != 0xff) {
        do {
            v = *p++ & 0x7f;
            if (v >= 0x10) {
                n++;
            }
            i--;
        } while (i >= 0);
    }
    data_8002c3a8_slot12[2].value = n;
}

void func_80016f38_slot12(Object *o) {
    Slot12Obj *obj = (Slot12Obj *)o;
    u8 *p;
    int n;
    int i;
    int v;

    data_8002c3a8_slot12[3].value = 0;
    p = (u8 *)obj->field_50;
    i = obj->field_5c;
    n = 0;
    if (*p != 0xff) {
        do {
            v = *p++ & 0x7f;
            if ((v == 7) | (v == 0xb)) {
                n++;
            }
            i--;
        } while (i >= 0);
    }
    data_8002c3a8_slot12[3].value = n;
}

void func_80016fa0_slot12(Object *obj) {
    data_80028a30_slot12[obj->field_05](obj);
    func_80016ff4_slot12(obj);
}
