/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b34e4_slot04_04(Object *obj);

void func_801b33bc_slot04_04(Object *obj) {
    int a;
    obj->field_46 = (s16)obj->field_46 - 0x100;
    if ((obj->field_46 & 0xff00) == 0) {
        obj->state_3++;
        obj->field_27c = 0;
        ((Slot04bObj *)obj)->field_27d = 0;
        obj->field_46 = (u8)obj->field_46 + 0x1400;
        if (obj->field_4b != 0) {
            a = 0x48;
        } else {
            a = ((Slot04bObj *)obj)->field_c6 + 0x1e;
        }
        obj->field_2a1 = a;
        obj->field_180 = 0;
    }
    func_80142fe8(obj);
    update_sequence(obj);
}

void func_801b3450_slot04_04(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int t;

    obj->field_27c++;
    update_sequence(o);
    o->field_46 = t = o->field_46 - 1;
    if ((t & 0xff) != 0) {
        func_801b34e4_slot04_04(o);
    } else {
        if (o->field_45 != 0) {
            o->state_0 = 1;
        }
        o->state_2 = 3;
        o->state_1 = 0;
        o->state_3 = 1;
        func_80142c70(o);
    }
}

void func_801b34e4_slot04_04(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;

    if (o->pressed_bits != 0) {
        if (obj->field_27d == 0) {
            obj->field_27d = obj->field_27c;
        }
    }
    func_80142fe8(o);
}

void func_801b3530_slot04_04(Object *object) {
    u8 *p = (u8 *)object->slots;
    int i;
    u8 z = 0;

    for (i = 0x50; i >= 0; i--) {
        *p++ = z;
    }
}
