/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern ObjectFn data_801dd53c_slot05_06[];

void func_801ccf78_slot05_06(Object *obj) {
    if (((Slot04aObj *)obj)->timer != 0) {
        ((Slot04aObj *)obj)->timer--;
        if (((Slot04aObj *)obj)->timer == 0) {
            game_state_second.side_bits = game_state.field_4b | (1 << obj->side);
        }
    }
    update_sequence(obj);
}

void func_801ccfe8_slot05_06(Object *obj) {
    data_801dd53c_slot05_06[obj->state_2](obj);
}

void func_801cd028_slot05_06(Object *obj) {
    obj->state_2 = obj->state_2 + 1;
    obj->field_0b = obj->field_158;
    set_sequence_by_side(obj, 0);
}

void func_801cd05c_slot05_06(Object *object) {
    if (game_state.field_5c == 0) {
        object->state_2 = object->state_2 + 1;
    }
    update_sequence(object);
}

void func_801cd098_slot05_06(Object *o) {
    Slot04bObj *obj = (Slot04bObj *)o;
    int arg = 0x29;

    obj->field_47 = 0x78;
    o->state_2 = o->state_2 + 1;
    if (game_state_second.field_a6 == 0) {
        arg = 0x28;
    }
    set_sequence_by_side(o, arg);
}
