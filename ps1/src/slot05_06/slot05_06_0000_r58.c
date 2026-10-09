/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801dd53c_slot05_06[];
void func_80130678(Object *object, int arg);

void func_801ccf78_slot05_06(Object *obj) {
    if (((Slot04aObj *)obj)->field_47 != 0) {
        ((Slot04aObj *)obj)->field_47--;
        if (((Slot04aObj *)obj)->field_47 == 0) {
            game_state_second.field_4b = game_state.field_4b | (1 << obj->side);
        }
    }
    func_80130efc(obj);
}

void func_801ccfe8_slot05_06(Object *obj) {
    data_801dd53c_slot05_06[obj->field_06](obj);
}

void func_801cd028_slot05_06(Object *obj) {
    obj->field_06 = obj->field_06 + 1;
    obj->field_0b = obj->field_158;
    func_80130678(obj, 0);
}

void func_801cd05c_slot05_06(Object *object) {
    if (game_state.field_5c == 0) {
        object->field_06 = object->field_06 + 1;
    }
    func_80130efc(object);
}

void func_801cd098_slot05_06(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int arg = 0x29;

    obj->field_47 = 0x78;
    o->field_06 = o->field_06 + 1;
    if (game_state_second.field_a6 == 0) {
        arg = 0x28;
    }
    func_80130678(o, arg);
}
