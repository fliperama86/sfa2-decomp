/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80148608(Object *object) {
    table_8017cd38[object->field_05](object);
}

void func_80148648(Object *object) {
    ref_other.p = object->field_3c;
    if (ref_other.p->field_45 != 0) {
        object->field_05 = object->field_05 + 1;
        func_80130768(object, 15, seqs_8017c7f8);
    }
}
