/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

Block172 *func_8011f1e0(void);

void func_80149aa8(Object *object) {
    object->field_04 = 2;
    object->field_01 = 0;
}

void func_80149ab8(Object *object) {
    func_80120554(object->field_3c, object->field_3c->side, 0x329);
    func_8011f240(object);
}

void func_80149af8(Object *object) {
    ref_other.p = (Object *)func_8011f1e0();
    if (ref_other.p != 0) {
        ref_other.p->field_00++;
        ref_other.p->field_02 = 0x6d;
        ref_other.p->field_03 = object->side;
        ref_other.p->field_3c = object;
    }
}
