/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern int data_8002c77c_slot28[];
extern ObjectFn data_8002c78c_slot28[];
extern ObjectFn data_8002c79c_slot28[];
void func_80015d3c_slot28(Object *obj);

void func_8001591c_slot28(Object *obj) {
    obj->vel_x = 0x10;
    obj->field_0e = 0;
    obj->facing_side = 0;
    obj->variant_index = 0;
    obj->state_0++;
    func_80130768(obj, data_8002c77c_slot28[obj->table_index], (SequenceStep **)obj->box_tables);
}

void func_80015978_slot28(Object *obj) {
    data_8002c78c_slot28[obj->table_index](obj);
}

void func_800159b8_slot28(Object *obj) {
    data_8002c79c_slot28[obj->state_1](obj);
    advance_sequence_checked(obj);
    func_80120028(obj);
}

void func_80015a14_slot28(Object *obj) {
    if (obj->variant_index != 0) {
        obj->state_1++;
        obj->field_0c = 0;
        func_80130768(obj, 4, (SequenceStep **)obj->box_tables);
    }
    func_80015d3c_slot28(obj);
}
