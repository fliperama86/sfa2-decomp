/* Reconstruction. Names/roles inferred, not original symbols. */
#include "game.h"
#include "externs.h"
#include "protos.h"



void func_8012b590(Object *object) {
    object->field_16a = 0;
    handler_table_19d8[object->field_07](object);
    if (game_state.field_30 != 0 && game_state.mode != object->side + 1) {
        object->field_1a1 = object->field_1a1 - 1;
        if (object->field_1a1 & 0x80) {
            object->field_1a0 = 0;
        }
    }
}
