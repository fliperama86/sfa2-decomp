/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"


void func_80140f98(void) {
    table_8017ac44[game_state.config->handler_index]();
}

void func_80140fe0(Object *object) {
    if (object->mode_flag_2 != 0) {
        func_80141058(object);
    } else {
        if (object->pressed_bits & 0xff00) update_sequence(object);
        if (*(u8 *)&object->pressed_bits != 0) update_sequence(object);
    }
}

void func_80141058(Object *object) {
    if (func_8014a170(object, table_8017d618)) update_sequence(object);
    if (func_8014a170(object, table_8017d618)) update_sequence(object);
}
