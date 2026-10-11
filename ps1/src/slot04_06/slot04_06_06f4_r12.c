/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801b502c_slot04_06(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->field_0b = obj->field_158;
    func_80130678(obj, 0);
}

void func_801b5060_slot04_06(Object *object) {
    if (game_state.field_5c == 0) {
        object->state_2 = object->state_2 + 1;
    }
    update_sequence(object);
}

void func_801b509c_slot04_06(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int arg = 0x29;

    obj->field_47 = 0x78;
    o->state_2 = o->state_2 + 1;
    if (game_state_second.field_a6 == 0) {
        arg = 0x28;
    }
    func_80130678(o, arg);
}
