/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_800e87bc_slot0f[])(Object *);
int load_memory_card_file_slot0f(int a);
void update_two_option_selection_slot0f(Object *o);

void func_800e1b84_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    s8 t;
    if (obj->countdown != 0) {
        obj->countdown = obj->countdown - 1;
    } else {
        if (obj->field_0b == 0) {
            o->field_0e = load_memory_card_file_slot0f(0);
        } else {
            o->field_0e = load_memory_card_file_slot0f(0x10);
        }
        t = -0x4c;
        o->field_0f = t;
        o->field_01 += 1;
        o->state_0 += 1;
    }
}

void func_800e1c04_slot0f(Object *o) {
    data_800e87bc_slot0f[(s8)o->field_01](o);
}

void func_800e1c44_slot0f(Object *o) {
    Slot0fObj *obj = (Slot0fObj *)o;
    o->field_01 += 1;
    obj->field_0c = 0;
    update_two_option_selection_slot0f(o);
}
