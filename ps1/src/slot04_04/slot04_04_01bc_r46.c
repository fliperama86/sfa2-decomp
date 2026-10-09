/* Reconstruction. Names/roles inferred, not original symbols. */
#include "../game.h"
#include "../protos.h"
#include "../externs.h"

extern SequenceStep **data_801c4670_slot04_04[];
extern SequenceStep **data_1f8000b4;
extern SequenceStep **data_1f800164;
extern SequenceStep **data_1f8000f0;
extern SequenceStep **data_1f8001a0;
void func_801b4080_slot04_04(Object *obj);
void func_801b40ec_slot04_04(Object *obj, Object *p);
void func_801b4198_slot04_04(Object *obj, Object *p);
void func_801b4244_slot04_04(Object *obj);

void func_801b3f84_slot04_04(Object *obj) {
    Object *p = obj->field_3c;

    obj->field_04 = obj->field_04 + 1;
    obj->field_1c = p->field_1c;
    obj->field_07 = p->kind;
    obj->field_0c = p->field_0c;
    obj->field_0d = p->field_0d;
    obj->field_0e = p->field_0e;
    obj->field_48 = 0;
    if (obj->field_03 == 0) {
        if (p->side == 0) {
            data_801c4670_slot04_04[0] = data_1f8000b4;
        } else {
            data_801c4670_slot04_04[1] = data_1f800164;
        }
    } else {
        if (p->side == 0) {
            data_801c4670_slot04_04[2] = data_1f8000f0;
        } else {
            data_801c4670_slot04_04[3] = data_1f8001a0;
        }
    }
    func_801b4080_slot04_04(obj);
}

void func_801b4080_slot04_04(Object *obj) {
    Object *p = obj->field_3c;

    obj->field_01 = 0;
    if (p->kind != obj->field_07) {
        func_801b4244_slot04_04(obj);
    } else if (obj->field_03 == 0) {
        func_801b40ec_slot04_04(obj, p);
    } else {
        func_801b4198_slot04_04(obj, p);
    }
}
