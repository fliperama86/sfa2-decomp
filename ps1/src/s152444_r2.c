/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80152774(Object *object) {
    object->field_05++;
    func_80152ad8(object, 0);
    func_801528e0(object);
}

void func_801527b4(Object *object) {
    ref_other.p = object->field_3c;
    if (ref_other.p->field_45 == 0) {
        object->field_05++;
        func_80130768(object, 0x33, (SequenceStep **)table_8017c7f8);
    }
    func_801528e0(object);
    func_80131094(object);
}

void func_8015282c(Object *object) {
    if ((s16)object->field_3a < 0) {
        object->field_04++;
    } else {
        func_80131094(object);
    }
}

/* The parameter is passed on to func_8011f240, which takes it as a Slab172: the original sets no argument register before that call, so the callee receives what this function's caller passed. The table table_80180274 holds this function and is declared with this parameter. */
void func_80152870(Object *p) {
    func_8011f240((Slab172 *)p);
}

/* The parameter is passed on to func_8011f240, which takes it as a Slab172: the original sets no argument register before that call, so the callee receives what this function's caller passed. The table table_80180274 holds this function and is declared with this parameter. */
void func_80152890(Object *p) {
    func_8011f240((Slab172 *)p);
}

u8 func_801528b0(Object *object) {
    ref_other.p = object->field_3c;
    return (*(u32 *)&ref_other.p->field_258 & 0xffffff) == 0;
}
