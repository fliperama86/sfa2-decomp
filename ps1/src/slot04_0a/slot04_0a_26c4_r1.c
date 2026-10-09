/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern u8 data_801c0690_slot04_0a[];
void func_801483a4(Object *object, int a_arg, int b_arg);
void func_801b3e68_slot04_0a(Object *obj);

void func_801b26c4_slot04_0a(Object *obj) {
    int t;
    func_80130efc(obj);
    t = (s16)obj->field_3a;
    if (t & 0x80) {
        obj->field_07++;
        func_801204f4(obj, obj->side, 0xd);
        obj->field_45 = 1;
        obj->field_67 = 0;
        obj->field_54 = 0;
        obj->field_50 = 0;
        obj->field_58 = 0;
        t = 0x90000;
        if (obj->field_0b == 0) {
            t = 0xfff70000;
        }
        obj->field_4c = t;
        func_801b3e68_slot04_0a(obj);
        func_801307e0(obj, (obj->field_12a >> 1) + 0x3c);
    } else if (t == 2) {
        obj->field_165 = 0;
        t = 0;
        if (obj->field_4b == 0) {
            obj->other->field_6b = 0xa;
            t = (obj->field_12a >> 1) + 1;
        }
        obj->field_27b = data_801c0690_slot04_0a[t];
        func_801204f4(obj, obj->side, 5);
    } else if (t == 1) {
        if (obj->field_4b == 0) {
            t = -1;
        }
        obj->field_165 = t;
        obj->field_3a = obj->field_3a & 0xff00;
        func_801483a4(obj, 0x2f, 0x41);
    }
}
