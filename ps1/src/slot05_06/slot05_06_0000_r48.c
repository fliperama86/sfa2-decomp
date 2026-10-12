/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

void func_801cc228_slot05_06(Object *obj);
extern ObjectFn data_801dd4bc_slot05_06[];

void func_801cc194_slot05_06(Object *obj) {
    obj->hit_count_3++;
    update_sequence(obj);
    ((Slot04bObj *)obj)->countdown--;
    if (((Slot04bObj *)obj)->countdown != 0) {
        func_801cc228_slot05_06(obj);
    } else {
        if (obj->active_state != 0) {
            obj->state_0 = 1;
            obj->state_1 = 0;
            obj->state_2 = 3;
            obj->state_3 = 1;
        }
        func_80142c70(obj);
    }
}

void func_801cc228_slot05_06(Object *o) {
    Slot04aObj *obj = (Slot04aObj *)o;

    if (obj->field_134 != 0) {
        if (obj->field_27d == 0) {
            obj->field_27d = ((Object *)obj)->hit_count_3;
        }
    } else if (obj->timer != 0) {
        obj->timer -= 1;
    }
    func_80142fe8(o);
}

void func_801cc294_slot05_06(Object *obj) {
    data_801dd4bc_slot05_06[obj->state_3](obj);
}
