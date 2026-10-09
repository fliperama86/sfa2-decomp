/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b51f8_slot04_06(Object *obj);

void func_801b4064_slot04_06(Object *obj) {
    s16 a;
    int b;

    if (((Slot04aObj *)obj)->field_3b != 0) {
        obj->field_165 = 0xff;
        ((Slot04aObj *)obj)->field_47 = 0x1e;
        ((Slot04aObj *)obj)->field_46 = 1;
        obj->field_07++;
        if (obj->field_45 == 0) {
            a = -8;
            b = 0x51;
        } else {
            a = -0xa;
            b = 0x6a;
        }
        func_801495f8(obj, a, b);
        func_8011fe40(obj);
        func_80149af8(obj);
        func_80143184(obj);
        func_801b51f8_slot04_06(obj);
    }
    func_80130efc(obj);
}

void func_801b410c_slot04_06(Object *obj) {
    int a;
    ((Slot04aObj *)obj)->field_47--;
    if (((Slot04aObj *)obj)->field_47 == 0) {
        ((Slot04aObj *)obj)->field_47 = 0x14;
        obj->field_07++;
        obj->field_27c = 0;
        ((Slot04aObj *)obj)->field_27d = 0;
        if (obj->field_4b == 0) {
            a = ((Slot04aObj *)obj)->field_c6 + 0x1e;
        } else {
            a = 0x48;
        }
        obj->field_2a1 = a;
        obj->field_180 = 0;
    }
    func_80142fe8(obj);
    func_80130efc(obj);
}
