/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"

extern void (*table_801802c0[])(Object *);

void func_80152700(Object *object) {
    if (game_state.field_6a != 0) {
        object->field_04++;
    } else {
        table_801802c0[object->field_05](object);
        func_80120028(object);
    }
}
