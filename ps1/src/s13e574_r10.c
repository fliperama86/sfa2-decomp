/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80140f98(void) {
    table_8017ac44[game_state.config->field_12]();
}

void func_80140fe0(Object *object) {
    if (object->field_cd != 0) {
        func_80141058(object);
    } else {
        if (object->field_134 & 0xff00) func_80130efc(object);
        if (*(u8 *)&object->field_134 != 0) func_80130efc(object);
    }
}

void func_80141058(Object *object) {
    if (func_8014a170(object, table_8017d618)) func_80130efc(object);
    if (func_8014a170(object, table_8017d618)) func_80130efc(object);
}
