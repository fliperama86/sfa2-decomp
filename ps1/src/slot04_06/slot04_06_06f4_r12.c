/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_80130678(Object *object, int arg);

void func_801b502c_slot04_06(Object *obj) {
    obj->field_06 = obj->field_06 + 1;
    obj->field_0b = obj->field_158;
    func_80130678(obj, 0);
}

void func_801b5060_slot04_06(Object *object) {
    if (game_state.field_5c == 0) {
        object->field_06 = object->field_06 + 1;
    }
    func_80130efc(object);
}

void func_801b509c_slot04_06(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int arg = 0x29;

    obj->field_47 = 0x78;
    o->field_06 = o->field_06 + 1;
    if (game_state_second.field_a6 == 0) {
        arg = 0x28;
    }
    func_80130678(o, arg);
}
