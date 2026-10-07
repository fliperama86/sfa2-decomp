/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b4818_slot04_06(Object *obj);
void func_801b4850_slot04_06(Object *obj);
void func_80146960(Object *object);

void func_801b2414_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    func_801b4850_slot04_06(o);
    if (o->field_50 <= 0x1ffff && obj->field_1c8 == 0) {
        obj->field_1c8 = 1;
        func_801307e0(o, 0x28);
    } else if (o->field_50 >= 0) {
        func_801b4818_slot04_06(o);
        func_80130efc(o);
    } else {
        o->field_50 = -0x80000;
        o->field_07++;
        func_801204f4(o, o->side, 8);
        func_801307e0(o, 0x20);
    }
}

void func_801b24cc_slot04_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    func_801b4850_slot04_06(o);
    if (o->pos_y < (s16)obj->field_70) {
        func_801b4818_slot04_06(o);
    } else {
        o->field_07++;
        func_80146960(o);
        o->field_45 = 0;
        func_80120554(o, o->side, 0x319);
        obj->field_1c2 = 0xa;
        o->pos_y = obj->field_70;
        game_state.field_63 = 0xa;
        func_801307e0(o, 0x21);
    }
}
