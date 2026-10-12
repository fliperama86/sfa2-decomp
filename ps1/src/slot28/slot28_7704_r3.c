/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern void (*data_80032a4c_slot28[])(Object *);
extern void (*data_80032a54_slot28[])(Object *);

void func_80017b54_slot28(Object *obj) {
    int arg;
    obj->countdown = 0x1818;
    obj->field_0e = 0;
    obj->field_0c = 0;
    obj->facing_side = 0;
    obj->variant_index = 0;
    obj->state_0++;
    arg = 1;
    if (obj->table_index == 1) {
        arg = 3;
    }
    func_80130768(obj, arg, (SequenceStep **)obj->box_tables);
}

void func_80017bac_slot28(Object *obj) {
    advance_sequence_checked(obj);
    data_80032a4c_slot28[obj->table_index](obj);
}

void func_80017bfc_slot28(Object *obj) {
    data_80032a54_slot28[obj->state_1](obj);
    obj->field_01 = 1;
}
