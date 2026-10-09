/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

Block172 *func_8011f1e0(void);

void func_80146998(Object *object) {
    Object *spawned = func_80146a98(object, 6);
    if (spawned) {
        if (object->field_0b != 0) {
            spawned->pos_x = spawned->pos_x + table_8017ca64[object->kind].first;
        } else {
            spawned->pos_x = spawned->pos_x - table_8017ca64[object->kind].first;
        }
        spawned->pos_y = spawned->pos_y - table_8017ca64[object->kind].second;
    }
    func_80120554(object, object->side, 0x313);
    if (table_8017ca38[object->kind] != -1) {
        func_801204f4(object, object->side, table_8017ca38[object->kind]);
    }
}

Object *func_80146a98(Object *object, int unused) {
    ref_other.p = (Object *)func_8011f1e0();
    if (ref_other.p != 0) {
        ref_other.p->field_00 = 1;
        ref_other.p->field_02 = 5;
        ref_other.p->field_03 = 0;
        ref_other.p->field_0e = object->field_0e;
        ref_other.p->field_09 = 4;
        ref_other.p->field_48 = 0x12;
        ref_other.p->pos_x = object->pos_x;
        ref_other.p->pos_y = object->pos_y;
        ref_other.p->field_0b = object->field_0b;
        ref_other.p->field_1c = object->field_1c;
        ref_other.p->field_0c = 0;
        return ref_other.p;
    } else {
        return 0;
    }
}
