/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_80152444(Object *object) {
    table_80180284[object->field_48 >> 1](object);
}

void func_80152488(Object *object) {
    table_80180294[object->state_1](object);
    func_80120028(object);
}

void func_801524dc(Object *object) {
    object->state_1++;
    func_80152ad8(object, object->field_48);
    func_801528e0(object);
}

void func_8015251c(Object *object) {
    if (func_801528b0(object)) {
        object->state_1++;
        func_80130768(object, (object->field_48 * 2 + 0x33) & 0xff, table_8017c7f8);
    }
    func_801528e0(object);
    advance_sequence_checked(object);
}

void func_80152588(Object *object) {
    if ((s16)object->step_flags >= 0) {
        advance_sequence_checked(object);
    } else {
        object->state_0++;
    }
}

void func_801525d0(Object *object) {
    func_80152488(object);
}

void func_801525f0(Object *object) {
    if (func_801528b0(object)) {
        object->state_0++;
    } else {
        table_801802a0[object->state_1](object);
        func_80120028(object);
    }
}

void func_80152668(Object *object) {
    object->state_1++;
    func_80130768(object, (table_801802a8[(s16)object->field_46] + (object->field_03 >> 2)) & 0xff, table_8017c7f8);
    func_801529f4(object);
}

void func_801526d0(Object *object) {
    advance_sequence_checked(object);
    func_801529f4(object);
}
