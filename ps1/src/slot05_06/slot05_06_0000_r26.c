/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801cc814_slot05_06(Object *obj);
extern ObjectFn data_801dd3ac_slot05_06[];

void func_801ca168_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if (obj->field_3a != 0) {
        o->field_07++;
        obj->field_3a = 0;
    }
    func_80130efc(o);
}

void func_801ca1a8_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if (obj->field_3b & 0x80) {
        o->other->field_249 = 5;
        func_801312b8(o);
    } else {
        if (obj->field_3a != 0) {
            obj->field_3a = 0;
            o->field_17b = 0;
        }
        if (o->field_4c >= 0) {
            func_801cc814_slot05_06(o);
        }
        func_80142adc(o);
        func_80130efc(o);
    }
}

void func_801ca234_slot05_06(Object *obj) {
    data_801dd3ac_slot05_06[obj->field_07](obj);
}

void func_801ca274_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    o->field_07++;
    obj->field_1c3 = 0x32;
    func_80145d20(o);
    func_801428e4(o);
    func_80138b38(&game_state, o);
    func_801204f4(o, o->side, 7);
    func_801307e0(o, 0x22);
}
