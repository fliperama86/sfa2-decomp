/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801495f8(Object *object, int x, int y);
void func_8011fe40(Object *object);
void func_80149af8(Object *object);
void func_80143184(Object *object);
void func_80142fe8(Object *object);
void func_801b3f58_slot04_0a(Object *object);

void func_801b37fc_slot04_0a(Object *obj) {
    int a = 0;
    int b;
    if ((s16)obj->field_3a & 0xff00) {
        b = 0x59;
        obj->field_165 = 0xff;
        obj->field_46 = 0x1e01;
        obj->field_07 = obj->field_07 + 1;
        if (obj->field_45 != 0) {
            a = -0x1d;
            b = 0x56;
        }
        func_801495f8(obj, a, b);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b3f58_slot04_0a(obj);
    }
    func_80130efc(obj);
}

void func_801b3894_slot04_0a(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;
    u8 a = 0x48;
    o->field_46 = (s16)o->field_46 - 0x100;
    if ((o->field_46 & 0xff00) == 0) {
        o->field_07++;
        o->field_27c = 0;
        obj->field_27d = 0;
        o->field_46 = (u8)o->field_46 | 0x1400;
        if (o->field_4b == 0) {
            a = obj->field_c6 + 0x1e;
        }
        o->field_2a1 = a;
        o->field_180 = 0;
    }
    func_80142fe8(o);
    func_80130efc(o);
}
